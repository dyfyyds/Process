#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "List.h"
#include "Process.h"
#include "SchedulingStrategy.h"
#include <vector>
#include <memory>
#include <QString>
#include <QColor>

/**
 * @file ScheDuler.h
 * @brief 进程调度器头文件
 *
 * 定义了调度器核心类 Scheduler，实现了进程调度的主要逻辑。
 * 调度器负责管理就绪队列、执行调度算法、维护统计信息和生成甘特图数据。
 *
 * @author dyf
 * @version 1.0
 * @date 2024年3月
 */

/**
 * @struct StepResult
 * @brief 单步调度结果结构体
 *
 * 记录每次调用 stepRun() 后的调度结果，包含进程执行状态和解释信息。
 * 用于UI更新和教学演示。
 *
 * 字段说明：
 * - process: 当前正在运行的进程指针
 * - justFinished: 进程是否刚刚完成执行
 * - reinsertIndex: 进程重新插入就绪队列的位置（-1表示不需要重新插入）
 * - explanation: 调度过程的详细解释（用于教学演示）
 */
struct StepResult {
    Process* process = nullptr;      // 当前运行的进程，nullptr表示没有进程在运行
    bool justFinished = false;       // 是否刚刚完成一个进程的执行
    int reinsertIndex = -1;          // 重新插入队列的索引，-1表示不重新插入
    QString explanation;             // 调度步骤的解释说明
};

/**
 * @struct GanttEntry
 * @brief 甘特图数据条目
 *
 * 记录进程在甘特图中的显示信息，包括进程名称、颜色和执行时间区间。
 * 用于绘制调度过程的甘特图可视化。
 *
 * 时间表示：
 * - startTick: 进程开始执行的时钟周期
 * - endTick: 进程结束执行的时钟周期（包含）
 *
 * 示例：进程P1在tick 3-5执行，则startTick=3, endTick=5
 */
struct GanttEntry {
    QString name;    // 进程名称（用于显示）
    QColor color;    // 进程颜色（用于区分不同进程）
    int startTick;   // 开始执行时间
    int endTick;     // 结束执行时间
};

/**
 * @class Scheduler
 * @brief 进程调度器主类
 *
 * Scheduler 类是进程调度系统的核心组件，负责：
 * - 管理就绪队列和已完成进程列表
 * - 执行具体的调度算法（通过策略模式）
 * - 维护调度统计信息（等待时间、周转时间等）
 * - 生成甘特图数据用于可视化
 * - 控制调度过程的单步执行
 *
 * ## 设计模式
 * - 策略模式：通过 SchedulingStrategy 接口支持多种调度算法的动态切换
 * - 观察者模式：通过 StepResult 通知UI更新
 *
 * ## 使用示例
 * @code
 * Scheduler scheduler;
 *
 * // 设置调度算法
 * scheduler.setStrategy(Algorithm::RoundRobin);
 * scheduler.setTimeQuantum(3);
 *
 * // 添加进程
 * Process* p1 = new Process("P1", 5, 10);
 * scheduler.addProcess(p1);
 *
 * // 单步执行
 * while(true) {
 *     StepResult result = scheduler.stepRun();
 *     if(result.justFinished) {
 *         // 更新UI
 *     }
 *     if(scheduler.getReadyList().isEmpty()) break;
 * }
 *
 * // 获取统计信息
 * double avgWait = scheduler.avgWaitingTime();
 * @endcode
 *
 * @see SchedulingStrategy
 * @see Process
 * @see StepResult
 * @see GanttEntry
 */
class Scheduler {
private:
    /**
     * @var std::unique_ptr<SchedulingStrategy> m_strategy
     * @brief 当前使用的调度策略
     *
     * 通过智能指针管理调度策略对象，确保内存安全。
     * 支持动态切换不同的调度算法（FCFS、SJF、Priority等）。
     *
     * @see SchedulingStrategy
     * @see setStrategy()
     */
    std::unique_ptr<SchedulingStrategy> m_strategy;

    /**
     * @var List<Process*> m_readyQueue
     * @brief 就绪队列
     *
     * 存储所有等待CPU执行的进程，采用链表结构实现。
     * 队列的排序方式由具体的调度策略决定。
     *
     * @note List是自定义的链表类，提供基本的链表操作
     * @see List
     * @see Process
     */
    List<Process*>                      m_readyQueue;

