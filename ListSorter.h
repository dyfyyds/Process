#ifndef LISTSORTER_H
#define LISTSORTER_H

#include "List.h"
#include "Process.h"
#include <QDebug>

class ListSorter{
public:
    // template <typename T>
    // static void insertByPriority(List<T>& list, Process* p){
    //     auto head = list.getHead();

    //     if(!head || p->getPriority() >= head->data->getPriority())
    //         list.push_front(p);
    //     else{
    //         auto cur = head;

    //         while(cur->next && cur->next->data->getPriority() > p->getPriority())
    //             cur = cur->next;

    //         list.insert(cur,p);
    //     }
    // }

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
};


#endif
