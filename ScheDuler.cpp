#include "Scheduler.h"
#include "ListSorter.h"
#include <iostream>
#include <sstream>


void Scheduler::ListInfo(Process* p){
    std::cout << "名称:" << p->getName() << std::endl << "优先级:" << p->getPriority() << std::endl << "状态:" << p->getState()
        << std::endl << "运行时间:" << p->getRTime() << std::endl << "总时间:" << p->getNTime() << std::endl;
}

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

void Scheduler::run(){
    while(!this->m_process.empty()){
        this->m_count++;
        std::cout << "第" << m_count <<"次调度:" << std::endl;
        auto font = this->m_process.front();
        this->m_process.pop_front();

        if(font->setRunning()){
            //打印
            display(font);

            font->execute();
            font->updateState();

            if(font->isFinish()){

                std::cout << "进程[" << font->getName() << "]" << "运行完成!" << std::endl;
                delete font;
            }
            else{
                //插入链表继续运行
                ListSorter::insertByPriority(this->m_process,font);
            }
        }
    }
}

void Scheduler::display(Process* p){
    std::cout << "----------------------------------------------------" << std::endl;

    std::cout << "当前进程: " << p->getName() << std::endl;
    ListInfo(p);

    std::cout << "当前就绪队列状态为：" << std::endl;
    auto curr = this->m_process.getHead();
    if (!curr)
        std::cout << "(空)" << std::endl;
    else {
        while (curr) {
            ListInfo(curr->data);
            curr = curr->next;
        }
    }
    std::cout << "----------------------------------------------------" << std::endl;
}


std::istream& operator>>(std::istream& is,Scheduler& s){
    int num;

    if (&is == &std::cin) {
        std::cout << "请输入调度进程数目：";

        is >> num;
        for (int i = 0; i < num; ++i) {
            std::string name;
            int priority, rtime;
            std::cout << "\n输入进程号 No." << i + 1 << "：" << std::endl;
            std::cout << "输入进程名：";
            is >> name;
            std::cout << "输入进程优先数：";
            is >> priority;
            std::cout << "输入进程运行时间：";
            is >> rtime;

            ListSorter::insertByPriority(s.getList(),new Process(name, priority, rtime));
        }
    }
    else{
        std::string line;
        while(getline(is,line)){
            //跳过空行
            if(line.empty()) continue;

            std::stringstream lineStream(line);
            std::string name;
            int priority, rtime;

            lineStream >> name >> priority >> rtime;

            ListSorter::insertByPriority(s.getList(),new Process(name, priority, rtime));
        }
    }


    return is;
}