    /**
     * @var std::vector<Process*> m_finishedList
     * @brief 已完成进程列表
     *
     * 存储所有已经执行完成的进程，用于统计和结果显示。
     * 在reset()时需要手动释放这些进程的内存。
     *
     * @see reset()
     * @see getFinishedList()
     */
    std::vector<Process*>               m_finishedList;

    /**
     * @var std::vector<Process*> m_allProcesses
     * @brief 所有进程的备份
     *
     * 存储所有添加到调度器的进程（包括就绪和已完成）。
     * 主要用于统计计算，防止进程被重复添加。
     *
     * @see addProcess()
     * @see getAllProcesses()
     */
    std::vector<Process*>               m_allProcesses; // 所有进程备份（用于统计）

    /**
     * @var std::vector<GanttEntry> m_ganttData
     * @brief 甘特图数据
     *
     * 记录每个进程的执行时间区间，用于绘制甘特图。
     * 每次执行stepRun()时更新。
     *
     * @see stepRun()
     * @see getGanttData()
     */
    std::vector<GanttEntry>             m_ganttData;

    /**
     * @var int m_tick
     * @brief 当前时钟周期
     *
     * 调度器的全局时间计数器，从0开始递增。
     * 每个stepRun()调用通常会使tick增加1。
     *
     * @see stepRun()
     * @see getTick()
     */
    int                                 m_tick = 0;

    /**
     * @var int m_busyTicks
     * @brief CPU忙碌时间
     *
     * 记录CPU实际执行进程的时钟周期数，用于计算CPU利用率。
     * 当没有进程运行时，tick增加但busyTicks不增加。
     *
     * @see stepRun()
     * @see cpuUtilization()
     */
    int                                 m_busyTicks = 0;

    /**
     * @var int m_remainingSlice
     * @brief 剩余时间片
     *
     * 当前进程还可以执行的时间片数量。
     * 主要用于RoundRobin等时间片轮转调度算法。
     *
     * @see stepRun()
     * @see setTimeQuantum()
     */
    int                                 m_remainingSlice = 0;

    /**
     * @var Process* m_currentProcess
     * @brief 当前正在执行的进程
     *
     * 指向当前占用CPU的进程。
     * 如果没有进程在运行，则为nullptr。
     *
     * @see stepRun()
     * @see getCurrentProcess()
     */
    Process*                            m_currentProcess = nullptr;

    /**
     * @var Algorithm m_currentAlgo
     * @brief 当前使用的调度算法
     *
     * 记录当前的调度算法类型，用于时间片设置等操作。
     *
     * @see setStrategy()
     * @see currentAlgorithm()
     */
    Algorithm                           m_currentAlgo = Algorithm::Priority;

    /**
     * @var int m_timeQuantum
     * @brief 时间片长度
     *
     * RoundRobin调度算法使用的时间片大小。
     * 默认值为2个时钟周期。
     *
     * @see setTimeQuantum()
     * @see RoundRobinStrategy
     */
    int                                 m_timeQuantum = 2;
    Process* selectProcessForStep();
    StepResult buildIdleStepResult();
    void incrementReadyWaitTimes();
    void executeSelectedProcess(Process* p, StepResult& result);
    void updateGanttData(Process* p);
    void finalizeStepResult(Process* p, StepResult& result);

public:
    /**
     * @brief 构造函数
     *
     * 创建调度器实例，初始化默认调度策略（优先级调度）。
     *
     * @note 默认使用PriorityStrategy作为初始调度算法
     * @see PriorityStrategy
     */
    Scheduler();

    /**
     * @brief 析构函数
     *
     * 清理所有分配的内存，包括：
     * - 就绪队列中的进程
     * - 已完成列表中的进程
     *
     * @note 确保不会发生内存泄漏
     */
    ~Scheduler();

    /**
     * @brief 设置调度算法
     *
     * 动态切换调度算法，支持运行时算法变更。
     *
     * @param algo 要使用的调度算法类型
     *
     * @note 会创建对应算法的新策略对象
     * @note 对于RoundRobin算法，会使用当前的时间片设置
     * @see Algorithm
     * @see SchedulingStrategy
     */
    void setStrategy(Algorithm algo);

