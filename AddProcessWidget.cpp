#include "AddProcessWidget.h"
#include "ui_AddProcessWidget.h"

#include <QIntValidator>

AddProcessWidget::AddProcessWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddProcessWidget)
{
    ui->setupUi(this);

    //限制9999
    QIntValidator *timeValidator = new QIntValidator(1, 9999, this);
    ui->Ntime->setValidator(timeValidator);
    ui->Priority->setValidator(timeValidator);


    //禁用
    ui->ComfireButton->setEnabled(false);
    connect(ui->Name,&QLineEdit::textChanged,this,&AddProcessWidget::checkInput);
    connect(ui->Ntime,&QLineEdit::textChanged,this,&AddProcessWidget::checkInput);
    connect(ui->Priority,&QLineEdit::textChanged,this,&AddProcessWidget::checkInput);

    connect(ui->ComfireButton,&QPushButton::clicked,this,[this](){emit this->accepted();});
    connect(ui->CancleButton,&QPushButton::clicked,this,[this](){emit this->rejected();});
}

AddProcessWidget::~AddProcessWidget()
{
    delete ui;
}

void AddProcessWidget::checkInput()
{
    bool allFilled = !ui->Name->text().isEmpty() &&
                     !ui->Priority->text().isEmpty() &&
                     !ui->Ntime->text().isEmpty();
    ui->ComfireButton->setEnabled(allFilled);
}

QString AddProcessWidget::getName() const
{
    return ui->Name->text();
}

QString AddProcessWidget::getPriority() const
{
    return ui->Priority->text();
}

QString AddProcessWidget::getRunTime() const
{
    return ui->Ntime->text();
}
