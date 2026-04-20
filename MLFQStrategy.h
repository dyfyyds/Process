#ifndef MLFQSTRATEGY_H
#define MLFQSTRATEGY_H

#include "SchedulingStrategy.h"
#include "ListSorter.h"
#include <map>

/**
 * @brief 多级反馈队列（Multi-Level Feedback Queue）调度策略
 *
 * MLFQ是最复杂也是最实用的调度算法，结合了多种调度策略的优点。
 * 通过多个不同优先级的队列，动态调整进程的优先级，达到最优性能。
 *
 * 算法设计思想：
 * 1. 多级队列：3个不同优先级的就绪队列
 * 2. 动态反馈：根据进程行为调整其所在队列级别
 * 3. 不同策略：不同级别使用不同的调度策略
 * 4. 自适应：自动识别I/O密集型和CPU密集型进程
 *
 * 队列结构：
 * - Level 0（最高优先级）：q=1，RR调度，用于交互式进程
 * - Level 1（中等优先级）：q=2，RR调度，用于后台进程
 * - Level 2（最低优先级）：q=9999，FCFS调度，用于批处理进程
 *
 * 调度规则：
 * 1. 总是优先执行高优先级队列中的进程
 * 2. 同一队列内按RR或FCFS规则调度
 * 3. 进程用完时间片后降级到下一级队列
 * 4. 新进程进入最高优先级队列
 * 5. I/O密集型进程会保持在较高优先级
 *
 * 优点：
 * 1. 结合了多种调度策略的优点
 * 2. 自适应，能识别不同类型的进程
 * 3. 响应性好，适合交互式系统
 * 4. 公平性好，不会产生饥饿
 *
 * 缺点：
 * 1. 实现复杂
 * 2. 需要维护多个队列
 * 3. 参数调优复杂
 *
 * 应用场景：
 * - 现代操作系统内核（Linux、Windows）
 * - 多用户分时系统
 * - 需要兼顾响应性和吞吐量的系统
 *
 * @inherit SchedulingStrategy 基类
 */
class MLFQStrategy : public SchedulingStrategy {
private:
    // 3级队列: level 0 (最高优先级, q=1), level 1 (q=2), level 2 (FCFS)
    List<Process*> m_queues[3];
    // 各级队列的时间片
    int m_quantums[3] = {1, 2, 9999};
    // 记录进程当前所在队列级别（进程名 -> 队列级别）
    std::map<std::string, int> m_processLevel;
    // 当前执行的进程所在队列级别
    int m_currentLevel = 0;

public:
    /**
     * @brief 选择下一个要执行的进程
     *
     * 从最高优先级队列开始查找，找到第一个非空队列并取出队首进程。
     * 这保证了高优先级队列的进程总是优先执行。
     *
     * 调度顺序：
     * 1. 检查Level 0队列（最高优先级）
     * 2. 检查Level 1队列
     * 3. 检查Level 2队列（最低优先级）
     *
     * @param readyQueue 就绪队列的引用（MLFQ使用内部队列，此参数不使用）
     * @return 要执行的进程指针，所有队列为空返回nullptr
     */
    Process* selectNext(List<Process*>& readyQueue) override {
        // 从最高优先级队列开始查找
        for(int i = 0; i < 3; i++){
            if(!m_queues[i].empty()){
                m_currentLevel = i;  // 记录当前执行的进程所在级别
                Process* p = m_queues[i].front();
                m_queues[i].pop_front();
                return p;
            }
        }
        return nullptr;
    }

    /**
     * @brief 进程执行后的重新插入逻辑
     *
     * 这是MLFQ的核心机制：根据进程执行情况动态调整其优先级。
     *
     * 处理逻辑：
     * 1. 获取进程当前所在队列级别
     * 2. 如果进程用完时间片且未完成，降级到下一级队列
     * 3. 如果进程主动放弃CPU（如I/O操作），保持在当前级别
     * 4. 新进程进入最高优先级队列（由insertNew处理）
     *
     * 降级规则：
     * - Level 0 -> Level 1
     * - Level 1 -> Level 2
     * - Level 2 -> Level 2（不再降级）
     *
     * @param readyQueue 就绪队列的引用（MLFQ使用内部队列，此参数不使用）
     * @param p 要重新插入的进程指针
     * @return 插入位置的索引
     */
    int reinsert(List<Process*>& readyQueue, Process* p) override {
        std::string name = p->getName();
        int level = m_processLevel.count(name) ? m_processLevel[name] : 0;

        // 降级逻辑：如果不是最低级，则降级
        if(level < 2) {
            level++;  // 降级到下一级队列
        }

        m_processLevel[name] = level;
        m_queues[level].push_back(p);
        return m_queues[level].size() - 1;
    }

    /**
     * @brief 新进程插入就绪队列
     *
     * 新到达的进程进入最高优先级队列（Level 0）。
     * 这是MLFQ的一个重要特性：新进程默认给予最高优先级。
     *
     * 处理逻辑：
     * 1. 在进程映射中记录该进程在Level 0
     * 2. 将进程插入到Level 0队列的队尾
     *
     * @param readyQueue 就绪队列的引用（MLFQ使用内部队列，此参数不使用）
     * @param p 新到达的进程指针
     * @return 插入位置的索引（Level 0队列的队尾）
     */
    int insertNew(List<Process*>& readyQueue, Process* p) override {
        // 新进程进入最高优先级队列
        m_processLevel[p->getName()] = 0;
        m_queues[0].push_back(p);
        return m_queues[0].size() - 1;
    }

    /**
     * @brief 获取时间片长度
     *
     * 返回当前进程所在队列级别的时间片长度。
     * 不同级别的队列有不同的时间片：
     * - Level 0: q=1（快速响应）
     * - Level 1: q=2（平衡）
     * - Level 2: q=9999（批处理，相当于FCFS）
     *
     * @return 当前队列级别的时间片长度
     */
    int getTimeSlice() const override {
        return m_quantums[m_currentLevel];
    }

    /**
     * @brief 获取算法的显示名称
     *
     * 返回用户友好的算法名称。
     *
     * @return 算法名称字符串
     */
    QString displayName() const override {
        return QString::fromUtf8("多级反馈队列 (MLFQ)");
    }

    /**
     * @brief 生成当前调度步骤的解释说明
     *
     * 为教学演示提供详细的MLFQ调度过程解释。
     * 重点说明队列级别、时间片和降级机制。
     *
     * 输出示例：
     * "第5步: 从第1级队列选择进程 P2 (剩余时间=3)，时间片=1。若未完成将降级到第2级队列。"
     * "第1步: CPU空闲，所有队列为空。"
     *
     * @param selected 被选中的进程指针
     * @param tick 当前时钟周期
     * @return 解释说明字符串
     */
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