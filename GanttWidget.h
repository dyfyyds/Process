#ifndef GANTTWIDGET_H
#define GANTTWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <vector>
#include "ScheDuler.h"

/**
 * @brief 甘特图可视化组件 - 展示进程调度的时序图
 *
 * 该类继承自QWidget，负责：
 * - 可视化展示进程调度的甘特图
 * - 动态扫描线动画效果
 * - 响应式布局适应不同数据量
 * - 清晰展示进程执行时间区间
 *
 * 主要设计特点：
 * - 支持滚动查看大量数据
 * - 自定义绘制实现精确的甘特图展示
 * - 动画效果增强用户体验
 * - 与调度器数据无缝集成
 *
 * @see GanttEntry
 * @see Scheduler
 */
class GanttWidget : public QWidget {
    Q_OBJECT
    // 扫描线X坐标属性，用于垂直扫描动画效果
    Q_PROPERTY(qreal scanLineX READ scanLineX WRITE setScanLineX)

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针
     * 初始化甘特图组件，设置默认参数
     */
    explicit GanttWidget(QWidget* parent = nullptr);

    /**
     * @brief 设置甘特图数据
     * @param data 甘特图数据向量，包含进程执行时间区间信息
     * @param currentTick 当前时钟周期，用于高亮显示当前执行位置
     *
     * 此方法会更新组件数据并触发重绘
     * @see GanttEntry
     */
    void setGanttData(const std::vector<GanttEntry>& data, int currentTick);

    /**
     * @brief 清空甘特图数据
     *
     * 移除所有甘特图数据，恢复到初始状态
     */
    void clear();

    // 扫描线X坐标属性访问器
    qreal scanLineX() const { return m_scanLineX; }
    void setScanLineX(qreal val) { m_scanLineX = val; update(); }

protected:
    /**
     * @brief 绘制事件 - 实现甘特图的自定义绘制
     * @param event 绘制事件对象
     *
     * 绘制内容包括：
     * - 时间轴刻度
     * - 进程执行区间块
     * - 进程名称和颜色标识
     * - 扫描线效果
     */
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<GanttEntry> m_data;          // 甘特图数据容器
    int m_currentTick = 0;                   // 当前时钟周期
    int m_cellWidth = 40;                    // 每个时间单元格的宽度
    int m_cellHeight = 30;                   // 每个进程行的高度
    qreal m_scanLineX = 0.0;                 // 扫描线X坐标位置
    QPropertyAnimation* m_scanAnim;          // 扫描线动画对象
    void drawBackground(QPainter& p);
    void drawTitle(QPainter& p);
    void drawEmptyState(QPainter& p);
    void drawGrid(QPainter& p, int offsetX, int offsetY, int barHeight);
    void drawTimeline(QPainter& p, int offsetX, int offsetY, int barHeight);
    void drawEntries(QPainter& p, int offsetX, int offsetY, int barHeight);
    void drawEntry(QPainter& p, const GanttEntry& entry, int offsetX, int offsetY, int barHeight);
    void drawScanOverlay(QPainter& p);
    void drawCurrentTickMarker(QPainter& p, int offsetX, int offsetY, int barHeight);
};

#endif
