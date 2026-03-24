#include "Process.h"

#include <qDebug>

Process::Process(const std::string& name,int priority,int ntime):
    m_name(name),m_state('W'),m_priority(priority),m_ntime(ntime),m_rtime(0){}


Process::Process(): m_name(""), m_state('W'), m_priority(0), m_ntime(0), m_rtime(0) {}


bool Process::setRunning(){
    if(this->m_state == 'W' && this->m_rtime < this->m_ntime)
    {
        this->m_state = 'R';
        return true;
    }

    return false;
}

bool Process::isFinish(){
    return this->m_state == 'F';
}

void Process::execute(){
    if(this->m_state == 'R')
    {
        this->m_priority--;
        this->m_rtime++;
    }
}

void Process::updateState(){

    if(this->m_rtime < this->m_ntime)
        this->m_state = 'W';
    else
        this->m_state = 'F';
}

int Process::getPriority() const{
    return this->m_priority;
}

std::string Process::getName() const{
    return this->m_name;
}

int Process::getNTime() const{
    return this->m_ntime;
}

int Process::getRTime() const{
    return this->m_rtime;
}

char Process::getState() const{
    return m_state;
}

