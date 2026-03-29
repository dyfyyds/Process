#include "StatsPanel.h"
#include "CyberStyle.h"
#include <QVBoxLayout>
#include <QHeaderView>

StatsPanel::StatsPanel(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void StatsPanel::setupUI() {
    setFixedWidth(260);
    setStyleSheet(CyberStyle::panelStyle());

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(7);

    auto* title = new QLabel(QString::fromUtf8("📊 统计面板"), this);
    title->setStyleSheet(CyberStyle::sectionTitle());
    layout->addWidget(title);

    QString statStyle = QString("color: %1; font-size: 11px; background: transparent; border: none;").arg(Cyber::TEXT_NEON);
    QString valStyle = QString("color: %1; font-size: 12px; font-weight: bold; background: transparent; border: none;").arg(Cyber::GREEN);

    m_algoLabel = new QLabel(this);
    m_algoLabel->setStyleSheet(QString("color: %1; font-size: 12px; font-weight: bold; "
        "background: transparent; border: none; padding: 4px; border-radius: 4px;")
        .arg(Cyber::MAGENTA));
    layout->addWidget(m_algoLabel);

    m_tickLabel = new QLabel(this);
    m_tickLabel->setStyleSheet(valStyle);
    layout->addWidget(m_tickLabel);

    // 分隔
    auto* sep = new QLabel(this);
    sep->setFixedHeight(1);
    sep->setStyleSheet(QString("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 transparent, stop:0.5 %1, stop:1 transparent)").arg(Cyber::BORDER_LIT));
    layout->addWidget(sep);

    auto* metricsTitle = new QLabel(QString::fromUtf8("⚡ 性能指标"), this);
    metricsTitle->setStyleSheet(CyberStyle::sectionTitle());
    layout->addWidget(metricsTitle);

    m_waitLabel = new QLabel(this);
    m_waitLabel->setStyleSheet(statStyle);
    layout->addWidget(m_waitLabel);

    m_turnaroundLabel = new QLabel(this);
    m_turnaroundLabel->setStyleSheet(statStyle);
    layout->addWidget(m_turnaroundLabel);

    m_responseLabel = new QLabel(this);
    m_responseLabel->setStyleSheet(statStyle);
    layout->addWidget(m_responseLabel);

    m_cpuUtilLabel = new QLabel(this);
    m_cpuUtilLabel->setStyleSheet(statStyle);
    layout->addWidget(m_cpuUtilLabel);

    m_cpuBar = new QProgressBar(this);
    m_cpuBar->setRange(0, 100);
    m_cpuBar->setValue(0);
    m_cpuBar->setFixedHeight(18);
    layout->addWidget(m_cpuBar);

    // 分隔
    auto* sep2 = new QLabel(this);
    sep2->setFixedHeight(1);
    sep2->setStyleSheet(QString("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 transparent, stop:0.5 %1, stop:1 transparent)").arg(Cyber::BORDER_LIT));
    layout->addWidget(sep2);

    m_readyCountLabel = new QLabel(this);
    m_readyCountLabel->setStyleSheet(statStyle);
    layout->addWidget(m_readyCountLabel);

    m_finishedCountLabel = new QLabel(this);
    m_finishedCountLabel->setStyleSheet(statStyle);
    layout->addWidget(m_finishedCountLabel);

    // 完成进程表
    auto* tableTitle = new QLabel(QString::fromUtf8("✅ 已完成进程"), this);
    tableTitle->setStyleSheet(CyberStyle::sectionTitle());
    layout->addWidget(tableTitle);

    m_finishedTable = new QTableWidget(this);
    m_finishedTable->setColumnCount(4);
    m_finishedTable->setHorizontalHeaderLabels({
        QString::fromUtf8("名称"),
        QString::fromUtf8("周转"),
        QString::fromUtf8("等待"),
        QString::fromUtf8("响应")
    });
    m_finishedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_finishedTable->verticalHeader()->setVisible(false);
    m_finishedTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_finishedTable->setSelectionMode(QAbstractItemView::NoSelection);
    layout->addWidget(m_finishedTable, 1);

    clear();
}

void StatsPanel::updateStats(Scheduler* scheduler) {
    if(!scheduler) return;

    m_algoLabel->setText(scheduler->strategyName());
    m_tickLabel->setText(QString::fromUtf8("调度步数: %1").arg(scheduler->getTick()));

    m_waitLabel->setText(QString::fromUtf8("平均等待: %1").arg(scheduler->avgWaitingTime(), 0, 'f', 1));
    m_turnaroundLabel->setText(QString::fromUtf8("平均周转: %1").arg(scheduler->avgTurnaroundTime(), 0, 'f', 1));
    m_responseLabel->setText(QString::fromUtf8("平均响应: %1").arg(scheduler->avgResponseTime(), 0, 'f', 1));

    double cpuUtil = scheduler->cpuUtilization();
    m_cpuUtilLabel->setText(QString::fromUtf8("CPU利用率: %1%").arg(cpuUtil, 0, 'f', 1));
    m_cpuBar->setValue(static_cast<int>(cpuUtil));

    m_readyCountLabel->setText(QString::fromUtf8("就绪队列: %1 个进程").arg(scheduler->getReadyList().size()));
    m_finishedCountLabel->setText(QString::fromUtf8("已完成: %1 个进程").arg(scheduler->getFinishedList().size()));

    // 更新完成表
    const auto& finished = scheduler->getFinishedList();
    m_finishedTable->setRowCount(finished.size());
    for(int i = 0; i < (int)finished.size(); i++) {
        auto* proc = finished[i];
        auto* nameItem = new QTableWidgetItem(QString::fromStdString(proc->getName()));
        nameItem->setForeground(proc->getColor());
        m_finishedTable->setItem(i, 0, nameItem);
        m_finishedTable->setItem(i, 1, new QTableWidgetItem(QString::number(proc->getTurnaroundTime())));
        m_finishedTable->setItem(i, 2, new QTableWidgetItem(QString::number(proc->getWaitTime())));
        m_finishedTable->setItem(i, 3, new QTableWidgetItem(QString::number(proc->getResponseTime())));
    }
    m_finishedTable->scrollToBottom();
}

void StatsPanel::clear() {
    m_algoLabel->setText(QString::fromUtf8("优先级调度"));
    m_tickLabel->setText(QString::fromUtf8("调度步数: 0"));
    m_waitLabel->setText(QString::fromUtf8("平均等待: --"));
    m_turnaroundLabel->setText(QString::fromUtf8("平均周转: --"));
    m_responseLabel->setText(QString::fromUtf8("平均响应: --"));
    m_cpuUtilLabel->setText(QString::fromUtf8("CPU利用率: 0%"));
    m_cpuBar->setValue(0);
    m_readyCountLabel->setText(QString::fromUtf8("就绪队列: 0 个进程"));
    m_finishedCountLabel->setText(QString::fromUtf8("已完成: 0 个进程"));
    m_finishedTable->setRowCount(0);
}
