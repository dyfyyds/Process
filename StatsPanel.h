#ifndef STATSPANEL_H
#define STATSPANEL_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QTableWidget>
#include "ScheDuler.h"

class QVBoxLayout;

/**
 * @brief 统计信息面板组件 - 实时展示调度器运行统计数据
 *
 * 该类继承自QWidget，负责：
 * - 实时显示调度器运行的关键性能指标
 * - 展示CPU利用率和各种平均时间统计
 * - 显示就绪队列和已完成进程数量
 * - 提供已完成进程详细信息的表格视图
 *
 * 主要设计特点：
 * - 多维度统计展示（时间、利用率、进程数量）
 * - 进度条可视化CPU利用率
 * - 表格形式展示详细的进程完成信息
 * - 与Scheduler类无缝集成，自动更新数据
 *
 * @see Scheduler
 * @see Process
 */
class StatsPanel : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针
     *
     * 创建统计面板实例，初始化UI组件和布局
     * @note 需要通过updateStats()方法更新数据
     */
    explicit StatsPanel(QWidget* parent = nullptr);

    /**
     * @brief 更新统计数据
     * @param scheduler 调度器指针，从中获取统计数据
     *
     * 从调度器获取最新统计数据并更新UI显示，包括：
     * - 当前调度算法名称
     * - 时钟周期计数
     * - 平均等待/周转/响应时间
     * - CPU利用率（进度条和标签）
     * - 就绪队列和已完成进程数量
     * - 已完成进程详细表格
     *
     * @note 每次调用都会完全刷新面板数据
     * @see Scheduler
     */
    void updateStats(Scheduler* scheduler);

    /**
     * @brief 清空统计数据
     *
     * 重置所有统计显示到初始状态，包括：
     * - 清空所有标签文本
     * - 重置进度条到0
     * - 清空已完成进程表格
     *
     * @note 通常在调度器重置时调用
     */
    void clear();

private:
    /**
     * @brief 设置和初始化UI组件
     *
     * 创建所有子控件并建立布局，包括：
     * - 标签控件用于显示各种统计数据
     * - CPU利用率进度条
     * - 进程信息表格
     * - 布局管理器组织UI元素
     *
     * @note 仅在构造函数中调用一次
     */
    void setupUI();
    void buildHeaderSection(QVBoxLayout* layout);
    void buildMetricsSection(QVBoxLayout* layout);
    void buildCountSection(QVBoxLayout* layout);
    void buildFinishedTableSection(QVBoxLayout* layout);
    void updateFinishedTable(const std::vector<Process*>& finished);
    QLabel* createSeparator();
    QLabel* createStatLabel();
    QLabel* createValueLabel();

    // 调度算法显示标签
    QLabel* m_algoLabel;
    // 当前时钟周期显示标签
    QLabel* m_tickLabel;
    // 平均等待时间显示标签
    QLabel* m_waitLabel;
    // 平均周转时间显示标签
    QLabel* m_turnaroundLabel;
    // 平均响应时间显示标签
    QLabel* m_responseLabel;
    // CPU利用率百分比标签
    QLabel* m_cpuUtilLabel;
    // CPU利用率进度条（可视化显示）
    QProgressBar* m_cpuBar;
    // 就绪队列进程数量标签
    QLabel* m_readyCountLabel;
    // 已完成进程数量标签
    QLabel* m_finishedCountLabel;
    // 已完成进程详细信息表格
    QTableWidget* m_finishedTable;
};

#endif
