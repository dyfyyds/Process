#ifndef ADDPROCESSWIDGET_H
#define ADDPROCESSWIDGET_H

#include <QWidget>

namespace Ui {
class AddProcessWidget;
}

class AddProcessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddProcessWidget(QWidget *parent = nullptr);
    ~AddProcessWidget();

protected slots:
    void checkInput();

signals:
    //自定义的确定和取消信号
    void accepted();
    void rejected();


public:
    QString getName() const;
    QString getPriority() const;
    QString getRunTime() const;

private:
    Ui::AddProcessWidget *ui;
};

#endif // ADDPROCESSWIDGET_H
