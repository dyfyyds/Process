#ifndef LISTSORTER_H
#define LISTSORTER_H

#include "List.h"
#include "Process.h"

class ListSorter{
public:
    template <typename T>
    static void insertByPriority(List<T>& list, Process* p){
        auto head = list.getHead();

        if(!head || p->getPriority() >= head->data->getPriority())
            list.push_front(p);
        else{
            auto cur = head;

            while(cur->next && cur->next->data->getPriority() > p->getPriority())
                cur = cur->next;

            list.insert(cur,p);
        }
    }
};


#endif
