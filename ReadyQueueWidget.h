#ifndef READYQUEUEWIDGET_H
#define READYQUEUEWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QLabel>
#include <vector>
#include "ProcessCard.h"
#include "Process.h"
#include "List.h"

class ReadyQueueWidget : public QWidget {
    Q_OBJECT
public:
    explicit ReadyQueueWidget(QWidget* parent = nullptr);

    void addCard(Process* p, int index = -1);
    void removeCard(const QString& name);
    void clear();
    void refreshAll(List<Process*>& readyList);

signals:
    void processDeleted(const QString& name);

private:
    QScrollArea* m_scrollArea;
    QWidget* m_container;
    QHBoxLayout* m_cardsLayout;
    QLabel* m_titleLabel;
    QLabel* m_emptyLabel;
    std::vector<ProcessCard*> m_cards;

    void updateEmptyState();
};

#endif
