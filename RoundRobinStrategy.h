#ifndef ROUNDROBINSTRATEGY_H
#define ROUNDROBINSTRATEGY_H

#include "SchedulingStrategy.h"
#include "ListSorter.h"

class RoundRobinStrategy : public SchedulingStrategy {
private:
    int m_quantum;
public:
    explicit RoundRobinStrategy(int quantum = 2) : m_quantum(quantum) {}

    void setQuantum(int q) { m_quantum = q; }

    Process* selectNext(List<Process*>& readyQueue) override {
        if(readyQueue.empty()) return nullptr;
        Process* p = readyQueue.front();
        readyQueue.pop_front();
        return p;
    }

    int reinsert(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::pushBack(readyQueue, p);
    }

    int insertNew(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::pushBack(readyQueue, p);
    }

    int getTimeSlice() const override { return m_quantum; }

    QString displayName() const override {
        return QString::fromUtf8("时间片轮转 (RR, q=%1)").arg(m_quantum);
    }

    QString explainStep(Process* selected, int tick) const override {
        if(!selected) return QString::fromUtf8("第%1步: CPU空闲，就绪队列为空。").arg(tick);
        return QString::fromUtf8("第%1步: 轮转调度选择进程 %2 (剩余时间=%3)，"
               "分配时间片=%4个时间单位。")
            .arg(tick)
            .arg(QString::fromStdString(selected->getName()))
            .arg(selected->getRemainingTime())
            .arg(m_quantum);
    }
};

#endif
