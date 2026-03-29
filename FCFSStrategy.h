#ifndef FCFSSTRATEGY_H
#define FCFSSTRATEGY_H

#include "SchedulingStrategy.h"
#include "ListSorter.h"

class FCFSStrategy : public SchedulingStrategy {
public:
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

    int getTimeSlice() const override {
        return 9999; // 非抢占，运行到完成
    }

    QString displayName() const override { return QString::fromUtf8("先来先服务 (FCFS)"); }

    QString explainStep(Process* selected, int tick) const override {
        if(!selected) return QString::fromUtf8("第%1步: CPU空闲，就绪队列为空。").arg(tick);
        return QString::fromUtf8("第%1步: 按到达顺序选择进程 %2 (到达时间=%3, 剩余时间=%4)，"
               "先来先服务，运行至完成。")
            .arg(tick)
            .arg(QString::fromStdString(selected->getName()))
            .arg(selected->getArrivalTime())
            .arg(selected->getRemainingTime());
    }
};

#endif
