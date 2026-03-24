#include "NodeItem.h"
#include "ui_NodeItem.h"

NodeItem::NodeItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NodeItem)
{
    ui->setupUi(this);
}

NodeItem::~NodeItem()
{
    delete ui;
}

void NodeItem::setValue(const QString &name, const QString &priority, const QString &runtime)
{
    ui->Name->setText(name);
    ui->Priority->setText(priority);
    ui->Ntime->setText(runtime);

    ui->Status->setText("就绪");
    ui->Rtime->setText(QString('0'));
}
