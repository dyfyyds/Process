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

void GanttWidget::drawBackground(QPainter& p) {
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
}

void GanttWidget::drawTitle(QPainter& p) {
    p.setPen(QColor(Cyber::CYAN));
    QFont titleFont("Consolas", 12, QFont::Bold);
    p.setFont(titleFont);
    p.drawText(15, 25, QString::fromUtf8("⚡ 甘特图 (Gantt Chart)"));
}

void GanttWidget::drawEmptyState(QPainter& p) {
    p.setPen(QColor(Cyber::TEXT_DIM));
    p.setFont(QFont("Consolas", 13));
    p.drawText(rect(), Qt::AlignCenter, QString::fromUtf8("暂无执行记录"));
}

void GanttWidget::drawGrid(QPainter& p, int offsetX, int offsetY, int barHeight) {
    p.setPen(QPen(QColor(Cyber::BORDER), 1, Qt::DotLine));
    for(int t = 0; t <= m_currentTick + 2; t++) {
        int x = offsetX + t * m_cellWidth;
        p.drawLine(x, offsetY, x, offsetY + barHeight + 20);
    }
}

void GanttWidget::drawTimeline(QPainter& p, int offsetX, int offsetY, int barHeight) {
    p.setFont(QFont("Consolas", 9));
    p.setPen(QColor(Cyber::CYAN_DARK));
    for(int t = 0; t <= m_currentTick; t++) {
        int x = offsetX + t * m_cellWidth;
        p.drawText(x - 5, offsetY + barHeight + 18, QString::number(t));
    }
}

void GanttWidget::drawEntry(QPainter& p, const GanttEntry& entry, int offsetX, int offsetY, int barHeight) {
    const int x1 = offsetX + entry.startTick * m_cellWidth;
    const int x2 = offsetX + entry.endTick * m_cellWidth;
    const int width = x2 - x1;

    for(int i = 0; i < 3; i++) {
        QColor glowColor = entry.color;
        glowColor.setAlphaF(0.25 - i*0.08);
        p.setPen(Qt::NoPen);
        p.setBrush(glowColor);
        p.drawRoundedRect(x1 + i, offsetY + 4 - i, width - i*2, barHeight - 8 + i*2, 4 - i*0.5, 4 - i*0.5);
    }

    QLinearGradient blockGrad(x1, offsetY, x2, offsetY);
    blockGrad.setColorAt(0, entry.color.darker(120));
    blockGrad.setColorAt(0.5, entry.color);
    blockGrad.setColorAt(1, entry.color.lighter(130));
    p.setBrush(blockGrad);
    p.drawRoundedRect(x1 + 2, offsetY + 4, width - 4, barHeight - 8, 4, 4);

    if(width > 25) {
        p.setPen(QColor(Cyber::TEXT_BRIGHT));
        p.setFont(QFont("Consolas", 9, QFont::Bold));
        p.drawText(QRect(x1 + 3, offsetY + 4, width - 6, barHeight - 8), Qt::AlignCenter, entry.name);
    }
}

void GanttWidget::drawEntries(QPainter& p, int offsetX, int offsetY, int barHeight) {
    for(const auto& entry : m_data) {
        drawEntry(p, entry, offsetX, offsetY, barHeight);
    }
}

void GanttWidget::drawScanOverlay(QPainter& p) {
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
}

void GanttWidget::drawCurrentTickMarker(QPainter& p, int offsetX, int offsetY, int barHeight) {
    const int curX = offsetX + m_currentTick * m_cellWidth;
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

void GanttWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    drawBackground(p);
    drawTitle(p);
    if(m_data.empty()) {
        drawEmptyState(p);
        return;
    }

    const int offsetX = 20;
    const int offsetY = 40;
    const int barHeight = m_cellHeight;

    drawGrid(p, offsetX, offsetY, barHeight);
    drawTimeline(p, offsetX, offsetY, barHeight);
    drawEntries(p, offsetX, offsetY, barHeight);
    drawScanOverlay(p);
    drawCurrentTickMarker(p, offsetX, offsetY, barHeight);
}
