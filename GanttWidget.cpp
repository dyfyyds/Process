#include "GanttWidget.h"
#include "CyberStyle.h"
#include <QPainter>
#include <QPainterPath>
#include <QScrollBar>
#include <QPropertyAnimation>

GanttWidget::GanttWidget(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(140);
    
    m_scanAnim = new QPropertyAnimation(this, "scanLineX", this);
    m_scanAnim->setDuration(3000);
    m_scanAnim->setStartValue(0.0);
    m_scanAnim->setEndValue(1.0);
    m_scanAnim->setLoopCount(-1);
    m_scanAnim->start();
}

void GanttWidget::setGanttData(const std::vector<GanttEntry>& data, int currentTick) {
    m_data = data;
    m_currentTick = currentTick;
    int neededWidth = (currentTick + 2) * m_cellWidth + 60;
    if(neededWidth > width())
        setMinimumWidth(neededWidth);
    update();
}

void GanttWidget::clear() {
    m_data.clear();
    m_currentTick = 0;
    setMinimumWidth(0);
    update();
}

void GanttWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // 背景渐变
    QLinearGradient bgGrad(rect().topLeft(), rect().bottomLeft());
    bgGrad.setColorAt(0, QColor(Cyber::BG_PANEL));
    bgGrad.setColorAt(1, QColor(Cyber::BG_DEEP));
    p.setPen(Qt::NoPen);
    p.setBrush(bgGrad);
    p.drawRoundedRect(rect(), 8, 8);

    // 多层发光边框
    for(int i = 0; i < 3; i++) {
        QColor borderGlow = QColor(Cyber::CYAN);
        borderGlow.setAlphaF(0.25 - i*0.08);
        p.setPen(QPen(borderGlow, 3 - i));
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(rect().adjusted(i, i, -i, -i), 8 - i*0.5, 8 - i*0.5);
    }

    // 标题
    p.setPen(QColor(Cyber::CYAN));
    QFont titleFont("Consolas", 12, QFont::Bold);
    p.setFont(titleFont);
    p.drawText(15, 25, QString::fromUtf8("⚡ 甘特图 (Gantt Chart)"));

    if(m_data.empty()) {
        p.setPen(QColor(Cyber::TEXT_DIM));
        QFont emptyFont("Consolas", 13);
        p.setFont(emptyFont);
        p.drawText(rect(), Qt::AlignCenter, QString::fromUtf8("暂无执行记录"));
        return;
    }

    int offsetX = 20;
    int offsetY = 40;
    int barHeight = m_cellHeight;

    // 背景网格
    p.setPen(QPen(QColor(Cyber::BORDER), 1, Qt::DotLine));
    for(int t = 0; t <= m_currentTick + 2; t++) {
        int x = offsetX + t * m_cellWidth;
        p.drawLine(x, offsetY, x, offsetY + barHeight + 20);
    }

    // 时间轴刻度
    QFont tickFont("Consolas", 9);
    p.setFont(tickFont);
    p.setPen(QColor(Cyber::CYAN_DARK));

    for(int t = 0; t <= m_currentTick; t++) {
        int x = offsetX + t * m_cellWidth;
        p.drawText(x - 5, offsetY + barHeight + 18, QString::number(t));
    }

    // 绘制甘特图块 - 带霓虹轨迹效果
    for(const auto& entry : m_data) {
        int x1 = offsetX + entry.startTick * m_cellWidth;
        int x2 = offsetX + entry.endTick * m_cellWidth;
        int w = x2 - x1;

        // 霓虹发光效果
        for(int i = 0; i < 3; i++) {
            QColor glowColor = entry.color;
            glowColor.setAlphaF((0.25 - i*0.08));
            p.setPen(Qt::NoPen);
            p.setBrush(glowColor);
            p.drawRoundedRect(x1 + i, offsetY + 4 - i, w - i*2, barHeight - 8 + i*2, 4 - i*0.5, 4 - i*0.5);
        }

        // 主体彩色块
        QLinearGradient blockGrad(x1, offsetY, x2, offsetY);
        blockGrad.setColorAt(0, entry.color.darker(120));
        blockGrad.setColorAt(0.5, entry.color);
        blockGrad.setColorAt(1, entry.color.lighter(130));
        p.setBrush(blockGrad);
        p.drawRoundedRect(x1 + 2, offsetY + 4, w - 4, barHeight - 8, 4, 4);

        // 进程名标签
        if(w > 25) {
            p.setPen(QColor(Cyber::TEXT_BRIGHT));
            QFont labelFont("Consolas", 9, QFont::Bold);
            p.setFont(labelFont);
            p.drawText(QRect(x1 + 3, offsetY + 4, w - 6, barHeight - 8),
                       Qt::AlignCenter, entry.name);
        }
    }

    // 当前 tick 标记线 - 带扫描效果
    int curX = offsetX + m_currentTick * m_cellWidth;
    
    // 扫描线
    int scanX = rect().x() + static_cast<int>(rect().width() * m_scanLineX);
    QLinearGradient scanGrad(scanX - 30, 0, scanX + 30, 0);
    scanGrad.setColorAt(0, Qt::transparent);
    QColor scanColor(Cyber::CYAN);
    scanColor.setAlphaF(0.15);
    scanGrad.setColorAt(0.5, scanColor);
    scanGrad.setColorAt(1, Qt::transparent);
    p.setPen(Qt::NoPen);
    p.setBrush(scanGrad);
    p.drawRect(rect());

    // 当前 tick 发光线
    QColor cursorGlow = QColor(Cyber::CYAN);
    cursorGlow.setAlphaF(0.4);
    p.setPen(QPen(cursorGlow, 4));
    p.drawLine(curX, offsetY - 8, curX, offsetY + barHeight + 8);
    
    p.setPen(QPen(QColor(Cyber::CYAN), 2));
    p.drawLine(curX, offsetY - 8, curX, offsetY + barHeight + 8);

    // 三角标记
    QPainterPath triangle;
    triangle.moveTo(curX - 6, offsetY - 8);
    triangle.lineTo(curX + 6, offsetY - 8);
    triangle.lineTo(curX, offsetY - 2);
    triangle.closeSubpath();
    p.setBrush(QColor(Cyber::CYAN));
    p.setPen(Qt::NoPen);
    p.drawPath(triangle);
}
