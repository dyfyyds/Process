#ifndef TITLEITEM_H
#define TITLEITEM_H

#include <QWidget>

namespace Ui {
class TitleItem;
}

class TitleItem : public QWidget
{
    Q_OBJECT

public:
    explicit TitleItem(QWidget *parent = nullptr);
    ~TitleItem();

private:
    Ui::TitleItem *ui;
};

#endif // TITLEITEM_H
