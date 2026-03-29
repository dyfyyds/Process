#ifndef PROCESSCARD_H
#define PROCESSCARD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include "Process.h"

class ProcessCard : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal scanLinePos READ scanLinePos WRITE setScanLinePos)
    Q_PROPERTY(qreal pulseIntensity READ pulseIntensity WRITE setPulseIntensity)
public:
    explicit ProcessCard(Process* process, QWidget* parent = nullptr);
    void updateFromProcess();
    Process* process() const { return m_process; }
    QString processName() const;

    qreal scanLinePos() const { return m_scanLinePos; }
    void setScanLinePos(qreal pos) { m_scanLinePos = pos; update(); }
    qreal pulseIntensity() const { return m_pulseIntensity; }
    void setPulseIntensity(qreal val) { m_pulseIntensity = val; update(); }

signals:
    void deleteClicked(ProcessCard* card);

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event) override;

private:
    Process* m_process;
    QLabel* m_nameLabel;
    QLabel* m_priorityLabel;
    QLabel* m_timeLabel;
    QLabel* m_stateLabel;
    bool m_hovered = false;
    qreal m_scanLinePos = 0.0;
    qreal m_pulseIntensity = 0.3;
    QPropertyAnimation* m_scanAnim;
    QPropertyAnimation* m_pulseAnim;
};

#endif
