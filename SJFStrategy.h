#ifndef SJFSTRATEGY_H
#define SJFSTRATEGY_H

#include "SchedulingStrategy.h"
#include "ListSorter.h"

class SJFStrategy : public SchedulingStrategy {
public:
    Process* selectNext(List<Process*>& readyQueue) override {
        if(readyQueue.empty()) return nullptr;
        Process* p = readyQueue.front();
        readyQueue.pop_front();
        return p;
    }

    int reinsert(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::insertBySJF(readyQueue, p);
    }

    int insertNew(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::insertBySJF(readyQueue, p);
    }

    int getTimeSlice() const override {
        return 9999; // 非抢占，运行到完成
    }

    QString displayName() const override { return QString::fromUtf8("短作业优先 (SJF)"); }

    QString explainStep(Process* selected, int tick) const override {
        if(!selected) return QString::fromUtf8("第%1步: CPU空闲，就绪队列为空。").arg(tick);
        return QString::fromUtf8("第%1步: 选择进程 %2 (需要时间=%3)，"
               "因为它是就绪队列中运行时间最短的进程。")
            .arg(tick)
            .arg(QString::fromStdString(selected->getName()))
            .arg(selected->getNTime());
    }
};

#endif
