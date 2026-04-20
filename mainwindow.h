#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMouseEvent>
#include <QSet>
#include <memory>

#include "ScheDuler.h"
#include "ControlPanel.h"
#include "CpuWidget.h"
#include "ReadyQueueWidget.h"
#include "GanttWidget.h"
#include "StatsPanel.h"
#include "ExplanationBar.h"

class QWidget;
class QHBoxLayout;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    void setupUI();
    void applyWindowChrome();
    QWidget* createTitleBar(QWidget* parent);
    QHBoxLayout* createBodyLayout(QWidget* parent);
    QHBoxLayout* createBottomLayout(QWidget* parent);
    QPushButton* createWindowButton(const QString& iconPath, const QString& hoverStyle, QWidget* parent);
    void connectSignals();
    void onAddProcess();
    void onRandomGenerate();
    void onSingleStep();
    void onAutoRun();
    void onReset();
    void onCompare();
    void onAlgorithmChanged(Algorithm algo);
    void onSpeedChanged(int ms);
    void refreshUI();
    void executeStep();
    bool isExecutionComplete() const;
    void handleExecutionComplete();
    void updateExecutionViews(const StepResult& result);
    void updateStepControls();
    void rebuildProcessesForAlgorithmChange();

    std::shared_ptr<Scheduler> m_scheduler;
    QTimer* m_timer;
    bool m_autoRunning = false;
    bool m_isDrag = false;
    QPointF m_dragVal;
    QSet<QString> m_existNames;

    // UI组件
    ControlPanel* m_controlPanel;
    CpuWidget* m_cpuWidget;
    ReadyQueueWidget* m_readyQueue;
    GanttWidget* m_ganttWidget;
    QScrollArea* m_ganttScroll;
    StatsPanel* m_statsPanel;
    ExplanationBar* m_explanationBar;
};

#endif // MAINWINDOW_H
