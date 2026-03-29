#ifndef GANTTWIDGET_H
#define GANTTWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <vector>
#include "ScheDuler.h"

class GanttWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal scanLineX READ scanLineX WRITE setScanLineX)
public:
    explicit GanttWidget(QWidget* parent = nullptr);
    void setGanttData(const std::vector<GanttEntry>& data, int currentTick);
    void clear();

    qreal scanLineX() const { return m_scanLineX; }
    void setScanLineX(qreal val) { m_scanLineX = val; update(); }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<GanttEntry> m_data;
    int m_currentTick = 0;
    int m_cellWidth = 40;
    int m_cellHeight = 30;
    qreal m_scanLineX = 0.0;
    QPropertyAnimation* m_scanAnim;
};

#endif
