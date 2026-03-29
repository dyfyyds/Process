#ifndef CPUWIDGET_H
#define CPUWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include <vector>
#include "Process.h"

struct Particle {
    qreal x, y;
    qreal vx, vy;
    qreal life;
    QColor color;
};

class CpuWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal scanLinePos READ scanLinePos WRITE setScanLinePos)
    Q_PROPERTY(qreal glowIntensity READ glowIntensity WRITE setGlowIntensity)
    Q_PROPERTY(qreal energyFlow READ energyFlow WRITE setEnergyFlow)
public:
    explicit CpuWidget(QWidget* parent = nullptr);
    void setProcess(Process* p);
    void clearProcess();
    void setTick(int tick);

    qreal scanLinePos() const { return m_scanLinePos; }
    void setScanLinePos(qreal pos) { m_scanLinePos = pos; update(); }
    qreal glowIntensity() const { return m_glowIntensity; }
    void setGlowIntensity(qreal val) { m_glowIntensity = val; update(); }
    qreal energyFlow() const { return m_energyFlow; }
    void setEnergyFlow(qreal val) { m_energyFlow = val; update(); }

protected:
    void paintEvent(QPaintEvent* event) override;
    void timerEvent(QTimerEvent* event) override;

private:
    Process* m_process = nullptr;
    int m_tick = 0;
    qreal m_scanLinePos = 0.0;
    qreal m_glowIntensity = 0.5;
    qreal m_energyFlow = 0.0;
    QPropertyAnimation* m_scanAnim;
    QPropertyAnimation* m_glowAnim;
    QPropertyAnimation* m_energyAnim;
    std::vector<Particle> m_particles;
    int m_timerId = 0;
    
    void updateParticles();
    void drawCpuCore(QPainter& p, const QRect& area);
    void drawParticles(QPainter& p);
    void drawEnergyFlow(QPainter& p, const QRect& area);
};

#endif
