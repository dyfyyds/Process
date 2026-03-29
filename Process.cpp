#include "Process.h"

Process::Process(const std::string& name, int priority, int ntime)
    : m_name(name), m_state('W'), m_priority(priority), m_ntime(ntime), m_rtime(0)
    , m_originalPriority(priority), m_arrivalTime(0), m_waitTime(0)
    , m_startTime(-1), m_finishTime(-1), m_color(Qt::cyan)
{}

Process::Process(const std::string& name, int priority, int rtime, int ntime)
    : m_name(name), m_state('W'), m_priority(priority), m_ntime(ntime), m_rtime(rtime)
    , m_originalPriority(priority), m_arrivalTime(0), m_waitTime(0)
    , m_startTime(-1), m_finishTime(-1), m_color(Qt::cyan)
{}

Process::Process()
    : m_name(""), m_state('W'), m_priority(0), m_ntime(0), m_rtime(0)
    , m_originalPriority(0), m_arrivalTime(0), m_waitTime(0)
    , m_startTime(-1), m_finishTime(-1), m_color(Qt::cyan)
{}

bool Process::setRunning(){
    if(this->m_state == 'W' && this->m_rtime < this->m_ntime){
        this->m_state = 'R';
        return true;
    }
    return false;
}

bool Process::isFinish() const{
    return this->m_state == 'F';
}

void Process::execute(){
    if(this->m_state == 'R'){
        this->m_rtime++;
    }
}

void Process::updateState(){
    if(this->m_rtime < this->m_ntime)
        this->m_state = 'W';
    else
        this->m_state = 'F';
}

void Process::decreasePriority(){
    this->m_priority--;
}

void Process::incrementWaitTime(){
    if(this->m_state == 'W')
        this->m_waitTime++;
}

void Process::setArrivalTime(int tick){ m_arrivalTime = tick; }
void Process::setStartTime(int tick){ if(m_startTime < 0) m_startTime = tick; }
void Process::setFinishTime(int tick){ m_finishTime = tick; }
void Process::setColor(const QColor& color){ m_color = color; }

Process* Process::clone() const{
    Process* p = new Process(m_name, m_originalPriority, m_ntime);
    p->m_color = m_color;
    p->m_arrivalTime = m_arrivalTime;
    return p;
}

int Process::getPriority() const{ return m_priority; }
int Process::getOriginalPriority() const{ return m_originalPriority; }
std::string Process::getName() const{ return m_name; }
int Process::getNTime() const{ return m_ntime; }
int Process::getRTime() const{ return m_rtime; }
int Process::getRemainingTime() const{ return m_ntime - m_rtime; }
char Process::getState() const{ return m_state; }
int Process::getArrivalTime() const{ return m_arrivalTime; }
int Process::getWaitTime() const{ return m_waitTime; }
int Process::getStartTime() const{ return m_startTime; }
int Process::getFinishTime() const{ return m_finishTime; }
int Process::getTurnaroundTime() const{
    if(m_finishTime < 0) return 0;
    return m_finishTime - m_arrivalTime;
}
int Process::getResponseTime() const{
    if(m_startTime < 0) return 0;
    return m_startTime - m_arrivalTime;
}
QColor Process::getColor() const{ return m_color; }
