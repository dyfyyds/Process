#ifndef STATSPANEL_H
#define STATSPANEL_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QTableWidget>
#include "ScheDuler.h"

class StatsPanel : public QWidget {
    Q_OBJECT
public:
    explicit StatsPanel(QWidget* parent = nullptr);
    void updateStats(Scheduler* scheduler);
    void clear();

private:
    void setupUI();

    QLabel* m_algoLabel;
    QLabel* m_tickLabel;
    QLabel* m_waitLabel;
    QLabel* m_turnaroundLabel;
    QLabel* m_responseLabel;
    QLabel* m_cpuUtilLabel;
    QProgressBar* m_cpuBar;
    QLabel* m_readyCountLabel;
    QLabel* m_finishedCountLabel;
    QTableWidget* m_finishedTable;
};

#endif
