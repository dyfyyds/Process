#include "NodeItem.h"
#include "ui_NodeItem.h"

NodeItem::NodeItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NodeItem)
{
    ui->setupUi(this);


    //删除信号 把自己的信息带出去
    connect(ui->IconpushButton,&QPushButton::clicked,this,[this](){
        emit deleteClicked(this);
    });


}

NodeItem::~NodeItem()
{
    delete ui;
}

void NodeItem::setValue(const QString &name, const QString &priority, const QString &ntime)
{
    ui->Name->setText(name);
    ui->Priority->setText(priority);
    ui->Ntime->setText(ntime);

    ui->Status->setText("就绪");
    ui->Rtime->setText(QString('0'));
}

void NodeItem::setValue(const QString &name, const QString &priority, const QString &rtime, const QString &ntime)
{
    ui->Name->setText(name);
    ui->Priority->setText(priority);
    ui->Ntime->setText(ntime);

    ui->Status->setText("就绪");
    ui->Rtime->setText(rtime);
}

QString NodeItem::getName()
{
    return ui->Name->text();
}
