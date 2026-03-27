#ifndef PROCESS_H
#define PROCESS_H


#include <string>

/**
 * @brief 负责管理一个进程的行为
 *
 */
class Process{
private:
    std::string m_name;         //进程名
    char        m_state;        //状态 W  R  F (就绪，运行，完成)
    int         m_priority;     //优先级
    int         m_ntime;        //需要的总时间
    int         m_rtime;        //已经运行的时间

public:
    /**
     * @brief 构造函数
     *
     * @param name      进程名
     * @param priority  优先级
     * @param ntime     需要运行的时间
     */
    Process(const std::string& name,int priority,int ntime);
    Process(const std::string& name,int priority,int rtime, int ntime);
    Process();

    /**
     * @brief 默认析构函数
     *
     */
    ~Process()=default;

    /**
     * @brief   调整到运行状态
     *
     * @details 判断是否满足条件(处于就绪态且运行时间没有超出总时间)
     *
     * @return true
     * @return false
     */
    bool setRunning();

    /**
     * @brief 判断进程是否运行完成
     *
     * @return true
     * @return false
     */
    bool isFinish();

    /**
     * @brief 执行一次进程
     * @details 运行一次，时间+1，优先级-1
     */
    void execute();

    /**
     * @brief 更新状态
     *
     */
    void updateState();

    /**
     * @brief 对外接口
     *
     */
    int getPriority() const;
    std::string getName() const;
    int getNTime() const;
    int getRTime() const;
    char getState() const;
};


#endif // PROCESS_H
