#ifndef SCHEDULINGSTRATEGY_H
#define SCHEDULINGSTRATEGY_H

#include "List.h"
#include "Process.h"
#include <QString>

/**
 * @brief 进程调度算法类型枚举
 *
 * 定义了系统中支持的所有调度算法类型，用于算法选择和管理
 */
enum class Algorithm {
    FCFS,        // 先来先服务 (First-Come, First-Served) - 最简单的FIFO调度
    SJF,         // 短作业优先 (Shortest Job First) - 选择剩余时间最短的进程
    Priority,    // 优先级调度 (Priority Scheduling) - 基于优先级数值的调度
    RoundRobin,  // 时间片轮转 (Round Robin) - 固定时间片轮转调度
    MLFQ         // 多级反馈队列 (Multi-Level Feedback Queue) - 最复杂的动态优先级调度
};

/**
 * @brief 进程调度策略抽象基类
 *
 * 定义了所有调度算法必须实现的接口，采用策略设计模式。
 * 通过多态实现不同调度算法的动态切换，每个算法负责决定：
 * 1. 哪个进程应该获得CPU时间
 * 2. 进程执行后如何重新插入队列
 * 3. 新进程如何插入就绪队列
 *
 * 关键设计思想：
 * - 策略模式：将算法封装成独立的类，可以互换使用
 * - 多态机制：运行时动态决定使用哪个具体算法
 * - 开闭原则：新增算法不影响现有代码
 */
class SchedulingStrategy {
public:
    /**
     * @brief 虚析构函数
     *
     * 确保派生类对象通过基类指针删除时能正确调用派生类的析构函数，
     * 避免内存泄漏。
     */
    virtual ~SchedulingStrategy() = default;

    /**
     * @brief 核心调度函数 - 选择下一个要执行的进程
     *
     * 从就绪队列中选择下一个应该获得CPU的进程。
     * 这是所有调度算法最核心的方法，决定了算法的具体行为。
     *
     * @param readyQueue 就绪队列的引用，包含所有等待CPU的进程
     * @return 返回选中的进程指针，如果队列为空返回nullptr
     */
    virtual Process* selectNext(List<Process*>& readyQueue) = 0;

    /**
     * @brief 进程执行后的重新插入逻辑
     *
     * 当一个进程执行完一个时间片（或执行完毕）后，
     * 如果需要重新插入就绪队列，这个方法决定插入位置。
     *
     * 用途示例：
     * - RoundRobin：时间片用完，重新插入队尾
     * - SJF：执行完成后不重新插入（进入完成状态）
     * - MLFQ：根据执行情况调整优先级队列
     *
     * @param readyQueue 就绪队列的引用
     * @param p 要重新插入的进程指针
     * @return 插入位置的索引（可用于统计和调试）
     */
    virtual int reinsert(List<Process*>& readyQueue, Process* p) = 0;

    /**
     * @brief 新进程插入就绪队列
     *
     * 当新进程到达时，决定如何插入就绪队列。
     * 不同算法有不同的插入策略：
     * - FCFS：插入队尾
     * - SJF：按剩余时间排序插入
     * - Priority：按优先级排序插入
     *
     * @param readyQueue 就绪队列的引用
     * @param p 新到达的进程指针
     * @return 插入位置的索引
     */
    virtual int insertNew(List<Process*>& readyQueue, Process* p) = 0;

    /**
     * @brief 获取时间片长度
     *
     * 返回当前调度算法使用的时间片长度。
     * 只有需要时间片的算法（如RoundRobin）才会重写这个方法。
     *
     * @return 时间片长度（单位：时钟周期），默认返回1
     */
    virtual int getTimeSlice() const { return 1; }

    /**
     * @brief 获取算法的显示名称
     *
     * 返回当前调度算法的用户友好名称，用于UI显示。
     * 每个具体的算法类都必须实现这个方法。
     *
     * @return 算法名称字符串
     */
    virtual QString displayName() const = 0;

    /**
     * @brief 生成当前调度步骤的解释说明
     *
     * 为教学演示功能提供详细的调度过程解释。
     * 生成人类可读的说明文本，解释为什么选择这个进程。
     *
     * 示例输出：
     * - "选择进程P1（剩余时间最短：3ms）"
     * - "进程P2时间片用完，重新插入队尾"
     *
     * @param selected 被选中的进程指针
     * @param tick 当前时钟周期
     * @return 解释说明字符串
     */
    virtual QString explainStep(Process* selected, int tick) const = 0;

    /**
     * @brief 进程执行完成后的回调函数
     *
     * 当一个进程执行完毕后调用的钩子函数。
     * 派生类可以重写这个方法来执行额外的清理或统计工作。
     *
     * 默认实现为空，表示不需要额外的处理。
     *
     * @param p 执行完成的进程指针
     */
    virtual void onExecuted(Process* p) {}
};

#endif // SCHEDULINGSTRATEGY_H