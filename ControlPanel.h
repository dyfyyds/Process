#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include "SchedulingStrategy.h"

class ControlPanel : public QWidget {
    Q_OBJECT
public:
    explicit ControlPanel(QWidget* parent = nullptr);

    QString processName() const;
    int processPriority() const;
    int processTime() const;
    Algorithm selectedAlgorithm() const;
    int timeQuantum() const;
    int speedMs() const;

    void setRunning(bool running);
    void setAutoRunning(bool autoRunning);
    void clearInputs();
    void setControlsEnabled(bool enabled);

signals:
    void addProcessClicked();
    void randomGenerateClicked();
    void singleStepClicked();
    void autoRunClicked();
    void resetClicked();
    void compareClicked();
    void algorithmChanged(Algorithm algo);
    void timeQuantumChanged(int q);
    void speedChanged(int ms);

private:
    void setupUI();
    void onAlgorithmChanged(int index);

    QComboBox* m_algoCombo;
    QSpinBox* m_quantumSpin;
    QLabel* m_quantumLabel;
    QLineEdit* m_nameEdit;
    QSpinBox* m_prioritySpin;
    QSpinBox* m_timeSpin;
    QPushButton* m_addBtn;
    QPushButton* m_randomBtn;
    QSlider* m_speedSlider;
    QLabel* m_speedLabel;
    QPushButton* m_singleBtn;
    QPushButton* m_autoBtn;
    QPushButton* m_resetBtn;
    QPushButton* m_compareBtn;
    QLabel* m_priorityLabel;
};

#endif
