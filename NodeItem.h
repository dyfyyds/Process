#ifndef NODEITEM_H
#define NODEITEM_H

#include <QWidget>

namespace Ui {
class NodeItem;
}

class NodeItem : public QWidget
{
    Q_OBJECT

public:
    explicit NodeItem(QWidget *parent = nullptr);
    ~NodeItem();

    void setValue(const QString& name, const QString& priority, const QString& ntime);
    void setValue(const QString& name, const QString& priority, const QString& rtime, const QString& ntime);

    QString getName();

signals:
    void deleteClicked(QWidget* w);
    void check(QWidget* w);

private:
    Ui::NodeItem *ui;
};

#endif // NODEITEM_H
