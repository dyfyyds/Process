#include "ProcessCard.h"
#include "CyberStyle.h"
#include <QPushButton>
#include <QPainterPath>

ProcessCard::ProcessCard(Process* process, QWidget* parent)
    : QWidget(parent), m_process(process)
{
    setFixedSize(140, 120);
    setCursor(Qt::PointingHandCursor);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 10, 12, 10);
    layout->setSpacing(4);

    m_nameLabel = new QLabel(this);
    m_nameLabel->setStyleSheet(QString("color: %1; font-size: 15px; font-weight: bold; "
        "background: transparent; text-shadow: 0 0 4px %1;")
        .arg(m_process->getColor().name()));
    layout->addWidget(m_nameLabel);

    m_stateLabel = new QLabel(this);
    m_stateLabel->setStyleSheet(QString("color: %1; font-size: 11px; background: transparent; "
        "font-weight: bold;").arg(Cyber::GREEN));
    layout->addWidget(m_stateLabel);

    m_priorityLabel = new QLabel(this);
    m_priorityLabel->setStyleSheet(QString("color: %1; font-size: 10px; background: transparent;")
        .arg(Cyber::TEXT_NEON));
    layout->addWidget(m_priorityLabel);

    m_timeLabel = new QLabel(this);
    m_timeLabel->setStyleSheet(QString("color: %1; font-size: 10px; background: transparent; "
        "font-family: 'Consolas';").arg(Cyber::CYAN));
    layout->addWidget(m_timeLabel);

    layout->addStretch();
    updateFromProcess();

    m_scanAnim = new QPropertyAnimation(this, "scanLinePos", this);
    m_scanAnim->setDuration(2500);
    m_scanAnim->setStartValue(0.0);
    m_scanAnim->setEndValue(1.0);
    m_scanAnim->setLoopCount(-1);
    m_scanAnim->start();

    m_pulseAnim = new QPropertyAnimation(this, "pulseIntensity", this);
    m_pulseAnim->setDuration(1800);
    m_pulseAnim->setStartValue(0.2);
    m_pulseAnim->setEndValue(0.6);
    m_pulseAnim->setLoopCount(-1);
    m_pulseAnim->setEasingCurve(QEasingCurve::InOutSine);
    m_pulseAnim->start();
}

void ProcessCard::updateFromProcess() {
    if(!m_process) return;
    m_nameLabel->setText(QString::fromStdString(m_process->getName()));
    QString stateText;
    switch(m_process->getState()) {
        case 'W': stateText = QString::fromUtf8("就绪"); break;
        case 'R': stateText = QString::fromUtf8("运行"); break;
        case 'F': stateText = QString::fromUtf8("完成"); break;
        default: stateText = "?";
    }
    m_stateLabel->setText(stateText);
    m_priorityLabel->setText(QString::fromUtf8("优先级: %1").arg(m_process->getPriority()));
    m_timeLabel->setText(QString("%1/%2").arg(m_process->getRTime()).arg(m_process->getNTime()));
}

QString ProcessCard::processName() const {
    return m_process ? QString::fromStdString(m_process->getName()) : QString();
}

void ProcessCard::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QColor borderColor = m_process ? m_process->getColor() : QColor(Cyber::CYAN);
    QColor glowColor = borderColor;
    glowColor.setAlphaF(m_pulseIntensity * 0.5);
    
    QColor bgColor = m_hovered ? QColor(Cyber::BG_CARD).lighter(115) : QColor(Cyber::BG_DEEP);

    p.setPen(Qt::NoPen);
    p.setBrush(bgColor);
    p.drawRoundedRect(rect().adjusted(1,1,-1,-1), 6, 6);

    // 多层发光边框
    for(int i = 0; i < 3; i++) {
        QColor borderGlow = borderColor;
        borderGlow.setAlphaF((0.3 - i*0.08) * m_pulseIntensity);
        p.setPen(QPen(borderGlow, 3 - i*0.7));
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(rect().adjusted(2+i,2+i,-2-i,-2-i), 6-i*0.5, 6-i*0.5);
    }

    // 内边框
    p.setPen(QPen(borderColor, 1));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rect().adjusted(2,2,-2,-2), 6, 6);

    // 左侧彩色能量条
    QLinearGradient leftGrad(0, 4, 5, 4);
    leftGrad.setColorAt(0, borderColor);
    leftGrad.setColorAt(1, borderColor.lighter(140));
    p.setPen(Qt::NoPen);
    p.setBrush(leftGrad);
    p.drawRoundedRect(2, 4, 5, height()-8, 2, 2);

    // 背景网格线
    p.setPen(QPen(QColor(Cyber::BORDER), 1, Qt::DotLine));
    for(int y = 10; y < height()-20; y += 15) {
        p.drawLine(10, y, width()-10, y);
    }

    // 底部进度条
    if(m_process && m_process->getNTime() > 0) {
        double progress = (double)m_process->getRTime() / m_process->getNTime();
        int barY = height() - 12;
        int barW = width() - 24;
        int barH = 6;
        
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(Cyber::BG_PANEL));
        p.drawRoundedRect(12, barY, barW, barH, 3, 3);
        
        QLinearGradient barGrad(12, 0, 12 + barW * progress, 0);
        barGrad.setColorAt(0, borderColor.darker(120));
        barGrad.setColorAt(1, borderColor);
        p.setBrush(barGrad);
        p.drawRoundedRect(12, barY, barW * progress, barH, 3, 3);
        
        p.setPen(QColor(Cyber::TEXT_BRIGHT));
        QFont barFont("Consolas", 8, QFont::Bold);
        p.setFont(barFont);
        p.drawText(QRect(12, barY - 2, barW, barH), Qt::AlignCenter,
                   QString("%1/%2").arg(m_process->getRTime()).arg(m_process->getNTime()));
    }

    // 扫描线效果
    int scanY = 5 + (height() - 10) * m_scanLinePos;
    QLinearGradient scanGrad(0, scanY - 10, 0, scanY + 10);
    scanGrad.setColorAt(0, Qt::transparent);
    QColor scanColor = borderColor;
    scanColor.setAlphaF(0.15);
    scanGrad.setColorAt(0.5, scanColor);
    scanGrad.setColorAt(1, Qt::transparent);
    p.setPen(Qt::NoPen);
    p.setBrush(scanGrad);
    
    QPainterPath clipPath;
    clipPath.addRoundedRect(rect().adjusted(2,2,-2,-2), 6, 6);
    p.setClipPath(clipPath);
    p.drawRect(2, scanY - 10, width()-4, 20);
    p.setClipping(false);
}

void ProcessCard::enterEvent(QEvent*) {
    m_hovered = true;
    update();
}

void ProcessCard::leaveEvent(QEvent*) {
    m_hovered = false;
    update();
}