    /**
     * @brief 设置时间片长度
     *
     * 设置RoundRobin调度算法的时间片大小。
     *
     * @param q 时间片长度（必须大于0）
     *
     * @note 只有在当前算法是RoundRobin时才会立即生效
     * @note 如果当前算法不是RoundRobin，会保存设置供后续使用
     * @see RoundRobinStrategy
     */
    void setTimeQuantum(int q);

    /**
     * @brief 获取当前调度算法
     *
     * @return 当前使用的调度算法类型
     */
    Algorithm currentAlgorithm() const;

    /**
     * @brief 添加新进程
     *
     * 将新到达的进程添加到调度系统中。
     *
     * @param p 要添加的进程指针
     * @return 进程插入就绪队列的位置索引
     *
     * @note 进程的到达时间会自动设置为当前tick
     * @note 进程会被添加到就绪队列和所有进程列表中
     * @note 调用者需要确保传入的指针是有效的
     * @see Process::setArrivalTime()
     */
    int addProcess(Process* p);

    /**
     * @brief 单步执行调度
     *
     * 执行一个时钟周期的调度，这是调度器的核心方法。
     *
     * ## 执行流程：
     * 1. 检查是否有正在运行的进程（多时间片场景）
     * 2. 如果没有运行进程，选择新进程
     * 3. 为本 tick 内仍在就绪队列中的进程累计等待时间
     * 4. 执行选中的进程
     * 5. 更新甘特图数据
     * 6. 检查进程是否完成
     * 7. 返回调度结果
     *
     * @return StepResult 包含调度结果的详细信息
     *
     * @note 每次调用通常会使m_tick增加1
     * @see StepResult
     */
    StepResult stepRun();

    /**
     * @brief 重置调度器
     *
     * 清除所有状态，恢复到初始状态，包括：
     * - 清理就绪队列和已完成列表的内存
     * - 重置所有计数器
     * - 清空甘特图数据
     * - 重新初始化调度策略
     *
     * @note 会释放所有进程的内存
     * @note 重置后需要重新设置调度算法和添加进程
     */
    void reset();

    /**
     * @name 访问器方法
     * @{
     */

    /**
     * @brief 获取就绪队列引用
     *
     * @return 就绪队列的非常量引用，允许外部修改
     */
    List<Process*>& getReadyList();

    /**
     * @brief 获取已完成进程列表
     *
     * @return 已完成进程列表的常量引用
     */
    const std::vector<Process*>& getFinishedList() const;

    /**
     * @brief 获取所有进程列表
     *
     * @return 所有进程列表的常量引用
     */
    const std::vector<Process*>& getAllProcesses() const;

    /**
     * @brief 获取当前时钟周期
     *
     * @return 当前tick值
     */
    int getTick() const;

    /**
     * @brief 获取当前运行的进程
     *
     * @return 当前运行进程的指针，可能为nullptr
     */
    Process* getCurrentProcess() const;

    /**
     * @} // 访问器方法
     */

    /**
     * @name 统计计算方法
     * @{
     */

    /**
     * @brief 计算平均等待时间
     *
     * @return 已完成进程的平均等待时间
     *
     * @note 如果没有已完成进程，返回0.0
     * @see Process::getWaitTime()
     */
    double avgWaitingTime() const;

    /**
     * @brief 计算平均周转时间
     *
     * @return 已完成进程的平均周转时间
     *
     * @note 如果没有已完成进程，返回0.0
     * @see Process::getTurnaroundTime()
     */
    double avgTurnaroundTime() const;

    /**
     * @brief 计算平均响应时间
     *
     * @return 已完成进程的平均响应时间
     *
     * @note 如果没有已完成进程，返回0.0
     * @see Process::getResponseTime()
     */
    double avgResponseTime() const;

    /**
     * @brief 计算CPU利用率
     *
     * @return CPU利用率百分比（0-100）
     *
     * @note 公式：CPU利用率 = (busyTicks / totalTicks) * 100
     * @note 如果没有tick，返回0.0
     */
    double cpuUtilization() const;

    /**
     * @} // 统计计算方法
     */

    /**
     * @brief 获取甘特图数据
     *
     * @return 甘特图数据的常量引用
     *
     * @see GanttEntry
     */
    const std::vector<GanttEntry>& getGanttData() const;

    /**
     * @brief 获取当前策略名称
     *
     * @return 当前调度策略的显示名称
     *
     * @see SchedulingStrategy::displayName()
     */
    QString strategyName() const;
};

#endif // SCHEDULER_H
