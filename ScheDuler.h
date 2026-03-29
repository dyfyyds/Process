#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "List.h"
#include "Process.h"
#include "SchedulingStrategy.h"
#include <vector>
#include <memory>
#include <QString>
#include <QColor>

struct StepResult {
    Process* process = nullptr;
    bool justFinished = false;
    int reinsertIndex = -1;
    QString explanation;
};

struct GanttEntry {
    QString name;
    QColor color;
    int startTick;
    int endTick;
};

class Scheduler {
private:
    std::unique_ptr<SchedulingStrategy> m_strategy;
    List<Process*>                      m_readyQueue;
    std::vector<Process*>               m_finishedList;
    std::vector<Process*>               m_allProcesses; // 所有进程备份（用于统计）
    std::vector<GanttEntry>             m_ganttData;
    int                                 m_tick = 0;
    int                                 m_busyTicks = 0;
    int                                 m_remainingSlice = 0;
    Process*                            m_currentProcess = nullptr;
    Algorithm                           m_currentAlgo = Algorithm::Priority;
    int                                 m_timeQuantum = 2;

public:
    Scheduler();
    ~Scheduler();

    void setStrategy(Algorithm algo);
    void setTimeQuantum(int q);
    Algorithm currentAlgorithm() const;

    int addProcess(Process* p);
    StepResult stepRun();
    void reset();

    List<Process*>& getReadyList();
    const std::vector<Process*>& getFinishedList() const;
    const std::vector<Process*>& getAllProcesses() const;
    int getTick() const;
    Process* getCurrentProcess() const;

    // 统计
    double avgWaitingTime() const;
    double avgTurnaroundTime() const;
    double avgResponseTime() const;
    double cpuUtilization() const;

    // 甘特图
    const std::vector<GanttEntry>& getGanttData() const;

    // 策略名称
    QString strategyName() const;
};

#endif // SCHEDULER_H
