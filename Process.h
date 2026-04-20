#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <QColor>

/**
 * @file Process.h
 * @brief 进程调度系统中的进程类头文件
 *
 * 该文件定义了Process类，用于模拟操作系统中的进程实体。
 * 类实现了进程的生命周期管理、状态转换和统计信息计算。
 *
 * @author dyf
 * @version 1.0
 * @date 2024年3月
 */

/**
 * @class Process
 * @brief 进程实体类
 *
 * Process类模拟操作系统中的进程，管理进程的状态、优先级、
 * 执行时间和统计信息。类实现了完整的进程生命周期管理，
 * 支持状态转换、时间统计和进程克隆功能。
 *
 * ## 进程状态
 * - W: 就绪状态 (Waiting/Ready)
 * - R: 运行状态 (Running)
 * - F: 完成状态 (Finished)
 *
 * ## 使用示例
 * @code
 * // 创建进程
 * Process* p = new Process("P1", 5, 10);
 * p->setColor(ColorGenerator::next());
 * p->setArrivalTime(0);
 *
 * // 执行进程
 * if(p->setRunning()) {
 *     p->execute();          // 执行一个时间单位
 *     p->updateState();      // 更新状态
 * }
 *
 * // 获取统计信息
 * int waitTime = p->getWaitTime();
 * int turnaround = p->getTurnaroundTime();
 * @endcode
 *
 * ## 状态转换图
 * @dot
 * digraph ProcessState {
 *     rankdir=LR;
 *     node [shape=circle, style=filled];
 *     W [label="就绪\n(W)", fillcolor=lightblue];
 *     R [label="运行\n(R)", fillcolor=lightgreen];
 *     F [label="完成\n(F)", fillcolor=lightgray];
 *
 *     W -> R [label="setRunning()"];
 *     R -> W [label="updateState()", style=dashed];
 *     R -> F [label="updateState()"];
 * }
 * @enddot
 *
 * @see ColorGenerator
 * @see SchedulingStrategy
 * @see Scheduler
 */
class Process{
private:
    /**
     * @var std::string m_name
     * @brief 进程名称
     *
     * 进程的唯一标识符，通常格式为 "P1", "P2", "ProcessA" 等。
     * 用于在UI组件中显示和区分不同进程。
     */
    std::string m_name;

    /**
     * @var char m_state
     * @brief 进程状态
     *
     * 表示进程的当前状态，取值为：
     * - 'W': 就绪状态 (Waiting/Ready) - 等待CPU调度
     * - 'R': 运行状态 (Running) - 正在CPU上执行
     * - 'F': 完成状态 (Finished) - 执行完成，等待回收
     *
     * @see setRunning()
     * @see updateState()
     * @see isFinish()
     */
    char m_state;

    /**
     * @var int m_priority
     * @brief 当前优先级
     *
     * 进程的当前优先级，数值越大表示优先级越高。
     * 在调度算法中用于决定进程的执行顺序。
     * 该值可能随时间变化（如MLFQ算法中的优先级调整）。
     *
     * @see getPriority()
     * @see decreasePriority()
     */
    int m_priority;

    /**
     * @var int m_ntime
     * @brief 总需要运行时间
     *
     * 进程从开始到完成所需的总CPU时间。
     * 在进程创建时设置，执行过程中保持不变。
     *
     * @see getNTime()
     * @see getRemainingTime()
     */
    int m_ntime;

    /**
     * @var int m_rtime
     * @brief 已运行时间
     *
     * 进程已经执行的CPU时间累计值。
     * 每次调用execute()方法时增加1。
     *
     * @see execute()
     * @see getRTime()
     * @see getRemainingTime()
     */
    int m_rtime;

    /**
     * @var int m_originalPriority
     * @brief 原始优先级
     *
     * 进程创建时的初始优先级，在MLFQ等算法中需要
     * 保留原始优先级用于统计和显示目的。
     * 该值在进程生命周期内保持不变。
     *
     * @see getOriginalPriority()
     * @see clone()
     */
    int m_originalPriority;

    /**
     * @var int m_arrivalTime
     * @brief 到达时间
     *
     * 进程到达就绪队列的时间点（以tick为单位）。
     * 用于计算周转时间和响应时间。
     *
     * @see setArrivalTime()
     * @see getArrivalTime()
     * @see getTurnaroundTime()
     * @see getResponseTime()
     */
    int m_arrivalTime;

    /**
     * @var int m_waitTime
     * @brief 累计等待时间
     *
     * 进程在就绪队列中等待的总时间。
     * 每当进程处于就绪状态一个tick，该值增加1。
     *
     * @see incrementWaitTime()
     * @see getWaitTime()
     */
    int m_waitTime;

