#include "ScheDuler.h"
#include "ListSorter.h"
#include "PriorityStrategy.h"
#include "FCFSStrategy.h"
#include "SJFStrategy.h"
#include "RoundRobinStrategy.h"
#include "MLFQStrategy.h"

/**
 * @file ScheDuler.cpp
 * @brief 进程调度器实现文件
 *
 * 实现了Scheduler类的所有方法，包括：
 * - 调度器的生命周期管理
 * - 调度算法的动态切换
 * - 进程调度的核心逻辑
 * - 统计信息的计算
 * - 甘特图数据的生成
 *
 * @author dyf
 * @version 1.0
 * @date 2024年3月
 */

/**
 * @brief 构造函数实现
 *
 * 初始化调度器，设置默认的调度策略为优先级调度。
 * 在构造时不需要任何参数，所有成员变量都有默认值。
 *
 * @note 使用std::make_unique创建策略对象，确保内存安全
 * @see PriorityStrategy
 */
Scheduler::Scheduler() {
    m_strategy = std::make_unique<PriorityStrategy>();
}

/**
 * @brief 析构函数实现
 *
 * 释放所有动态分配的内存，包括：
 * 1. 已完成进程列表中的所有进程对象
 * 2. 就绪队列中的所有进程对象
 *
 * @note List析构函数只删除节点，不会删除节点中的数据指针
 * @note 因此需要手动遍历并删除所有Process对象
 * @see m_finishedList
 * @see m_readyQueue
 */
Scheduler::~Scheduler() {
    // 删除已完成列表中的进程对象
    for(auto* p : m_finishedList) delete p;

    // readyQueue中的节点由List析构函数删除节点，但Process*需要手动删除
    auto node = m_readyQueue.getHead();
    while(node) {
        delete node->data;
        node = node->next;
    }

    if(m_currentProcess) {
        delete m_currentProcess;
    }
}

/**
 * @brief 设置调度算法
 *
 * 根据传入的算法类型，创建对应的调度策略对象。
 * 支持FCFS、SJF、Priority、RoundRobin和MLFQ五种算法。
 *
 * @param algo 要设置的调度算法类型
 *
 * @note 对于RoundRobin算法，会使用当前存储的时间片设置
 * @note 切换算法会重置当前策略对象
 * @see Algorithm
 * @see SchedulingStrategy
 */
void Scheduler::setStrategy(Algorithm algo) {
    m_currentAlgo = algo;
    switch(algo) {
        case Algorithm::FCFS:
            m_strategy = std::make_unique<FCFSStrategy>();
            break;
        case Algorithm::SJF:
            m_strategy = std::make_unique<SJFStrategy>();
            break;
        case Algorithm::Priority:
            m_strategy = std::make_unique<PriorityStrategy>();
            break;
        case Algorithm::RoundRobin:
            m_strategy = std::make_unique<RoundRobinStrategy>(m_timeQuantum);
            break;
        case Algorithm::MLFQ:
            m_strategy = std::make_unique<MLFQStrategy>();
            break;
    }
}

/**
 * @brief 设置时间片长度
 *
 * 设置RoundRobin调度算法的时间片大小。
 *
 * @param q 新的时间片长度（单位：时钟周期）
 *
 * @note 如果当前算法是RoundRobin，会立即更新策略对象的时间片
 * @note 如果不是RoundRobin，只保存设置供后续使用
 * @see RoundRobinStrategy
 */
void Scheduler::setTimeQuantum(int q) {
    m_timeQuantum = q;
    if(m_currentAlgo == Algorithm::RoundRobin) {
        // 动态转换为RoundRobinStrategy指针
        auto* rr = dynamic_cast<RoundRobinStrategy*>(m_strategy.get());
        if(rr) rr->setQuantum(q);
    }
}

/**
 * @brief 获取当前调度算法
 *
 * @return 当前使用的调度算法枚举值
 */
Algorithm Scheduler::currentAlgorithm() const { return m_currentAlgo; }

/**
 * @brief 添加进程到调度器
 *
 * 处理新到达的进程：
 * 1. 设置进程的到达时间为当前tick
 * 2. 将进程添加到所有进程列表中（用于统计）
 * 3. 通过当前策略将进程插入就绪队列
 *
 * @param p 要添加的进程指针
 * @return 进程在就绪队列中的插入位置索引
 *
 * @note 调用者需确保传入的指针是new创建的（需要被管理）
 * @see Process::setArrivalTime()
 * @see SchedulingStrategy::insertNew()
 */
