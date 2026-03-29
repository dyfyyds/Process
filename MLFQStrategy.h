#ifndef MLFQSTRATEGY_H
#define MLFQSTRATEGY_H

#include "SchedulingStrategy.h"
#include "ListSorter.h"
#include <map>

class MLFQStrategy : public SchedulingStrategy {
private:
    // 3级队列: level 0 (最高优先级, q=1), level 1 (q=2), level 2 (FCFS)
    List<Process*> m_queues[3];
    int m_quantums[3] = {1, 2, 9999};
    std::map<std::string, int> m_processLevel; // 进程名 -> 当前所在队列级别
    int m_currentLevel = 0;

public:
    Process* selectNext(List<Process*>& readyQueue) override {
        // 从最高优先级队列开始查找
        for(int i = 0; i < 3; i++){
            if(!m_queues[i].empty()){
                m_currentLevel = i;
                Process* p = m_queues[i].front();
                m_queues[i].pop_front();
                return p;
            }
        }
        return nullptr;
    }

    int reinsert(List<Process*>& readyQueue, Process* p) override {
        std::string name = p->getName();
        int level = m_processLevel.count(name) ? m_processLevel[name] : 0;
        // 降级(如果不是最低级)
        if(level < 2) level++;
        m_processLevel[name] = level;
        m_queues[level].push_back(p);
        return m_queues[level].size() - 1;
    }

    int insertNew(List<Process*>& readyQueue, Process* p) override {
        // 新进程进入最高优先级队列
        m_processLevel[p->getName()] = 0;
        m_queues[0].push_back(p);
        return m_queues[0].size() - 1;
    }

    int getTimeSlice() const override {
        return m_quantums[m_currentLevel];
    }

    QString displayName() const override {
        return QString::fromUtf8("多级反馈队列 (MLFQ)");
    }

    QString explainStep(Process* selected, int tick) const override {
        if(!selected) return QString::fromUtf8("第%1步: CPU空闲，所有队列为空。").arg(tick);
        int level = m_currentLevel;
        return QString::fromUtf8("第%1步: 从第%2级队列选择进程 %3 (剩余时间=%4)，"
               "时间片=%5。若未完成将降级到第%6级队列。")
            .arg(tick)
            .arg(level + 1)
            .arg(QString::fromStdString(selected->getName()))
            .arg(selected->getRemainingTime())
            .arg(m_quantums[level])
            .arg(std::min(level + 2, 3));
    }
};

#endif
