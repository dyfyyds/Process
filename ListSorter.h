#ifndef LISTSORTER_H
#define LISTSORTER_H

#include "List.h"
#include "Process.h"

class ListSorter{
public:
    template <typename T>
    static int insertByPriority(List<T>& list, Process* p){
        auto head = list.getHead();
        int count = 0;

        if(!head || p->getPriority() >= head->data->getPriority()){
            list.push_front(p);
            count = 0;
        }
        else{
            auto cur = head;
            count = 1;

            while(cur->next && cur->next->data->getPriority() >= p->getPriority()){
                cur = cur->next;
                count++;
            }

            list.insert(cur,p);
        }
        return count;
    }

    template <typename T>
    static int insertBySJF(List<T>& list, Process* p){
        auto head = list.getHead();
        int count = 0;

        if(!head || p->getRemainingTime() <= head->data->getRemainingTime()){
            list.push_front(p);
            count = 0;
        }
        else{
            auto cur = head;
            count = 1;

            while(cur->next && cur->next->data->getRemainingTime() <= p->getRemainingTime()){
                cur = cur->next;
                count++;
            }

            list.insert(cur,p);
        }
        return count;
    }

    template <typename T>
    static int pushBack(List<T>& list, Process* p){
        int count = list.size();
        list.push_back(p);
        return count;
    }
};

#endif
