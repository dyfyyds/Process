#ifndef PRIORITYSTRATEGY_H
#define PRIORITYSTRATEGY_H

#include "SchedulingStrategy.h"
#include "ListSorter.h"

class PriorityStrategy : public SchedulingStrategy {
public:
    Process* selectNext(List<Process*>& readyQueue) override {
        if(readyQueue.empty()) return nullptr;
        Process* p = readyQueue.front();
        readyQueue.pop_front();
        return p;
    }

    int reinsert(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::insertByPriority(readyQueue, p);
    }

    int insertNew(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::insertByPriority(readyQueue, p);
    }

    void onExecuted(Process* p) override {
        p->decreasePriority();
    }

    QString displayName() const override { return QString::fromUtf8("优先级调度"); }

    QString explainStep(Process* selected, int tick) const override {
        if(!selected) return QString::fromUtf8("第%1步: CPU空闲，就绪队列为空。").arg(tick);
        return QString::fromUtf8("第%1步: 选择进程 %2 (优先级=%3, 剩余时间=%4)，"
               "因为它在就绪队列中优先级最高。执行1个时间单位后优先级降为%5。")
            .arg(tick)
            .arg(QString::fromStdString(selected->getName()))
            .arg(selected->getPriority() + 1)
            .arg(selected->getRemainingTime())
            .arg(selected->getPriority());
    }
};

#endif
