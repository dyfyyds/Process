#ifndef CPUWIDGET_H
#define CPUWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include <vector>
#include "Process.h"

/**
 * @brief 粒子结构体 - 用于视觉效果中的粒子动画
 *
 * 表示在CPU核心周围漂浮的粒子效果，包括：
 * - 位置和速度信息
 * - 生命周期管理
 * - 颜色属性
 *
 * 用于增强UI的视觉表现力和动态效果
 */
struct Particle {
    qreal x, y;      // 粒子当前位置坐标
    qreal vx, vy;    // 粒子速度分量
    qreal life;      // 生命周期（0.0到1.0）
    QColor color;    // 粒子颜色
};

/**
 * @brief CPU核心可视化组件 - 展示当前运行的进程和CPU状态
 *
 * 该类继承自QWidget，负责：
 * - 可视化展示当前运行的进程信息
 * - 提供CPU核心动画效果（扫描线、发光、能量流动）
 * - 粒子系统增强视觉体验
 * - 实时显示CPU时钟周期信息
 *
 * 主要设计特点：
 * - 多属性动画支持（Q_PROPERTY）
 * - 自定义绘制实现复杂视觉效果
 * - 粒子系统模拟动态效果
 * - 响应式UI更新机制
 */
class CpuWidget : public QWidget {
    Q_OBJECT
    // 扫描线位置属性，用于从上到下的扫描动画
    Q_PROPERTY(qreal scanLinePos READ scanLinePos WRITE setScanLinePos)
    // 发光强度属性，控制CPU核心的发光效果
    Q_PROPERTY(qreal glowIntensity READ glowIntensity WRITE setGlowIntensity)
    // 能量流动属性，展示CPU内部能量流动效果
    Q_PROPERTY(qreal energyFlow READ energyFlow WRITE setEnergyFlow)

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针
     */
    explicit CpuWidget(QWidget* parent = nullptr);

    /**
     * @brief 设置当前运行的进程
     * @param p 进程对象指针，nullptr表示无进程运行
     * 调用此方法后组件会更新显示并触发动画效果
     */
    void setProcess(Process* p);

    /**
     * @brief 清除当前运行的进程信息
     * 等价于setProcess(nullptr)
     */
    void clearProcess();

    /**
     * @brief 设置CPU时钟周期
     * @param tick 当前时钟周期数
     * 用于显示CPU的运行时间和调度信息
     */
    void setTick(int tick);

    // 扫描线位置属性访问器
    qreal scanLinePos() const { return m_scanLinePos; }
    void setScanLinePos(qreal pos) { m_scanLinePos = pos; update(); }

    // 发光强度属性访问器
    qreal glowIntensity() const { return m_glowIntensity; }
    void setGlowIntensity(qreal val) { m_glowIntensity = val; update(); }

    // 能量流动属性访问器
    qreal energyFlow() const { return m_energyFlow; }
    void setEnergyFlow(qreal val) { m_energyFlow = val; update(); }

protected:
    /**
     * @brief 绘制事件 - 实现所有视觉效果的自定义绘制
     * 包括：
     * - CPU核心形状绘制
     * - 扫描线动画
     * - 发光效果
     * - 粒子系统渲染
     * - 能量流动效果
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief 定时器事件 - 驱动动画和粒子系统更新
     * @param event 定时器事件对象
     * 每帧更新粒子位置和生命周期
     */
    void timerEvent(QTimerEvent* event) override;

private:
    Process* m_process = nullptr;           // 当前运行的进程指针
    int m_tick = 0;                         // CPU时钟周期计数
    qreal m_scanLinePos = 0.0;              // 扫描线当前位置
    qreal m_glowIntensity = 0.5;            // 发光强度
    qreal m_energyFlow = 0.0;               // 能量流动参数
    QPropertyAnimation* m_scanAnim;         // 扫描线动画对象
    QPropertyAnimation* m_glowAnim;         // 发光强度动画对象
    QPropertyAnimation* m_energyAnim;       // 能量流动动画对象
    std::vector<Particle> m_particles;      // 粒子系统容器
    int m_timerId = 0;                      // 定时器ID

    /**
     * @brief 更新粒子系统状态
     * 每帧调用，更新所有粒子的位置、速度和生命周期
     * 移除生命周期结束的粒子，添加新粒子
     */
    void updateParticles();

    /**
     * @brief 绘制CPU核心区域
     * @param p 画笔对象
     * @param area CPU核心绘制区域
     * 绘制CPU外框、内部结构和扫描线效果
     */
    void drawCpuCore(QPainter& p, const QRect& area);

    /**
     * @brief 绘制粒子系统
     * @param p 画笔对象
     * 渲染所有活跃粒子的视觉效果
     */
    void drawParticles(QPainter& p);

    /**
     * @brief 绘制能量流动效果
     * @param p 画笔对象
     * @param area 能量流动绘制区域
     * 创建从CPU核心向外流动的能量视觉效果
     */
    void drawEnergyFlow(QPainter& p, const QRect& area);
    void drawFrame(QPainter& p, const QRect& area);
    void drawHeader(QPainter& p, const QRect& area);
    void drawIdleState(QPainter& p, const QRect& area);
    void drawProcessInfo(QPainter& p, const QRect& area);
    void drawProgressBar(QPainter& p, const QRect& area, const QColor& procColor);
    void drawScanOverlay(QPainter& p, const QRect& area);
};

#endif
