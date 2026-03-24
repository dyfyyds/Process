#ifndef SCHEDULER_H
#define SCHEDULER_H


#include "List.h"
#include "Process.h"
#include <vector>

class Scheduler{
private:
    List<Process*>          m_process;          //进程链表
    int                     m_count;            //统计调度次数

public:
    Scheduler();
    ~Scheduler();

    List<Process*>& getList();

    /**
     * @brief 单步执行
     */
    Process* stepRun();

};

#endif // SCHEDULER_H