int Scheduler::addProcess(Process* p) {
    p->setArrivalTime(m_tick);              // 设置到达时间
    m_allProcesses.push_back(p);            // 添加到所有进程列表
    return m_strategy->insertNew(m_readyQueue, p);  // 通过策略插入就绪队列
}

/**
 * @brief 单步调度执行
 *
 * 这是调度器的核心方法，每个tick执行一次，完成以下任务：
 *
 * ## 执行流程：
 * 1. **选择进程**：
 *    - 如果有正在运行的进程且还有剩余时间片，继续运行
 *    - 否则通过调度策略选择下一个进程
 * 2. **累计等待**：为本 tick 内仍在就绪队列中的进程增加等待时间
 * 3. **执行进程**：如果没有进程，tick增加并返回
 * 4. **更新状态**：设置开始时间、执行进程、更新时间片
 * 5. **记录数据**：生成甘特图数据
 * 6. **检查结果**：进程是否完成或时间片是否用完
 *
 * @return StepResult 包含本次调度的详细结果
 *
 * @see StepResult
 * @see SchedulingStrategy::selectNext()
 * @see Process::execute()
 * @see Process::updateState()
 */
Process* Scheduler::selectProcessForStep() {
    Process* p = nullptr;
    if(m_currentProcess && m_remainingSlice > 0) {
        p = m_currentProcess;
    } else {
        p = m_strategy->selectNext(m_readyQueue);
        if(p) {
            m_remainingSlice = m_strategy->getTimeSlice();
            m_currentProcess = p;
        }
    }
    return p;
}

StepResult Scheduler::buildIdleStepResult() {
    StepResult result;
    m_tick++;
    m_currentProcess = nullptr;
    result.explanation = m_strategy->explainStep(nullptr, m_tick);
    return result;
}

void Scheduler::incrementReadyWaitTimes() {
    auto node = m_readyQueue.getHead();
    while(node) {
        node->data->incrementWaitTime();
        node = node->next;
    }
}

void Scheduler::executeSelectedProcess(Process* p, StepResult& result) {
    p->setStartTime(m_tick);
    if(p->getState() != 'R') {
        p->setRunning();
    }
    p->execute();
    m_strategy->onExecuted(p);
    m_remainingSlice--;
    m_tick++;
    m_busyTicks++;

    result.process = p;
    result.explanation = m_strategy->explainStep(p, m_tick);
}

void Scheduler::updateGanttData(Process* p) {
    if(!m_ganttData.empty() && m_ganttData.back().name == QString::fromStdString(p->getName())
       && m_ganttData.back().endTick == m_tick - 1) {
        m_ganttData.back().endTick = m_tick;
        return;
    }

    m_ganttData.push_back({
        QString::fromStdString(p->getName()),
        p->getColor(),
        m_tick - 1,
        m_tick
    });
}

void Scheduler::finalizeStepResult(Process* p, StepResult& result) {
    if(p->getRemainingTime() <= 0) {
        p->setFinished();
        p->setFinishTime(m_tick);
        m_finishedList.push_back(p);
        result.justFinished = true;
        result.reinsertIndex = -1;
        m_currentProcess = nullptr;
        m_remainingSlice = 0;
        return;
    }

    if(m_remainingSlice <= 0) {
        p->setReady();
        result.reinsertIndex = m_strategy->reinsert(m_readyQueue, p);
        result.justFinished = false;
        m_currentProcess = nullptr;
        m_remainingSlice = 0;
        return;
    }

    result.justFinished = false;
    result.reinsertIndex = -1;
}

StepResult Scheduler::stepRun() {
    Process* p = selectProcessForStep();

    if(!p) {
        return buildIdleStepResult();
    }

    StepResult result;
    incrementReadyWaitTimes();
    executeSelectedProcess(p, result);
    updateGanttData(p);
    finalizeStepResult(p, result);

    return result;
}

/**
 * @brief 重置调度器
 *
 * 将调度器恢复到初始状态，用于重新开始调度模拟。
 *
 * ## 清理内容：
 * - 释放就绪队列中的所有进程对象
 * - 释放已完成列表中的所有进程对象
 * - 清空所有容器
 * - 重置所有计数器
 * - 重建调度策略
 *
 * @note 会释放所有进程对象的内存
 * @note 重置后需要重新设置算法和添加进程
 * @see ~Scheduler()
 */
