#ifndef SCHEDULER_H
#define SCHEDULER_H


#include "List.h"
#include "Process.h"
#include <vector>

class Scheduler{
private:
    List<Process*>          m_process;          //进程链表
    int                     m_count;            //统计调度次数

    /**
     * @brief 显示进程链表信息
     *
     */
    void ListInfo(Process* p);

public:
    Scheduler();
    ~Scheduler();

    List<Process*>& getList();

    /**
     * @brief 调度过程
     *
     */
    void run();

    /**
     * @brief 显示链表节点细节
     *
     * @param p 链表节点指针
     */
    void display(Process* p);

    /**
     * @brief  重载输入符
     *
     * @param is 输入流
     * @param s  调度器对象
     * @return std::istream&
     */
    friend std::istream& operator>>(std::istream& is,Scheduler& s);

};

#endif // SCHEDULER_H
