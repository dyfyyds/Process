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
    buildHeaderSection(layout);
    layout->addWidget(createSeparator());
    buildMetricsSection(layout);
    layout->addWidget(createSeparator());
    buildCountSection(layout);
    buildFinishedTableSection(layout);
    clear();
}

void StatsPanel::buildHeaderSection(QVBoxLayout* layout) {
    auto* title = new QLabel(QString::fromUtf8("📊 统计面板"), this);
    title->setStyleSheet(CyberStyle::sectionTitle());
    layout->addWidget(title);

    m_algoLabel = new QLabel(this);
    m_algoLabel->setStyleSheet(QString(
        "color: %1; font-size: 12px; font-weight: bold; "
        "background: transparent; border: none; padding: 4px; border-radius: 4px;")
        .arg(Cyber::MAGENTA));
    layout->addWidget(m_algoLabel);

    m_tickLabel = createValueLabel();
    layout->addWidget(m_tickLabel);
}

void StatsPanel::buildMetricsSection(QVBoxLayout* layout) {
    auto* metricsTitle = new QLabel(QString::fromUtf8("⚡ 性能指标"), this);
    metricsTitle->setStyleSheet(CyberStyle::sectionTitle());
    layout->addWidget(metricsTitle);

    m_waitLabel = createStatLabel();
    m_turnaroundLabel = createStatLabel();
    m_responseLabel = createStatLabel();
    m_cpuUtilLabel = createStatLabel();

    layout->addWidget(m_waitLabel);
    layout->addWidget(m_turnaroundLabel);
    layout->addWidget(m_responseLabel);
    layout->addWidget(m_cpuUtilLabel);

    m_cpuBar = new QProgressBar(this);
    m_cpuBar->setRange(0, 100);
    m_cpuBar->setValue(0);
    m_cpuBar->setFixedHeight(18);
    layout->addWidget(m_cpuBar);
}

void StatsPanel::buildCountSection(QVBoxLayout* layout) {
    m_readyCountLabel = createStatLabel();
    m_finishedCountLabel = createStatLabel();
    layout->addWidget(m_readyCountLabel);
    layout->addWidget(m_finishedCountLabel);
}

void StatsPanel::buildFinishedTableSection(QVBoxLayout* layout) {
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
}

void StatsPanel::updateFinishedTable(const std::vector<Process*>& finished) {
    m_finishedTable->setRowCount(finished.size());
    for(int i = 0; i < static_cast<int>(finished.size()); i++) {
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

QLabel* StatsPanel::createSeparator() {
    auto* separator = new QLabel(this);
    separator->setFixedHeight(1);
    separator->setStyleSheet(QString(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 transparent, stop:0.5 %1, stop:1 transparent)")
        .arg(Cyber::BORDER_LIT));
    return separator;
}

QLabel* StatsPanel::createStatLabel() {
    auto* label = new QLabel(this);
    label->setStyleSheet(QString(
        "color: %1; font-size: 11px; background: transparent; border: none;")
        .arg(Cyber::TEXT_NEON));
    return label;
}

QLabel* StatsPanel::createValueLabel() {
    auto* label = new QLabel(this);
    label->setStyleSheet(QString(
        "color: %1; font-size: 12px; font-weight: bold; background: transparent; border: none;")
        .arg(Cyber::GREEN));
    return label;
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

    const auto& finished = scheduler->getFinishedList();
    updateFinishedTable(finished);
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