void Scheduler::reset() {
    // 清理就绪队列中的Process*
    auto node = m_readyQueue.getHead();
    while(node) {
        delete node->data;
        node = node->next;
    }
    m_readyQueue.setHead(nullptr);

    // 清理已完成列表
    for(auto* p : m_finishedList) delete p;
    m_finishedList.clear();

    if(m_currentProcess) {
        delete m_currentProcess;
        m_currentProcess = nullptr;
    }

    // 清理其他容器
    m_allProcesses.clear();
    m_ganttData.clear();

    // 重置计数器
    m_tick = 0;
    m_busyTicks = 0;
    m_remainingSlice = 0;

    // 重建策略（保持当前算法）
    setStrategy(m_currentAlgo);
}

/**
 * @brief 获取就绪队列引用
 *
 * @return 就绪队列的非常量引用，允许外部修改
 */
List<Process*>& Scheduler::getReadyList() { return m_readyQueue; }

/**
 * @brief 获取已完成进程列表
 *
 * @return 已完成进程列表的常量引用
 */
const std::vector<Process*>& Scheduler::getFinishedList() const { return m_finishedList; }

/**
 * @brief 获取所有进程列表
 *
 * @return 所有进程列表的常量引用
 */
const std::vector<Process*>& Scheduler::getAllProcesses() const { return m_allProcesses; }

/**
 * @brief 获取当前时钟周期
 *
 * @return 当前tick值
 */
int Scheduler::getTick() const { return m_tick; }

/**
 * @brief 获取当前运行的进程
 *
 * @return 当前运行进程的指针，可能为nullptr
 */
Process* Scheduler::getCurrentProcess() const { return m_currentProcess; }

/**
 * @brief 计算平均等待时间
 *
 * 统计所有已完成进程的平均等待时间。
 *
 * @return 平均等待时间，如果没有完成进程返回0.0
 *
 * @see Process::getWaitTime()
 */
double Scheduler::avgWaitingTime() const {
    if(m_finishedList.empty()) return 0.0;
    double sum = 0;
    for(auto* p : m_finishedList) sum += p->getWaitTime();
    return sum / m_finishedList.size();
}

/**
 * @brief 计算平均周转时间
 *
 * 统计所有已完成进程的平均周转时间。
 *
 * @return 平均周转时间，如果没有完成进程返回0.0
 *
 * @see Process::getTurnaroundTime()
 */
double Scheduler::avgTurnaroundTime() const {
    if(m_finishedList.empty()) return 0.0;
    double sum = 0;
    for(auto* p : m_finishedList) sum += p->getTurnaroundTime();
    return sum / m_finishedList.size();
}

/**
 * @brief 计算平均响应时间
 *
 * 统计所有已完成进程的平均响应时间。
 *
 * @return 平均响应时间，如果没有完成进程返回0.0
 *
 * @see Process::getResponseTime()
 */
double Scheduler::avgResponseTime() const {
    if(m_finishedList.empty()) return 0.0;
    double sum = 0;
    for(auto* p : m_finishedList) sum += p->getResponseTime();
    return sum / m_finishedList.size();
}

/**
 * @brief 计算CPU利用率
 *
 * 计算在整个调度过程中的CPU利用率百分比。
 *
 * @return CPU利用率（0-100），如果没有tick返回0.0
 *
 * @note 公式：CPU利用率 = (busyTicks / totalTicks) * 100
 * @see m_busyTicks
 * @see m_tick
 */
double Scheduler::cpuUtilization() const {
    if(m_tick == 0) return 0.0;
    return (double)m_busyTicks / m_tick * 100.0;
}

/**
 * @brief 获取甘特图数据
 *
 * @return 甘特图数据的常量引用
 *
 * @see GanttEntry
 */
const std::vector<GanttEntry>& Scheduler::getGanttData() const { return m_ganttData; }

/**
 * @brief 获取当前策略名称
 *
 * @return 当前调度策略的显示名称字符串
 *
 * @note 如果策略对象不存在，返回空字符串
 * @see SchedulingStrategy::displayName()
 */
QString Scheduler::strategyName() const {
    return m_strategy ? m_strategy->displayName() : QString();
}
