#include "ScheDuler.h"
#include "ListSorter.h"
#include "PriorityStrategy.h"
#include "FCFSStrategy.h"
#include "SJFStrategy.h"
#include "RoundRobinStrategy.h"
#include "MLFQStrategy.h"

Scheduler::Scheduler() {
    m_strategy = std::make_unique<PriorityStrategy>();
}

Scheduler::~Scheduler() {
    for(auto* p : m_finishedList) delete p;
    // readyQueue中的节点由List析构函数删除节点，但Process*需要手动删除
    auto node = m_readyQueue.getHead();
    while(node) {
        delete node->data;
        node = node->next;
    }
}

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

void Scheduler::setTimeQuantum(int q) {
    m_timeQuantum = q;
    if(m_currentAlgo == Algorithm::RoundRobin) {
        auto* rr = dynamic_cast<RoundRobinStrategy*>(m_strategy.get());
        if(rr) rr->setQuantum(q);
    }
}

Algorithm Scheduler::currentAlgorithm() const { return m_currentAlgo; }

int Scheduler::addProcess(Process* p) {
    p->setArrivalTime(m_tick);
    m_allProcesses.push_back(p);
    return m_strategy->insertNew(m_readyQueue, p);
}

StepResult Scheduler::stepRun() {
    StepResult result;

    // 增加就绪队列中所有进程的等待时间
    auto node = m_readyQueue.getHead();
    while(node) {
        node->data->incrementWaitTime();
        node = node->next;
    }

    // 检查是否有正在运行的进程(多时间片场景)
    Process* p = nullptr;

    if(m_currentProcess && m_remainingSlice > 0) {
        // 继续运行当前进程
        p = m_currentProcess;
    } else {
        // 选择新进程
        p = m_strategy->selectNext(m_readyQueue);
        if(p) {
            m_remainingSlice = m_strategy->getTimeSlice();
            m_currentProcess = p;
        }
    }

    if(!p) {
        m_tick++;
        m_currentProcess = nullptr;
        result.explanation = m_strategy->explainStep(nullptr, m_tick);
        return result;
    }

    // 记录首次运行时间
    p->setStartTime(m_tick);

    // 执行
    p->setRunning();
    p->execute();
    m_strategy->onExecuted(p);
    p->updateState();
    m_remainingSlice--;
    m_tick++;
    m_busyTicks++;

    result.process = p;
    result.explanation = m_strategy->explainStep(p, m_tick);

    // 记录甘特图
    if(!m_ganttData.empty() && m_ganttData.back().name == QString::fromStdString(p->getName())
       && m_ganttData.back().endTick == m_tick - 1) {
        m_ganttData.back().endTick = m_tick;
    } else {
        m_ganttData.push_back({QString::fromStdString(p->getName()), p->getColor(), m_tick - 1, m_tick});
    }

    if(p->isFinish()) {
        p->setFinishTime(m_tick);
        m_finishedList.push_back(p);
        result.justFinished = true;
        result.reinsertIndex = -1;
        m_currentProcess = nullptr;
        m_remainingSlice = 0;
    } else if(m_remainingSlice <= 0) {
        // 时间片用完，重新插入
        result.reinsertIndex = m_strategy->reinsert(m_readyQueue, p);
        result.justFinished = false;
        m_currentProcess = nullptr;
    } else {
        // 还有剩余时间片，继续运行
        result.justFinished = false;
        result.reinsertIndex = -1;
    }

    return result;
}

void Scheduler::reset() {
    // 清理就绪队列中的Process*
    auto node = m_readyQueue.getHead();
    while(node) {
        delete node->data;
        node = node->next;
    }
    m_readyQueue.setHead(nullptr);

    for(auto* p : m_finishedList) delete p;
    m_finishedList.clear();
    m_allProcesses.clear();
    m_ganttData.clear();
    m_tick = 0;
    m_busyTicks = 0;
    m_remainingSlice = 0;
    m_currentProcess = nullptr;

    // 重建策略
    setStrategy(m_currentAlgo);
}

List<Process*>& Scheduler::getReadyList() { return m_readyQueue; }
const std::vector<Process*>& Scheduler::getFinishedList() const { return m_finishedList; }
const std::vector<Process*>& Scheduler::getAllProcesses() const { return m_allProcesses; }
int Scheduler::getTick() const { return m_tick; }
Process* Scheduler::getCurrentProcess() const { return m_currentProcess; }

double Scheduler::avgWaitingTime() const {
    if(m_finishedList.empty()) return 0.0;
    double sum = 0;
    for(auto* p : m_finishedList) sum += p->getWaitTime();
    return sum / m_finishedList.size();
}

double Scheduler::avgTurnaroundTime() const {
    if(m_finishedList.empty()) return 0.0;
    double sum = 0;
    for(auto* p : m_finishedList) sum += p->getTurnaroundTime();
    return sum / m_finishedList.size();
}

double Scheduler::avgResponseTime() const {
    if(m_finishedList.empty()) return 0.0;
    double sum = 0;
    for(auto* p : m_finishedList) sum += p->getResponseTime();
    return sum / m_finishedList.size();
}

double Scheduler::cpuUtilization() const {
    if(m_tick == 0) return 0.0;
    return (double)m_busyTicks / m_tick * 100.0;
}

const std::vector<GanttEntry>& Scheduler::getGanttData() const { return m_ganttData; }

QString Scheduler::strategyName() const {
    return m_strategy ? m_strategy->displayName() : QString();
}
