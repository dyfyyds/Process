#include "TitleItem.h"
#include "ui_TitleItem.h"

TitleItem::TitleItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TitleItem)
{
    ui->setupUi(this);
}

TitleItem::~TitleItem()
{
    delete ui;
}
