#ifndef SCHEDULINGSTRATEGY_H
#define SCHEDULINGSTRATEGY_H

#include "List.h"
#include "Process.h"
#include <QString>

enum class Algorithm {
    FCFS,
    SJF,
    Priority,
    RoundRobin,
    MLFQ
};

class SchedulingStrategy {
public:
    virtual ~SchedulingStrategy() = default;

    virtual Process* selectNext(List<Process*>& readyQueue) = 0;
    virtual int reinsert(List<Process*>& readyQueue, Process* p) = 0;
    virtual int insertNew(List<Process*>& readyQueue, Process* p) = 0;
    virtual int getTimeSlice() const { return 1; }
    virtual QString displayName() const = 0;
    virtual QString explainStep(Process* selected, int tick) const = 0;
    virtual void onExecuted(Process* p) {}
};

#endif // SCHEDULINGSTRATEGY_H
