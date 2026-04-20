#ifndef ROUNDROBINSTRATEGY_H
#define ROUNDROBINSTRATEGY_H

#include "SchedulingStrategy.h"
#include "ListSorter.h"

/**
 * @brief 时间片轮转（Round Robin）调度策略
 *
 * Round Robin是最常用的交互式系统调度算法，通过时间片轮转保证公平性。
 *
 * 算法特点：
 * - 时间片轮转：每个进程获得固定时间片，用完即止
 * - 公平性：所有进程轮流执行，不会饥饿
 * - 抢占式：时间片用完后强制抢占
 * - 响应性好：每个进程都能及时获得CPU时间
 *
 * 时间片设置：
 * - 默认时间片：2个时间单位
 * - 可配置：通过构造函数或setQuantum()设置
 * - 时间片大小影响：
 *   * 太大：退化为FCFS
 *   * 太小：上下文切换开销大
 *
 * 优点：
 * 1. 公平性好，每个进程都能获得CPU时间
 * 2. 响应时间短，适用于交互式系统
 * 3. 实现简单，逻辑清晰
 * 4. 不会产生进程饥饿
 *
 * 缺点：
 * 1. 上下文切换开销
 * 2. 时间片大小难以确定
 * 3. 平均等待时间可能较长
 * 4. 对I/O密集型进程不够优化
 *
 * 应用场景：
 * - 分时操作系统
 * - 交互式系统
 * - 通用操作系统（如Linux、Windows）
 *
 * @继承 SchedulingStrategy 基类
 */
class RoundRobinStrategy : public SchedulingStrategy {
private:
    int m_quantum;  /// 时间片长度

public:
    /**
     * @brief 构造函数
     *
     * 创建RoundRobin调度策略实例，可以指定时间片长度。
     *
     * @param quantum 时间片长度，默认为2个时间单位
     */
    explicit RoundRobinStrategy(int quantum = 2) : m_quantum(quantum) {}

    /**
     * @brief 设置时间片长度
     *
     * 动态调整时间片大小，可以在运行时修改。
     *
     * @param q 新的时间片长度
     */
    void setQuantum(int q) { m_quantum = q; }

    /**
     * @brief 选择下一个要执行的进程
     *
     * 从队首取出进程执行。就绪队列按FIFO顺序排列，
     * 保证每个进程都能公平地获得CPU时间。
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
     * 时间片用完后，进程重新插入到就绪队列的队尾。
     * 这是RR算法的核心：公平轮转。
     *
     * 处理逻辑：
     * 1. 如果进程执行完毕，不重新插入
     * 2. 如果时间片用完但进程未完成，插入队尾
     * 3. 保证所有进程都能轮流执行
     *
     * @param readyQueue 就绪队列的引用
     * @param p 要重新插入的进程指针
     * @return 插入位置的索引（队尾）
     */
    int reinsert(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::pushBack(readyQueue, p);
    }

    /**
     * @brief 新进程插入就绪队列
     *
     * 新到达的进程直接插入到就绪队列的队尾。
     * 与FCFS类似，但每个进程都有时间片限制。
     *
     * @param readyQueue 就绪队列的引用
     * @param p 新到达的进程指针
     * @return 插入位置的索引（队尾）
     */
    int insertNew(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::pushBack(readyQueue, p);
    }

    /**
     * @brief 获取时间片长度
     *
     * 返回当前配置的时间片长度。
     * 这是RR算法与FCFS/SJF/Priority的主要区别。
     *
     * @return 时间片长度（时间单位）
     */
    int getTimeSlice() const override { return m_quantum; }

    /**
     * @brief 获取算法的显示名称
     *
     * 返回用户友好的算法名称，包含时间片信息。
     *
     * 示例：
     * "时间片轮转 (RR, q=2)"
     * "时间片轮转 (RR, q=4)"
     *
     * @return 算法名称字符串
     */
    QString displayName() const override {
        return QString::fromUtf8("时间片轮转 (RR, q=%1)").arg(m_quantum);
    }

    /**
     * @brief 生成当前调度步骤的解释说明
     *
     * 为教学演示提供详细的调度过程解释。
     * 重点说明时间片分配和轮转机制。
     *
     * 输出示例：
     * "第5步: 轮转调度选择进程 P2 (剩余时间=3)，分配时间片=2个时间单位。"
     * "第1步: CPU空闲，就绪队列为空。"
     *
     * @param selected 被选中的进程指针
     * @param tick 当前时钟周期
     * @return 解释说明字符串
     */
    QString explainStep(Process* selected, int tick) const override {
        if(!selected) return QString::fromUtf8("第%1步: CPU空闲，就绪队列为空。").arg(tick);
        return QString::fromUtf8("第%1步: 轮转调度选择进程 %2 (剩余时间=%3)，"
               "分配时间片=%4个时间单位。")
            .arg(tick)
            .arg(QString::fromStdString(selected->getName()))
            .arg(selected->getRemainingTime())
            .arg(m_quantum);
    }
};

#endif