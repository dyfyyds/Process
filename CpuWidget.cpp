#include "CpuWidget.h"
#include "CyberStyle.h"
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QRandomGenerator>

CpuWidget::CpuWidget(QWidget* parent) : QWidget(parent) {
    setFixedHeight(200);
    setMinimumWidth(320);

    m_scanAnim = new QPropertyAnimation(this, "scanLinePos", this);
    m_scanAnim->setDuration(2000);
    m_scanAnim->setStartValue(0.0);
    m_scanAnim->setEndValue(1.0);
    m_scanAnim->setLoopCount(-1);
    m_scanAnim->start();

    m_glowAnim = new QPropertyAnimation(this, "glowIntensity", this);
    m_glowAnim->setDuration(1500);
    m_glowAnim->setStartValue(0.3);
    m_glowAnim->setEndValue(1.0);
    m_glowAnim->setLoopCount(-1);
    m_glowAnim->setEasingCurve(QEasingCurve::InOutSine);
    m_glowAnim->start();

    m_energyAnim = new QPropertyAnimation(this, "energyFlow", this);
    m_energyAnim->setDuration(1200);
    m_energyAnim->setStartValue(0.0);
    m_energyAnim->setEndValue(2.0 * 3.14159);
    m_energyAnim->setLoopCount(-1);
    m_energyAnim->start();

    m_timerId = startTimer(50);
    
    for(int i = 0; i < 20; i++) {
        m_particles.push_back({
            QRandomGenerator::global()->bounded(100) / 100.0,
            QRandomGenerator::global()->bounded(100) / 100.0,
            (QRandomGenerator::global()->bounded(100) - 50) / 200.0,
            (QRandomGenerator::global()->bounded(100) - 50) / 200.0,
            QRandomGenerator::global()->bounded(100) / 100.0,
            QColor(Cyber::CYAN)
        });
    }
}

void CpuWidget::setProcess(Process* p) {
    m_process = p;
    update();
}

void CpuWidget::clearProcess() {
    m_process = nullptr;
    update();
}

void CpuWidget::setTick(int tick) {
    m_tick = tick;
    update();
}

void CpuWidget::timerEvent(QTimerEvent*) {
    updateParticles();
    update();
}

void CpuWidget::updateParticles() {
    for(auto& pt : m_particles) {
        pt.x += pt.vx;
        pt.y += pt.vy;
        pt.life -= 0.02;
        
        if(pt.x < 0 || pt.x > 1) pt.vx *= -1;
        if(pt.y < 0 || pt.y > 1) pt.vy *= -1;
        
        if(pt.life <= 0) {
            pt.x = QRandomGenerator::global()->bounded(100) / 100.0;
            pt.y = QRandomGenerator::global()->bounded(100) / 100.0;
            pt.life = 1.0;
            pt.color = m_process ? m_process->getColor() : QColor(Cyber::CYAN);
        }
    }
}

void CpuWidget::drawCpuCore(QPainter& p, const QRect& area) {
    QColor borderColor = m_process ? m_process->getColor() : QColor(Cyber::CYAN);
    
    // CPU 核心外框 - 多层发光
    for(int i = 0; i < 4; i++) {
        QColor glow = borderColor;
        glow.setAlphaF((0.4 - i*0.08) * m_glowIntensity);
        p.setPen(QPen(glow, 4 - i*0.8));
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(area.adjusted(i*2, i*2, -i*2, -i*2), 10 - i, 10 - i);
    }
    
    // 核心内部
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(Cyber::BG_DEEP));
    p.drawRoundedRect(area.adjusted(8, 8, -8, -8), 8, 8);
    
    // 能量环
    int ringRadius = qMin(area.width(), area.height()) / 2 - 20;
    QPoint center = area.center();
    
    for(int i = 0; i < 3; i++) {
        QColor ringColor = borderColor;
        ringColor.setAlphaF(0.3 - i*0.08);
        p.setPen(QPen(ringColor, 2 - i*0.5));
        p.setBrush(Qt::NoBrush);
        
        qreal angle = m_energyFlow + i * 0.5;
        int startAngle = static_cast<int>(angle * 16) % (360*16);
        p.drawArc(center.x() - ringRadius - i*8, center.y() - ringRadius - i*8,
                  (ringRadius + i*8)*2, (ringRadius + i*8)*2,
                  startAngle, 280*16);
    }
}

void CpuWidget::drawParticles(QPainter& p) {
    for(const auto& pt : m_particles) {
        QColor c = pt.color;
        c.setAlphaF(pt.life * 0.6);
        p.setPen(Qt::NoPen);
        p.setBrush(c);
        p.drawEllipse(static_cast<int>(pt.x * width()), 
                      static_cast<int>(pt.y * height()), 3, 3);
    }
}

