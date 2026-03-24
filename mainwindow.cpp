#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "AddProcessWidget.h"

#include <QDialog>
#include <QListWidgetItem>
#include "TitleItem.h"
#include "NodeItem.h"
#include "ScheDuler.h"
#include "ListSorter.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->m_scheduler = std::make_shared<Scheduler>();
    GuiInit();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GuiInit()
{
    //完成队列设置成两行
    ui->FinishQueue->setColumnCount(2);

    QStringList headers;
    headers << "进程名称" << "优先级";
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
    if(log.exec() == QDialog::Accepted){
        QString name = add->getName();
        QString priority = add->getPriority();
        QString ntime = add->getRunTime();

        NodeItem* node = new NodeItem;
        node->setValue(name,priority,ntime);
        QListWidgetItem* item = new QListWidgetItem(ui->ReadyQueue);
        item->setSizeHint(node->sizeHint());

        ui->ReadyQueue->addItem(item);
        ui->ReadyQueue->setItemWidget(item,node);

        //插入调度器的链表队列
        ListSorter::insertByPriority(m_scheduler->getList(),
                                      new Process(name.toStdString(),priority.toInt(),ntime.toInt()));

        //删除组件
        add->deleteLater();
        log.close();
    }
    else{
        //删除组件
        add->deleteLater();
        log.close();
    }


}

