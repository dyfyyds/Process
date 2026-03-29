#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDialog>
#include <QListWidgetItem>
#include <warning.h>
#include "TitleItem.h"
#include "NodeItem.h"
#include "ScheDuler.h"
#include "ListSorter.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_timer(new QTimer(this))
    , isStop(true)
    , isExist(false)
{
    ui->setupUi(this);

    this->m_scheduler = std::make_shared<Scheduler>();
    GuiInit();

    ui->SetSingleButton->setEnabled(false);
    ui->SetAutoButton->setEnabled(false);

    //行数改变时信号
    connect(ui->ReadyQueue,&QListWidget::currentRowChanged,this,[this](int currentRow){
        if(currentRow >= 2 || !isStop){
            ui->SetSingleButton->setEnabled(true);
            ui->SetAutoButton->setEnabled(true);
        }
        else{
            ui->SetSingleButton->setEnabled(false);
            ui->SetAutoButton->setEnabled(false);
        }
    });


    //连接定时器  调用start开始执行
    connect(m_timer,&QTimer::timeout,this,&MainWindow::on_SetSingleButton_clicked);


    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    //设置透明
    setAttribute(Qt::WA_TranslucentBackground);

    setWindowIcon(QIcon(":/icon/process.png"));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GuiInit()
{
    //完成队列设置成两行
    ui->FinishQueue->setColumnCount(1);
    ui->FinishQueue->verticalHeader()->setVisible(false);

    QStringList headers;
    headers << "进程名称";
    ui->FinishQueue->setHorizontalHeaderLabels(headers);

    QLabel* l = new QLabel;
    l->setText("就绪队列");

    QListWidgetItem* item = new QListWidgetItem;
    ui->ReadyQueue->addItem(item);
    ui->ReadyQueue->setItemWidget(item,l);

    QListWidgetItem* titleItem = new QListWidgetItem;
    ui->ReadyQueue->addItem(titleItem);
    TitleItem* title = new TitleItem(ui->ReadyQueue);
    titleItem->setSizeHint(title->sizeHint());
    ui->ReadyQueue->setItemWidget(titleItem,title);

    //优化字体
    QFont font;
    font.setBold(true);

    ui->Title->setFont(font);
    ui->Name->setFont(font);
    ui->Label->setFont(font);
    ui->Status->setFont(font);
    ui->Priority->setFont(font);
    ui->Rtime->setFont(font);


    connect(this,&MainWindow::isExecute,this,[this](bool b){
        if(b){
            ui->AddProcessButton->setEnabled(false);
            ui->RestButton->setEnabled(false);
        }
        else{
            ui->AddProcessButton->setEnabled(true);
            ui->RestButton->setEnabled(true);
        }
    });
}

void MainWindow::AddTableValue(QString name)
{
    int row = ui->FinishQueue->rowCount();

    ui->FinishQueue->insertRow(row);

    ui->FinishQueue->setItem(row,0,new QTableWidgetItem(name));

    //每一层插入到尾部，直接滚动到底部
    ui->FinishQueue->scrollToBottom();

}

bool MainWindow::containsDuplicateNames(QString name)
{
    if(m_existName.isEmpty() || !m_existName.contains(name))
        return false;

    return true;
}



void MainWindow::on_AddProcessButton_clicked()
{
    AddProcessWidget* add = new AddProcessWidget;


    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(add);


    QDialog log(this);
    log.setLayout(mainLayout);

    //注意这里不是accepted和rejected，这两个表示监听关闭后的动作
    connect(add,&AddProcessWidget::accepted,&log,&QDialog::accept);
    connect(add,&AddProcessWidget::rejected,&log,&QDialog::reject);


    connect(add,&AddProcessWidget::check,this,[this](QWidget* w){
        AddProcessWidget* a = qobject_cast<AddProcessWidget*>(w);

        if(containsDuplicateNames(a->getName())){
            a->setNameStyle(QString("#Name{"
                                    "color: #d32f2f;"
                                    "border: 2px solid #ef5350;"
                                    "background: #fff8f8;"
                                    "}"));
            isExist = true;
        }
        else{
            a->setNameStyle(QString("#Name{"
                                    "background: white;"
                                    "border: 2px solid #b0bec5;"
                                    "border-radius: 8px;"
                                    "padding: 8px 12px;"
                                    "font-size: 12px;"
                                    "color: #424242;"
                                    "}"

                                    "#Name:hover{"
                                    "border: 2px solid #90caf9;"
                                    "}"));

            isExist = false;
        }
    });


    if(log.exec() == QDialog::Accepted){
        if(isExist)
        {
            QMessageBox::warning(&log, "警告", "名称已存在，请修改！");
        }
        else{

            QString name = add->getName();
            QString priority = add->getPriority();
            QString ntime = add->getRunTime();

            //插入调度器的链表队列
            int index = ListSorter::insertByPriority(m_scheduler->getList(),
                                                     new Process(name.toStdString(),priority.toInt(),ntime.toInt()));

            //插入到集合
            m_existName.insert(name);

            NodeItem* node = new NodeItem;
            node->setValue(name,priority,ntime);
            QListWidgetItem* item = new QListWidgetItem;
            item->setSizeHint(node->sizeHint());

            ui->ReadyQueue->insertItem(index + 2,item);
            ui->ReadyQueue->setItemWidget(item,node);
            //自动导航到新加入的位置
            ui->ReadyQueue->setCurrentItem(item);

            connect(node,&NodeItem::deleteClicked,this,[this](QWidget* w){

                if (!w) return;

                NodeItem* n = qobject_cast<NodeItem*>(w);


                for(int i = 0;i < ui->ReadyQueue->count();i++){

                    qDebug() << ui->ReadyQueue->count();
                    QListWidgetItem* item = ui->ReadyQueue->item(i);

                    NodeItem* node1 = qobject_cast<NodeItem*>(ui->ReadyQueue->itemWidget(item));

                    if(!node1) continue;

                    if(node1 == n){
                        QListWidgetItem* takenItem = ui->ReadyQueue->takeItem(i);
                        delete takenItem;
                        break;
                    }
                }

                QString name = n->getName();

                List<Process*>::Node* cur = m_scheduler->getList().getHead();
                List<Process*>::Node* preNode = nullptr;

                while(cur && cur->data->getName() != name){
                    preNode = cur;
                    cur = cur->next;
                }

                if(cur){
                    //如果preNode没有赋值说明删除第一个节点
                    if(preNode)
                    {
                        preNode->next = cur->next;
                    }
                    else
                    {
                        m_scheduler->getList().setHead(cur->next);
                    }

                    m_existName.remove(name);
                }


            });

        //删除组件
        add->deleteLater();
        log.close();
        }

    }
    else{
        //删除组件
        add->deleteLater();
        log.close();

    }
}


void MainWindow::on_SetSingleButton_clicked()
{
    Process* p = m_scheduler->stepRun();
    isStop = false;

    //记录是否处于定时器状态
    bool wasAutoRunning = m_timer->isActive();

    if(!wasAutoRunning){
        emit isExecute(true);
        qDebug() << '1';
    }

    if(!p){
        if(m_timer->isActive()){
            m_timer->stop();
            ui->SetAutoButton->setText("自动运行");
            emit isExecute(false);
            return;
        }
    }

    //每次取出第一个数据，直接一起第一项  前面有2个组件索引是2
    QListWidgetItem* item = ui->ReadyQueue->takeItem(2);


    if(!item) return;


    delete item;

    ui->Name->setText(QString("进程名:") + QString::fromStdString(p->getName()));
    ui->Priority->setText(QString("优先级:") + QString::number(p->getPriority()));

    ui->Ntime->setText(QString("总时间:") + QString::number(p->getNTime()));
    ui->Rtime->setText(QString("运行时间:") + QString::number(p->getRTime()));
    ui->Status->setText(QString("状态:运行"));



    //判断是否完成
    if(p->isFinish()){
        if (wasAutoRunning)
            m_timer->stop();

        QTimer::singleShot(1000, this, [this,wasAutoRunning](){
            if(ui->ReadyQueue->currentRow() == 1){
                ui->Name->setText(QString("进程名:") );
                ui->Priority->setText(QString("优先级:") );

                ui->Ntime->setText(QString("总时间:") );
                ui->Rtime->setText(QString("运行时间:") );
                ui->Status->setText(QString("状态:"));
            }

            if (!m_scheduler->getList().empty()) {
                if (wasAutoRunning) m_timer->start();
            }
            else
            {
                isStop = true;
                emit isExecute(false);
                ui->SetAutoButton->setText("自动运行");
            }
        });


        AddTableValue(QString::fromStdString(p->getName()));
    }
    else{
        if (wasAutoRunning)
            m_timer->stop();

        emit isExecute(true);


        QTimer::singleShot(1000, this, [this,p,wasAutoRunning](){
            NodeItem* node = new NodeItem;

            node->setValue( QString::fromStdString(p->getName()),
                            QString::number(p->getPriority()),
                            QString::number(p->getRTime()),
                            QString::number(p->getNTime()));

            QListWidgetItem* item = new QListWidgetItem;
            item->setSizeHint(node->sizeHint());


            ListSorter::insertByPriority(m_scheduler->getList(),
                        new Process(p->getName(),p->getPriority(),p->getRTime(),p->getNTime()));


            int newIndex = 0;
            auto head = m_scheduler->getList().getHead();
            while(head && head->data != p) {
                newIndex++;
                head = head->next;
            }

            ui->ReadyQueue->insertItem(newIndex + 2, item);
            ui->ReadyQueue->setItemWidget(item, node);

            ui->ReadyQueue->setCurrentItem(item);
            ui->ReadyQueue->scrollToItem(item);

            ui->Status->setText(QString("状态:完成"));

            if (!m_scheduler->getList().empty())
            {
             if (wasAutoRunning) m_timer->start();
            }
            else
            {
                isStop = true;
                emit isExecute(false);
                ui->SetAutoButton->setText("自动运行");
            }
        });
    }

    if(!wasAutoRunning){
        emit isExecute(false);
        qDebug() << '1';
    }

    if(m_scheduler->getList().empty()){
        isStop = true;
        ui->SetAutoButton->setEnabled(false);
        ui->SetSingleButton->setEnabled(false);

        emit isExecute(false);
    }


}


void MainWindow::on_SetAutoButton_clicked()
{
    //启动定时器
    if(!m_timer->isActive()){
        if(m_scheduler->getList().empty())  return;

        m_timer->start(1000);

        ui->SetSingleButton->setEnabled(false);
        ui->SetAutoButton->setText("停止运行");


        emit isExecute(true);
    }
    else{
        m_timer->stop();
        ui->SetSingleButton->setEnabled(true);
        ui->SetAutoButton->setText("自动运行");

        emit isExecute(false);
    }
}


void MainWindow::on_RestButton_clicked()
{
    m_existName.clear();

    ui->FinishQueue->setRowCount(0);


    while(ui->ReadyQueue->currentRow() >= 2){
        QListWidgetItem *item = ui->ReadyQueue->currentItem();
        delete item;
    }
}


void MainWindow::on_CloseButton_clicked()
{
    this->close();
}


void MainWindow::on_MiniButton_clicked()
{
    this->showMinimized();
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton){
        isDrag = true;

        dVal = ev->globalPosition() - pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    if(isDrag){
        move((ev->globalPosition() - dVal).toPoint());
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
        isDrag = false;
}

