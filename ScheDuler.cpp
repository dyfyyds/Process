#include "Scheduler.h"
#include "ListSorter.h"



Scheduler::Scheduler():m_count(0) {}

Scheduler::~Scheduler(){
    auto node = this->m_process.getHead();

    while(node)
    {
        delete node->data;
        node = node->next;
    }
}


List<Process*>& Scheduler::getList(){
    return this->m_process;
}

Process *Scheduler::stepRun()
{
    if(m_process.empty()) return nullptr;

    m_count++;

    //取出优先级最高的
    Process* font = m_process.front();
    m_process.pop_front();

    if (font->setRunning()) {
        font->execute();
        font->updateState();
    }
    return font;
}