    /**
     * @var int m_startTime
     * @brief 首次开始运行时间
     *
     * 进程第一次获得CPU并开始执行的时间点。
     * 初始值为-1，表示进程尚未开始执行。
     * 用于计算响应时间。
     *
     * @see setStartTime()
     * @see getStartTime()
     * @see getResponseTime()
     */
    int m_startTime;

    /**
     * @var int m_finishTime
     * @brief 完成时间
     *
     * 进程执行完成的时间点。
     * 初始值为-1，表示进程尚未完成。
     * 用于计算周转时间。
     *
     * @see setFinishTime()
     * @see getFinishTime()
     * @see getTurnaroundTime()
     */
    int m_finishTime;

    /**
     * @var QColor m_color
     * @brief 显示颜色
     *
     * 进程在UI组件中显示时使用的颜色。
     * 从ColorGenerator获取，用于区分不同进程。
     * 在甘特图、进程卡片等组件中使用。
     *
     * @see setColor()
     * @see getColor()
     * @see ColorGenerator
     */
    QColor m_color;

public:
    /**
     * @brief 构造函数 - 创建新进程
     *
     * 创建一个新的进程实例，初始化基本属性。
     *
     * @param name 进程名称，如 "P1" 或 "ProcessA"
     * @param priority 进程优先级，数值越大优先级越高
     * @param ntime 进程需要的总CPU时间
     *
     * @note 初始状态设置为就绪('W')，运行时间为0
     * @note 开始时间和完成时间初始化为-1（表示未执行/未完成）
     * @see Process(const std::string&, int, int, int)
     * @see ColorGenerator::next()
     */
    Process(const std::string& name, int priority, int ntime);

    /**
     * @brief 高级构造函数 - 指定已运行时间
     *
     * 创建一个进程实例，并指定已经运行的时间。
     * 主要用于进程克隆和恢复场景。
     *
     * @param name 进程名称
     * @param priority 进程优先级
     * @param rtime 已经运行的时间
     * @param ntime 进程需要的总CPU时间
     *
     * @note 该构造函数允许创建部分执行的进程状态
     * @see clone()
     */
    Process(const std::string& name, int priority, int rtime, int ntime);

    /**
     * @brief 默认构造函数
     *
     * 创建一个空进程实例，所有属性初始化为默认值。
     *
     * @note 该构造函数主要用于特殊情况，一般使用带参数的构造函数
     */
    Process();

    /**
     * @brief 析构函数
     *
     * 默认析构函数，不执行特殊清理操作。
     * 进程对象通常由调用者管理内存。
     */
    ~Process() = default;

    /**
     * @brief 将进程设置为运行状态
     *
     * 尝试将进程从就绪状态转换为运行状态。
     *
     * @return bool 转换是否成功
     *
     * @retval true 成功转换为运行状态
     * @retval false 无法转换（状态不是就绪，或已经执行完毕）
     *
     * @note 该方法会检查状态转换的条件
     * @note 成功调用后，m_state 变为 'R'
     * @see m_state
     * @see updateState()
     */
    bool setRunning();

    /**
     * @brief 检查进程是否已完成
     *
     * 判断当前进程是否处于完成状态。
     *
     * @return bool 进程是否完成
     *
     * @retval true 进程已完成（m_state == 'F'）
     * @retval false 进程未完成
     *
     * @see m_state
     * @see updateState()
     */
    bool isFinish() const;

    /**
     * @brief 执行一个时间单位
     *
     * 如果进程处于运行状态，增加已运行时间。
     *
     * @note 只有运行状态的进程才会执行
     * @note 每次调用增加 m_rtime 的值
     * @see m_state
     * @see m_rtime
     * @see updateState()
     */
    void execute();

    /**
     * @brief 更新进程状态
     *
     * 根据已运行时间和总时间的关系，更新进程状态。
     *
     * ## 状态转换逻辑：
     * - 如果 m_rtime < m_ntime：转换为就绪状态('W')
     * - 如果 m_rtime >= m_ntime：转换为完成状态('F')
     *
     * @note 通常在execute()之后调用
     * @note 时间片轮转调度中常用
     *
     * @see m_state
     * @see m_rtime
     * @see m_ntime
     */
    void updateState();

    /**
     * @brief 降低进程优先级
     *
     * 将进程的优先级减1。
     *
     * @note 主要用于MLFQ（多级反馈队列）算法
     * @note 在进程用完时间片后调用
     * @see m_priority
     * @see MLFQStrategy
     */
    void decreasePriority();

    /**
     * @brief 增加等待时间
     *
     * 如果进程处于就绪状态，增加其等待时间。
     *
     * @note 每个调度tick中调用
     * @note 只有就绪状态的进程才会增加等待时间
     * @see m_waitTime
     * @see m_state
     */
    void incrementWaitTime();

