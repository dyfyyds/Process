#ifndef PRIORITYSTRATEGY_H
#define PRIORITYSTRATEGY_H

#include "SchedulingStrategy.h"
#include "ListSorter.h"

/**
 * @brief 优先级调度（Priority Scheduling）策略
 *
 * 优先级调度算法根据进程的优先级来选择下一个执行的进程。
 * 本实现采用动态优先级机制：进程每执行一个时间单位，优先级降低1。
 *
 * 算法特点：
 * - 基于优先级排序：总是选择优先级最高的进程
 * - 动态调整：执行后优先级降低，防止单一进程垄断CPU
 * - 非抢占式：当前实现中进程执行一个时间单位后重新调度
 * - 结合了老化（Aging）机制，避免低优先级进程饥饿
 *
 * 优先级规则：
 * - 数值越大，优先级越高（假设优先级用整数表示）
 * - 执行一个时间单位后，优先级减1
 * - 当优先级降到最低时，会重新提升（在Process类中实现）
 *
 * 优点：
 * 1. 可以灵活控制进程执行顺序
 * 2. 结合动态调整，避免高优先级进程垄断CPU
 * 3. 实现相对简单
 * 4. 适用于实时系统和交互式系统
 *
 * 缺点：
 * 1. 需要维护优先级排序
 * 2. 优先级设置需要经验
 * 3. 仍然可能存在一定的饥饿问题
 *
 * 应用场景：
 * - 实时系统
 * - 交互式系统
 * - 需要区分任务重要性的场景
 *
 * @继承 SchedulingStrategy 基类
 */
class PriorityStrategy : public SchedulingStrategy {
public:
    /**
     * @brief 选择下一个要执行的进程
     *
     * 从队首取出当前优先级最高的进程。
     * 由于在插入时已经按优先级规则排序，队首就是最高优先级进程。
     *
     * @param readyQueue 就绪队列的引用（已按优先级排序）
     * @return 要执行的进程指针，队列为空返回nullptr
     */
    Process* selectNext(List<Process*>& readyQueue) override {
        if(readyQueue.empty()) return nullptr;
        Process* p = readyQueue.front();   // 获取最高优先级进程（队首）
        readyQueue.pop_front();            // 从队列中移除
        return p;
    }

    /**
     * @brief 进程执行后的重新插入逻辑
     *
     * 进程执行一个时间单位后，根据其当前优先级重新插入到合适位置。
     * 使用insertByPriority保持队列按优先级有序。
     *
     * @param readyQueue 就绪队列的引用
     * @param p 要重新插入的进程指针
     * @return 插入位置的索引
     */
    int reinsert(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::insertByPriority(readyQueue, p);
    }

    /**
     * @brief 新进程插入就绪队列
     *
     * 新进程按优先级插入到就绪队列中的合适位置。
     * 这是优先级调度策略的核心：维护按优先级排序的队列。
     *
     * 插入逻辑：
     * 1. 遍历队列，找到第一个优先级比新进程低的进程
     * 2. 在新进程前插入
     * 3. 如果所有进程都比新进程优先级高，插入队尾
     *
     * @param readyQueue 就绪队列的引用
     * @param p 新到达的进程指针
     * @return 插入位置的索引
     */
    int insertNew(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::insertByPriority(readyQueue, p);
    }

    /**
     * @brief 进程执行完成后的回调函数
     *
     * 当一个进程执行完一个时间单位后调用的钩子函数。
     * 这里实现了动态优先级调整：每次执行后优先级降低1。
     *
     * 这种机制的作用：
     * 1. 防止高优先级进程永远占用CPU
     * 2. 让低优先级进程有机会执行
     * 3. 实现类似"老化"的效果
     *
     * @param p 执行完成的进程指针
     */
    void onExecuted(Process* p) override {
        p->decreasePriority();
    }

    /**
     * @brief 获取算法的显示名称
     *
     * 返回用户友好的算法名称，支持中文显示。
     *
     * @return 算法名称字符串
     */
    QString displayName() const override { return QString::fromUtf8("优先级调度"); }

    /**
     * @brief 生成当前调度步骤的解释说明
     *
     * 为教学演示提供详细的调度过程解释。
     * 重点说明为什么选择这个进程（优先级最高）以及执行后的变化。
     *
     * 输出示例：
     * "第5步: 选择进程 P2 (优先级=3, 剩余时间=5)，因为它在就绪队列中优先级最高。执行1个时间单位后优先级降为2。"
     * "第1步: CPU空闲，就绪队列为空。"
     *
     * @param selected 被选中的进程指针
     * @param tick 当前时钟周期
     * @return 解释说明字符串
     */
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