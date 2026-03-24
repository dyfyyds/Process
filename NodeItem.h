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

    void setValue(const QString& name, const QString& priority, const QString& runtime);

private:
    Ui::NodeItem *ui;
};

#endif // NODEITEM_H
