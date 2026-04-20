#ifndef PROCESSCARD_H
#define PROCESSCARD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include "Process.h"

/**
 * @brief 进程卡片组件 - 用于在UI中可视化展示单个进程信息
 *
 * 该类继承自QWidget，负责：
 * - 展示进程基本信息（名称、优先级、剩余时间、状态）
 * - 提供视觉效果（扫描线动画、脉冲效果、阴影）
 * - 处理用户交互（悬停效果、删除信号）
 *
 * 主要设计特点：
 * - 使用Q_PROPERTY支持属性动画
 * - 自定义绘制实现视觉效果
 * - 响应式UI设计（悬停状态变化）
 */
class ProcessCard : public QWidget {
    Q_OBJECT
    // 扫描线位置的属性定义，支持动画
    Q_PROPERTY(qreal scanLinePos READ scanLinePos WRITE setScanLinePos)
    // 脉冲强度的属性定义，支持动画
    Q_PROPERTY(qreal pulseIntensity READ pulseIntensity WRITE setPulseIntensity)

public:
    /**
     * @brief 构造函数
     * @param process 关联的进程对象指针
     * @param parent 父控件指针
     */
    explicit ProcessCard(Process* process, QWidget* parent = nullptr);

    /**
     * @brief 从进程对象更新卡片显示内容
     * 当进程状态变化时调用此方法刷新UI
     */
    void updateFromProcess();

    /**
     * @brief 获取关联的进程对象
     * @return 进程指针
     */
    Process* process() const { return m_process; }

    /**
     * @brief 获取进程名称
     * @return 进程名字符串
     */
    QString processName() const;

    // 扫描线位置属性访问器
    qreal scanLinePos() const { return m_scanLinePos; }
    void setScanLinePos(qreal pos) { m_scanLinePos = pos; update(); }

    // 脉冲强度属性访问器
    qreal pulseIntensity() const { return m_pulseIntensity; }
    void setPulseIntensity(qreal val) { m_pulseIntensity = val; update(); }

signals:
    /**
     * @brief 删除按钮点击信号
     * @param card 被点击的进程卡片指针
     */
    void deleteClicked(ProcessCard* card);

protected:
    /**
     * @brief 绘制事件 - 实现自定义视觉效果
     * 包括背景、边框、扫描线和脉冲效果
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief 鼠标进入事件 - 触发悬停效果
     */
    void enterEvent(QEvent* event);

    /**
     * @brief 鼠标离开事件 - 清除悬停效果
     */
    void leaveEvent(QEvent* event) override;

private:
    Process* m_process;                     // 关联的进程数据模型
    QLabel* m_nameLabel;                    // 进程名称标签
    QLabel* m_priorityLabel;                // 优先级标签
    QLabel* m_timeLabel;                    // 剩余时间标签
    QLabel* m_stateLabel;                   // 状态标签
    bool m_hovered = false;                 // 悬停状态标志
    qreal m_scanLinePos = 0.0;              // 扫描线当前位置
    qreal m_pulseIntensity = 0.3;           // 脉冲效果强度
    QPropertyAnimation* m_scanAnim;         // 扫描线动画对象
    QPropertyAnimation* m_pulseAnim;        // 脉冲动画对象
    void drawCardBackground(QPainter& p, const QColor& borderColor);
    void drawEnergyBar(QPainter& p, const QColor& borderColor);
    void drawGrid(QPainter& p);
    void drawProgressBar(QPainter& p, const QColor& borderColor);
    void drawScanOverlay(QPainter& p, const QColor& borderColor);
};

#endif
