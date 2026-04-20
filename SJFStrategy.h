#ifndef SJFSTRATEGY_H
#define SJFSTRATEGY_H

#include "SchedulingStrategy.h"
#include "ListSorter.h"

/**
 * @brief 短作业优先（Shortest Job First）调度策略
 *
 * SJF算法选择就绪队列中运行时间最短的进程优先执行。
 * 这是一种非抢占式的调度算法，通过优化作业执行顺序来减少平均等待时间。
 *
 * 算法特点：
 * - 非抢占式：进程一旦获得CPU，会运行到完成
 * - 按长度排序：总是选择剩余时间最短的进程
 * - 优化目标：最小化平均等待时间
 * - 需要排序：每次插入都需要维护有序队列
 *
 * 优点：
 * 1. 平均等待时间比FCFS更短
 * 2. 系统吞吐量较高
 * 3. 适用于批处理系统
 *
 * 缺点：
 * 1. 需要预知作业执行时间（实际中难以准确预测）
 * 2. 长作业可能"饥饿"（永远得不到执行）
 * 3. 需要额外的排序开销
 *
 * 应用场景：
 * - 批处理系统
 * - 作业执行时间可预测的环境
 * - 对平均响应时间要求较高的场景
 *
 * @继承 SchedulingStrategy 基类
 */
class SJFStrategy : public SchedulingStrategy {
public:
    /**
     * @brief 选择下一个要执行的进程
     *
     * 从队首取出当前剩余时间最短的进程。
     * 由于在插入时已经按SJF规则排序，队首就是最短作业。
     *
     * @param readyQueue 就绪队列的引用（已按剩余时间排序）
     * @return 要执行的进程指针，队列为空返回nullptr
     */
    Process* selectNext(List<Process*>& readyQueue) override {
        if(readyQueue.empty()) return nullptr;
        Process* p = readyQueue.front();   // 获取最短作业（队首）
        readyQueue.pop_front();            // 从队列中移除
        return p;
    }

    /**
     * @brief 进程执行后的重新插入逻辑
     *
     * 进程执行完成后，根据其剩余时间重新插入到合适位置。
     * 使用insertBySJF保持队列按剩余时间有序。
     *
     * @param readyQueue 就绪队列的引用
     * @param p 要重新插入的进程指针
     * @return 插入位置的索引
     */
    int reinsert(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::insertBySJF(readyQueue, p);
    }

    /**
     * @brief 新进程插入就绪队列
     *
     * 新进程按剩余时间插入到就绪队列中的合适位置。
     * 这是SJF策略的核心：维护按剩余时间排序的队列。
     *
     * 插入逻辑：
     * 1. 遍历队列，找到第一个剩余时间比新进程长的进程
     * 2. 在新进程前插入
     * 3. 如果所有进程都比新进程短，插入队尾
     *
     * @param readyQueue 就绪队列的引用
     * @param p 新到达的进程指针
     * @return 插入位置的索引
     */
    int insertNew(List<Process*>& readyQueue, Process* p) override {
        return ListSorter::insertBySJF(readyQueue, p);
    }

    /**
     * @brief 获取时间片长度
     *
     * SJF是非抢占式算法，进程会一直运行到完成。
     * 返回很大的值表示"无限"时间片。
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
    QString displayName() const override { return QString::fromUtf8("短作业优先 (SJF)"); }

    /**
     * @brief 生成当前调度步骤的解释说明
     *
     * 为教学演示提供详细的调度过程解释。
     * 重点说明为什么选择这个进程（剩余时间最短）。
     *
     * 输出示例：
     * "第3步: 选择进程 P2 (需要时间=3)，因为它是就绪队列中运行时间最短的进程。"
     * "第1步: CPU空闲，就绪队列为空。"
     *
     * @param selected 被选中的进程指针
     * @param tick 当前时钟周期
     * @return 解释说明字符串
     */
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