#include "ReadyQueueWidget.h"
#include "CyberStyle.h"
#include <QVBoxLayout>

ReadyQueueWidget::ReadyQueueWidget(QWidget* parent) : QWidget(parent) {
    setFixedHeight(150);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(5);

    m_titleLabel = new QLabel(QString::fromUtf8("⏳ 就绪队列"), this);
    m_titleLabel->setStyleSheet(CyberStyle::sectionTitle());
    mainLayout->addWidget(m_titleLabel);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setStyleSheet(QString(
        "QScrollArea { background: transparent; "
        "border: 1px solid %1; border-radius: 6px; }").arg(Cyber::BORDER_LIT));

    m_container = new QWidget;
    m_container->setStyleSheet("background: transparent;");
    m_cardsLayout = new QHBoxLayout(m_container);
    m_cardsLayout->setContentsMargins(10, 6, 10, 6);
    m_cardsLayout->setSpacing(10);
    m_cardsLayout->setAlignment(Qt::AlignLeft);

    m_emptyLabel = new QLabel(QString::fromUtf8("⊘ 队列为空"), m_container);
    m_emptyLabel->setStyleSheet(QString("color: %1; font-size: 13px; background: transparent; border: none;").arg(Cyber::TEXT_DIM));
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_cardsLayout->addWidget(m_emptyLabel);

    m_scrollArea->setWidget(m_container);
    mainLayout->addWidget(m_scrollArea);
}

void ReadyQueueWidget::addCard(Process* p, int index) {
    auto* card = new ProcessCard(p, m_container);
    if(index >= 0 && index < (int)m_cards.size()) {
        m_cardsLayout->insertWidget(index, card);
        m_cards.insert(m_cards.begin() + index, card);
    } else {
        m_cardsLayout->addWidget(card);
        m_cards.push_back(card);
    }
    updateEmptyState();
}

void ReadyQueueWidget::removeCard(const QString& name) {
    for(auto it = m_cards.begin(); it != m_cards.end(); ++it) {
        if((*it)->processName() == name) {
            m_cardsLayout->removeWidget(*it);
            (*it)->deleteLater();
            m_cards.erase(it);
            break;
        }
    }
    updateEmptyState();
}

void ReadyQueueWidget::clear() {
    for(auto* card : m_cards) {
        m_cardsLayout->removeWidget(card);
        card->deleteLater();
    }
    m_cards.clear();
    updateEmptyState();
}

void ReadyQueueWidget::refreshAll(List<Process*>& readyList) {
    clear();
    auto* node = readyList.getHead();
    while(node) {
        addCard(node->data);
        node = node->next;
    }
}

void ReadyQueueWidget::updateEmptyState() {
    m_emptyLabel->setVisible(m_cards.empty());
}