void CpuWidget::drawEnergyFlow(QPainter& p, const QRect& area) {
    if(!m_process) return;
    
    QColor c = m_process->getColor();
    c.setAlphaF(0.15);
    
    // 能量流动效果
    for(int i = 0; i < 5; i++) {
        qreal offset = fmod(m_energyFlow + i * 0.3, 1.0);
        int y = area.y() + static_cast<int>(offset * area.height());
        
        QLinearGradient grad(0, y - 5, width(), y + 5);
        grad.setColorAt(0, Qt::transparent);
        grad.setColorAt(0.5, c);
        grad.setColorAt(1, Qt::transparent);
        
        p.setPen(Qt::NoPen);
        p.setBrush(grad);
        p.drawRect(0, y - 5, width(), 10);
    }
}

void CpuWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect area = rect().adjusted(2, 2, -2, -2);

    // 背景
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(Cyber::BG_PANEL));
    p.drawRoundedRect(area, 8, 8);

    // 边框发光
    QColor borderColor = m_process ? m_process->getColor() : QColor(Cyber::CYAN);
    QColor glowColor = borderColor;
    glowColor.setAlphaF(m_glowIntensity * 0.6);
    p.setPen(QPen(glowColor, 2));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(area, 8, 8);

    // 内边框
    p.setPen(QPen(borderColor.darker(180), 1));
    p.drawRoundedRect(area.adjusted(3,3,-3,-3), 6, 6);

    // 标题
    p.setPen(QColor(Cyber::CYAN));
    QFont titleFont("Consolas", 11, QFont::Bold);
    p.setFont(titleFont);
    p.drawText(area.adjusted(15, 8, 0, 0), Qt::AlignLeft | Qt::AlignTop,
               QString::fromUtf8("CPU 执行单元"));

    // Tick显示
    p.setPen(QColor(Cyber::TEXT_DIM));
    QFont tickFont("Consolas", 10);
    p.setFont(tickFont);
    p.drawText(area.adjusted(0, 8, -15, 0), Qt::AlignRight | Qt::AlignTop,
               QString("TICK: %1").arg(m_tick));

    if(!m_process) {
        // 空闲状态
        p.setPen(QColor(Cyber::TEXT_DIM));
        QFont idleFont("Consolas", 20, QFont::Bold);
        p.setFont(idleFont);
        QColor idleColor(Cyber::CYAN);
        idleColor.setAlphaF(m_glowIntensity * 0.5);
        p.setPen(idleColor);
        p.drawText(area, Qt::AlignCenter, "CPU IDLE");
    } else {
        // 运行中进程信息
        QColor procColor = m_process->getColor();
        int centerY = area.center().y();

        // 进程名(大字)
        QFont nameFont("Consolas", 22, QFont::Bold);
        p.setFont(nameFont);
        p.setPen(procColor);
        p.drawText(area.adjusted(20, 30, 0, 0), Qt::AlignLeft | Qt::AlignTop,
                   QString::fromStdString(m_process->getName()));

        // 状态信息
        QFont infoFont("Consolas", 12);
        p.setFont(infoFont);

        int infoX = 20;
        int infoY = centerY + 10;

        p.setPen(QColor(Cyber::TEXT));
        p.drawText(infoX, infoY, QString::fromUtf8("优先级: %1").arg(m_process->getPriority()));
        p.drawText(infoX, infoY + 20, QString::fromUtf8("状态: 运行中"));

        // 进度条
        int barX = area.width() / 2;
        int barY = centerY + 5;
        int barW = area.width() / 2 - 30;
        int barH = 14;

        p.setPen(Qt::NoPen);
        p.setBrush(QColor(Cyber::BG_DEEP));
        p.drawRoundedRect(barX, barY, barW, barH, 4, 4);

        double progress = 0;
        if(m_process->getNTime() > 0)
            progress = (double)m_process->getRTime() / m_process->getNTime();

        QLinearGradient grad(barX, 0, barX + barW * progress, 0);
        grad.setColorAt(0, procColor.darker(150));
        grad.setColorAt(1, procColor);
        p.setBrush(grad);
        p.drawRoundedRect(barX, barY, barW * progress, barH, 4, 4);

        p.setPen(QColor(Cyber::TEXT_BRIGHT));
        QFont barFont("Consolas", 9, QFont::Bold);
        p.setFont(barFont);
        p.drawText(QRect(barX, barY, barW, barH), Qt::AlignCenter,
                   QString("%1/%2").arg(m_process->getRTime()).arg(m_process->getNTime()));

        // 剩余时间
        p.setPen(QColor(Cyber::TEXT_DIM));
        p.drawText(barX, barY + barH + 16, QString::fromUtf8("剩余: %1").arg(m_process->getRemainingTime()));
    }

    // 扫描线效果
    int scanY = area.y() + (area.height() * m_scanLinePos);
    QLinearGradient scanGrad(0, scanY - 8, 0, scanY + 8);
    scanGrad.setColorAt(0, Qt::transparent);
    QColor scanColor(Cyber::CYAN);
    scanColor.setAlphaF(0.08);
    scanGrad.setColorAt(0.5, scanColor);
    scanGrad.setColorAt(1, Qt::transparent);
    p.setPen(Qt::NoPen);
    p.setBrush(scanGrad);

    QPainterPath clipPath;
    clipPath.addRoundedRect(area.adjusted(1,1,-1,-1), 8, 8);
    p.setClipPath(clipPath);
    p.drawRect(area.x(), scanY - 8, area.width(), 16);
}
