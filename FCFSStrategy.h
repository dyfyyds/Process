#ifndef FCFSSTRATEGY_H
#define FCFSSTRATEGY_H

#include "SchedulingStrategy.h"
#include "ListSorter.h"

/**
 * @brief 先来先服务（First-Come, First-Served）调度策略
 *
 * FCFS是最简单、最基础的调度算法，遵循"先到先服务"的原则。
 *
 * 算法特点：
 * - 非抢占式：一旦进程获得CPU，会一直运行到完成
 * - FIFO队列：严格按照到达顺序排列
 * - 公平性：每个进程都有机会执行
 * - 简单性：实现最简单，开销最小
 *
 * 优点：
 * 1. 实现简单，逻辑清晰
 * 2. 没有复杂的计算开销
 * 3. 进程不会"饥饿"（每个进程最终都会执行）
 *
 * 缺点：
 * 1. 平均等待时间可能较长
 * 2. 对短作业不公平（长作业会阻塞短作业）
 * 3. 可能出现"护航效应"（长作业后面跟着很多短作业）
 *
 * 应用场景：
 * - 批处理系统
 * - 对响应时间要求不高的场景
 * - 作为其他复杂算法的基础
 *
 * @继承 SchedulingStrategy 基类，实现了所有纯虚函数
 */
class FCFSStrategy : public SchedulingStrategy {
public:
    /**
     * @brief 选择下一个要执行的进程
     *
     * FCFS策略直接从队首取出进程，遵循FIFO原则。
     * 如果队列为空，返回nullptr。
     *
     * @param readyQueue 就绪队列的引用
     * @return 要执行的进程指针，队列为空返回nullptr
     */
    Process* selectNext(List<Process*>& readyQueue) override {
        if(readyQueue.empty()) return nullptr;
        Process* p = readyQueue.front();   // 获取队首进程
        readyQueue.pop_front();            // 从队列中移除
        return p;
    }

    /**
     * @brief 进程执行后的重新插入逻辑
     *
     * 在FCFS中，进程执行完成后不再重新插入就绪队列（运行完毕）。
     * 但是为了保持接口统一，如果进程需要重新插入，插入到队尾。
     *
     * @param readyQueue 就绪队列的引用
     * @param p 要重新插入的进程指针
     * @return 插入位置的索引
     */
    int reinsert(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::pushBack(readyQueue, p);
    }

    /**
     * @brief 新进程插入就绪队列
     *
     * 新到达的进程直接插入到就绪队列的队尾。
     * 这是FCFS策略的核心：按到达顺序排队。
     *
     * @param readyQueue 就绪队列的引用
     * @param p 新到达的进程指针
     * @return 插入位置的索引
     */
    int insertNew(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::pushBack(readyQueue, p);
    }

    /**
     * @brief 获取时间片长度
     *
     * FCFS是非抢占式算法，进程会一直运行到完成。
     * 返回一个很大的值（9999），表示"无限"时间片，
     * 实际上意味着进程不会被抢占。
     *
     * @return 时间片长度（9999表示运行到完成）
     */
    int getTimeSlice() const override {
        return 9999; // 非抢占，运行到完成
    }

    /**
     * @brief 获取算法的显示名称
     *
     * 返回用户友好的算法名称，支持中文显示。
     *
     * @return 算法名称字符串
     */
    QString displayName() const override { return QString::fromUtf8("先来先服务 (FCFS)"); }

    /**
     * @brief 生成当前调度步骤的解释说明
     *
     * 为教学演示提供详细的调度过程解释。
     * 包含当前时钟周期、选择的进程信息、调度原因等。
     *
     * 输出示例：
     * "第5步: 按到达顺序选择进程 P1 (到达时间=0, 剩余时间=10)，先来先服务，运行至完成。"
     * "第3步: CPU空闲，就绪队列为空。"
     *
     * @param selected 被选中的进程指针
     * @param tick 当前时钟周期
     * @return 解释说明字符串
     */
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