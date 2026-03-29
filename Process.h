#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <QColor>

class Process{
private:
    std::string m_name;
    char        m_state;        // W(就绪) R(运行) F(完成)
    int         m_priority;
    int         m_ntime;        // 需要的总时间
    int         m_rtime;        // 已经运行的时间

    int         m_originalPriority;
    int         m_arrivalTime;
    int         m_waitTime;
    int         m_startTime;    // 首次运行时的tick (-1未运行)
    int         m_finishTime;
    QColor      m_color;

public:
    Process(const std::string& name, int priority, int ntime);
    Process(const std::string& name, int priority, int rtime, int ntime);
    Process();
    ~Process() = default;

    bool setRunning();
    bool isFinish() const;
    void execute();
    void updateState();

    // 优先级调度专用
    void decreasePriority();

    // 统计
    void incrementWaitTime();
    void setArrivalTime(int tick);
    void setStartTime(int tick);
    void setFinishTime(int tick);
    void setColor(const QColor& color);

    // 克隆(用于算法对比)
    Process* clone() const;

    // Getters
    int getPriority() const;
    int getOriginalPriority() const;
    std::string getName() const;
    int getNTime() const;
    int getRTime() const;
    int getRemainingTime() const;
    char getState() const;
    int getArrivalTime() const;
    int getWaitTime() const;
    int getStartTime() const;
    int getFinishTime() const;
    int getTurnaroundTime() const;
    int getResponseTime() const;
    QColor getColor() const;
};

#endif // PROCESS_H