    /**
     * @brief 设置到达时间
     *
     * 设置进程到达就绪队列的时间点。
     *
     * @param tick 到达时间（以调度tick为单位）
     * @see m_arrivalTime
     * @see getTurnaroundTime()
     */
    void setArrivalTime(int tick);

    /**
     * @brief 设置开始时间
     *
     * 设置进程首次获得CPU并开始执行的时间。
     *
     * @param tick 开始时间（以调度tick为单位）
     *
     * @note 只在进程第一次执行时设置一次
     * @note 如果已经设置过，则不再修改
     * @see m_startTime
     * @see getResponseTime()
     */
    void setStartTime(int tick);

    /**
     * @brief 设置完成时间
     *
     * 设置进程执行完成的时间点。
     *
     * @param tick 完成时间（以调度tick为单位）
     * @see m_finishTime
     * @see getTurnaroundTime()
     */
    void setFinishTime(int tick);

    /**
     * @brief 设置显示颜色
     *
     * 设置进程在UI组件中显示的颜色。
     *
     * @param color 显示颜色，通常从ColorGenerator获取
     * @see m_color
     * @see ColorGenerator
     * @see ProcessCard
     */
    void setColor(const QColor& color);

    /**
     * @brief 克隆进程
     *
     * 创建当前进程的副本，用于算法对比等场景。
     *
     * @return Process* 新创建的进程副本
     *
     * @note 返回的指针需要调用者管理内存
     * @note 克隆的进程重置了运行状态和时间统计
     *
     * 克隆内容：
     * - ✅ 进程名称
     * - ✅ 原始优先级
     * - ✅ 总运行时间
     * - ✅ 显示颜色
     * - ✅ 到达时间
     * - ❌ 运行状态（重置为就绪）
     * - ❌ 已运行时间（重置为0）
     * - ❌ 开始/完成时间（重置为-1）
     *
     * @see ComparisonDialog
     * @see Scheduler
     */
    Process* clone() const;

    /**
     * @name Getter方法
     * @{
     */

    /**
     * @brief 获取当前优先级
     *
     * @return int 当前优先级值
     * @see m_priority
     */
    int getPriority() const;

    /**
     * @brief 获取原始优先级
     *
     * @return int 创建时的初始优先级
     * @see m_originalPriority
     */
    int getOriginalPriority() const;

    /**
     * @brief 获取进程名称
     *
     * @return std::string 进程名称
     * @see m_name
     */
    std::string getName() const;

    /**
     * @brief 获取总运行时间
     *
     * @return int 进程需要的总CPU时间
     * @see m_ntime
     */
    int getNTime() const;

    /**
     * @brief 获取已运行时间
     *
     * @return int 已经执行的CPU时间
     * @see m_rtime
     */
    int getRTime() const;

    /**
     * @brief 获取剩余运行时间
     *
     * @return int 还需要执行的CPU时间
     *
     * @note 计算方式：m_ntime - m_rtime
     * @see m_ntime
     * @see m_rtime
     */
    int getRemainingTime() const;

    /**
     * @brief 获取当前状态
     *
     * @return char 状态字符：'W', 'R', 或 'F'
     * @see m_state
     */
    char getState() const;

    /**
     * @brief 获取到达时间
     *
     * @return int 到达时间（tick）
     * @see m_arrivalTime
     */
    int getArrivalTime() const;

    /**
     * @brief 获取等待时间
     *
     * @return int 累计等待时间
     * @see m_waitTime
     */
    int getWaitTime() const;

    /**
     * @brief 获取开始时间
     *
     * @return int 首次开始时间，-1表示未开始
     * @see m_startTime
     */
    int getStartTime() const;

    /**
     * @brief 获取完成时间
     *
     * @return int 完成时间，-1表示未完成
     * @see m_finishTime
     */
    int getFinishTime() const;

    /**
     * @brief 获取周转时间
     *
     * 计算从进程到达直到完成的总时间。
     *
     * @return int 周转时间
     *
     * @note 公式：周转时间 = 完成时间 - 到达时间
     * @note 如果进程未完成，返回0
     * @see m_arrivalTime
     * @see m_finishTime
     */
    int getTurnaroundTime() const;

    /**
     * @brief 获取响应时间
     *
     * 计算从进程到达直到首次获得CPU的时间。
     *
     * @return int 响应时间
     *
     * @note 公式：响应时间 = 首次开始时间 - 到达时间
     * @note 如果进程未开始，返回0
     * @see m_arrivalTime
     * @see m_startTime
     */
    int getResponseTime() const;

    /**
     * @brief 获取显示颜色
     *
     * @return QColor 进程在UI中的显示颜色
     * @see m_color
     */
    QColor getColor() const;

    /**
     * @} // Getter方法
     */

private:
    // /**
    //  * @brief 私有构造函数（已删除）
    //  *
    //  * 防止意外创建未初始化的进程实例。
    //  */
    // Process() = delete;
};

#endif // PROCESS_H
