#include "ComparisonDialog.h"
#include "CyberStyle.h"
#include <QHeaderView>
#include <QLabel>

ComparisonDialog::ComparisonDialog(const std::vector<Process*>& processes, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("算法对比"));
    setMinimumSize(700, 400);
    setStyleSheet(QString("QDialog { background-color: %1; }").arg(Cyber::BG_DEEP));

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(12);

    auto* title = new QLabel(QString::fromUtf8("调度算法性能对比"), this);
    title->setStyleSheet(CyberStyle::neonTitle());
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    auto* subtitle = new QLabel(
        QString::fromUtf8("使用相同的 %1 个进程测试所有调度算法").arg(processes.size()), this);
    subtitle->setStyleSheet(QString("color: %1; font-size: 12px; background: transparent;").arg(Cyber::TEXT_DIM));
    subtitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(subtitle);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels({
        QString::fromUtf8("调度算法"),
        QString::fromUtf8("平均等待时间"),
        QString::fromUtf8("平均周转时间"),
        QString::fromUtf8("平均响应时间"),
        QString::fromUtf8("CPU利用率")
    });
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->verticalHeader()->setVisible(false);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionMode(QAbstractItemView::NoSelection);
    layout->addWidget(m_table);

    runComparison(processes);
}

void ComparisonDialog::runComparison(const std::vector<Process*>& processes) {
    struct AlgoInfo {
        Algorithm algo;
        QString name;
        int quantum;
    };

    std::vector<AlgoInfo> algos = {
        {Algorithm::FCFS, QString::fromUtf8("先来先服务 (FCFS)"), 0},
        {Algorithm::SJF, QString::fromUtf8("短作业优先 (SJF)"), 0},
        {Algorithm::Priority, QString::fromUtf8("优先级调度"), 0},
        {Algorithm::RoundRobin, QString::fromUtf8("时间片轮转 (RR, q=2)"), 2},
        {Algorithm::MLFQ, QString::fromUtf8("多级反馈队列 (MLFQ)"), 0},
    };

    m_table->setRowCount(algos.size());

    for(int i = 0; i < (int)algos.size(); i++) {
        Scheduler scheduler;
        scheduler.setStrategy(algos[i].algo);
        if(algos[i].quantum > 0)
            scheduler.setTimeQuantum(algos[i].quantum);

        // 克隆所有进程
        for(auto* p : processes) {
            Process* clone = p->clone();
            scheduler.addProcess(clone);
        }

        // 运行到完成
        int maxSteps = 10000;
        while(!scheduler.getReadyList().empty() && maxSteps-- > 0) {
            scheduler.stepRun();
        }

        // 填表
        m_table->setItem(i, 0, new QTableWidgetItem(algos[i].name));
        m_table->setItem(i, 1, new QTableWidgetItem(
            QString::number(scheduler.avgWaitingTime(), 'f', 2)));
        m_table->setItem(i, 2, new QTableWidgetItem(
            QString::number(scheduler.avgTurnaroundTime(), 'f', 2)));
        m_table->setItem(i, 3, new QTableWidgetItem(
            QString::number(scheduler.avgResponseTime(), 'f', 2)));
        m_table->setItem(i, 4, new QTableWidgetItem(
            QString::number(scheduler.cpuUtilization(), 'f', 1) + "%"));

        // 高亮最优值
        for(int col = 1; col <= 4; col++) {
            auto* item = m_table->item(i, col);
            if(item) item->setForeground(QColor(Cyber::TEXT));
        }
        auto* nameItem = m_table->item(i, 0);
        if(nameItem) nameItem->setForeground(QColor(Cyber::CYAN));
    }

    // 找出各列最优值并高亮
    for(int col = 1; col <= 3; col++) {
        double bestVal = 1e9;
        int bestRow = -1;
        for(int row = 0; row < m_table->rowCount(); row++) {
            double val = m_table->item(row, col)->text().toDouble();
            if(val < bestVal) { bestVal = val; bestRow = row; }
        }
        if(bestRow >= 0)
            m_table->item(bestRow, col)->setForeground(QColor(Cyber::GREEN));
    }
    // CPU利用率最高最优
    {
        double bestVal = -1;
        int bestRow = -1;
        for(int row = 0; row < m_table->rowCount(); row++) {
            double val = m_table->item(row, 4)->text().remove('%').toDouble();
            if(val > bestVal) { bestVal = val; bestRow = row; }
        }
        if(bestRow >= 0)
            m_table->item(bestRow, 4)->setForeground(QColor(Cyber::GREEN));
    }
}
