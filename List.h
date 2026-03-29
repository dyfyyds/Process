#ifndef LIST_H
#define LIST_H

#include <iostream>

/**
 * @brief 手写一个链表,c++有高度封装的list的库可以直接使用
 *
 * @tparam T 自定义类型  可以是普通类型，也可以是一个结构体(类)
 */
template <typename T>
class List{
public:
    struct Node{
        T data;
        Node* next;
        Node(T d) : data(d), next(nullptr){}
    };

private:
    Node* m_head;
    int   m_size;

    /**
     * @brief 清除所有节点
     *
     */
    void clear(){
        while(this->m_head){
            Node* temp = this->m_head;
            this->m_head = this->m_head->next;
            delete temp;
        }
        this->m_size = 0;
    }

public:
    List(Node* head):m_head(head),m_size(head ? 1 : 0){}
    List():m_head(nullptr),m_size(0){}

    ~List(){
        clear();
    }

    bool empty() const{
        return this->m_head == nullptr;
    }

    int size() const{
        return this->m_size;
    }

    T front(){
        if(this->m_head)  return this->m_head->data;
        return nullptr;
    }

    Node* getHead() const{
        return this->m_head;
    }

    /**
     * @brief 头部插入数据
     */
    void push_front(T data){
        Node* newData = new Node(data);
        newData->next = this->m_head;
        this->m_head = newData;
        this->m_size++;
    }

    /**
     * @brief 尾部插入数据
     */
    void push_back(T data){
        Node* newData = new Node(data);
        if(!this->m_head){
            this->m_head = newData;
        } else {
            Node* cur = this->m_head;
            while(cur->next)
                cur = cur->next;
            cur->next = newData;
        }
        this->m_size++;
    }

    /**
     * @brief 在任意节点后插入数据
     */
    void insert(Node* preNode,T data){
        Node* newData = new Node(data);

        newData->next = preNode->next;
        preNode->next = newData;

        this->m_size++;
    }

    /**
     * @brief 移除头结点
     */
    void pop_front(){
        if (this->m_head){
            Node* temp = this->m_head;
            this->m_head = this->m_head->next;
            delete temp;
            this->m_size--;
        }
    }

    /**
     * @brief 设置头结点
     *
     * @details 排序后重新加入链表
     */
    void setHead(Node* node){

        clear();
        this->m_head = node;

        Node* cur = m_head;
        int count = 0;
        while(cur){
            count++;
            cur = cur->next;
        }

        this->m_size = count;
    }

};

#endif // LIST_H
