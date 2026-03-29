#include "ControlPanel.h"
#include "CyberStyle.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

ControlPanel::ControlPanel(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void ControlPanel::setupUI() {
    setFixedWidth(270);
    setStyleSheet(CyberStyle::panelStyle());

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(9);

    // 算法选择
    auto* algoTitle = new QLabel(QString::fromUtf8("⚙ 调度算法"), this);
    algoTitle->setStyleSheet(CyberStyle::sectionTitle());
    layout->addWidget(algoTitle);

    m_algoCombo = new QComboBox(this);
    m_algoCombo->addItem(QString::fromUtf8("⏱ 先来先服务 (FCFS)"));
    m_algoCombo->addItem(QString::fromUtf8("📊 短作业优先 (SJF)"));
    m_algoCombo->addItem(QString::fromUtf8("⭐ 优先级调度"));
    m_algoCombo->addItem(QString::fromUtf8("🔄 时间片轮转 (RR)"));
    m_algoCombo->addItem(QString::fromUtf8("🎯 多级反馈队列 (MLFQ)"));
    m_algoCombo->setCurrentIndex(2);
    layout->addWidget(m_algoCombo);

    // 时间片设置
    m_quantumLabel = new QLabel(QString::fromUtf8("⏲ 时间片大小"), this);
    m_quantumLabel->setStyleSheet(CyberStyle::sectionTitle());
    m_quantumLabel->hide();
    layout->addWidget(m_quantumLabel);

    m_quantumSpin = new QSpinBox(this);
    m_quantumSpin->setRange(1, 10);
    m_quantumSpin->setValue(2);
    m_quantumSpin->hide();
    layout->addWidget(m_quantumSpin);

    // 分隔
    auto* sep1 = new QLabel(this);
    sep1->setFixedHeight(1);
    sep1->setStyleSheet(QString("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 transparent, stop:0.5 %1, stop:1 transparent)").arg(Cyber::BORDER_LIT));
    layout->addWidget(sep1);

    // 进程输入
    auto* inputTitle = new QLabel(QString::fromUtf8("➕ 添加进程"), this);
    inputTitle->setStyleSheet(CyberStyle::sectionTitle());
    layout->addWidget(inputTitle);

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText(QString::fromUtf8("进程名称"));
    layout->addWidget(m_nameEdit);

    m_priorityLabel = new QLabel(QString::fromUtf8("优先级"), this);
    m_priorityLabel->setStyleSheet(QString("color: %1; font-size: 10px; background: transparent; border: none;").arg(Cyber::TEXT_DIM));
    layout->addWidget(m_priorityLabel);

    m_prioritySpin = new QSpinBox(this);
    m_prioritySpin->setRange(1, 9999);
    m_prioritySpin->setValue(10);
    layout->addWidget(m_prioritySpin);

    auto* timeLabel = new QLabel(QString::fromUtf8("运行时间"), this);
    timeLabel->setStyleSheet(QString("color: %1; font-size: 10px; background: transparent; border: none;").arg(Cyber::TEXT_DIM));
    layout->addWidget(timeLabel);

    m_timeSpin = new QSpinBox(this);
    m_timeSpin->setRange(1, 9999);
    m_timeSpin->setValue(5);
    layout->addWidget(m_timeSpin);

    auto* btnRow = new QHBoxLayout;
    m_addBtn = new QPushButton(QString::fromUtf8("➕ 添加"), this);
    m_addBtn->setStyleSheet(CyberStyle::greenButton());
    btnRow->addWidget(m_addBtn);

    m_randomBtn = new QPushButton(QString::fromUtf8("🎲 随机"), this);
    m_randomBtn->setStyleSheet(CyberStyle::orangeButton());
    btnRow->addWidget(m_randomBtn);
    layout->addLayout(btnRow);

    // 分隔
    auto* sep2 = new QLabel(this);
    sep2->setFixedHeight(1);
    sep2->setStyleSheet(QString("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 transparent, stop:0.5 %1, stop:1 transparent)").arg(Cyber::BORDER_LIT));
    layout->addWidget(sep2);

    // 速度控制
    auto* speedTitle = new QLabel(QString::fromUtf8("⚡ 运行速度"), this);
    speedTitle->setStyleSheet(CyberStyle::sectionTitle());
    layout->addWidget(speedTitle);

    m_speedSlider = new QSlider(Qt::Horizontal, this);
    m_speedSlider->setRange(200, 3000);
    m_speedSlider->setValue(1000);
    layout->addWidget(m_speedSlider);

    m_speedLabel = new QLabel("1000ms", this);
    m_speedLabel->setStyleSheet(QString("color: %1; font-size: 10px; background: transparent; border: none; "
        "font-family: 'Consolas';").arg(Cyber::CYAN));
    m_speedLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_speedLabel);

    // 分隔
    auto* sep3 = new QLabel(this);
    sep3->setFixedHeight(1);
    sep3->setStyleSheet(QString("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 transparent, stop:0.5 %1, stop:1 transparent)").arg(Cyber::BORDER_LIT));
    layout->addWidget(sep3);

    // 执行控制
    auto* ctrlTitle = new QLabel(QString::fromUtf8("▶ 执行控制"), this);
    ctrlTitle->setStyleSheet(CyberStyle::sectionTitle());
    layout->addWidget(ctrlTitle);

    auto* ctrlRow1 = new QHBoxLayout;
    m_singleBtn = new QPushButton(QString::fromUtf8("⏭ 单步"), this);
    m_singleBtn->setStyleSheet(CyberStyle::accentButton());
    ctrlRow1->addWidget(m_singleBtn);

    m_autoBtn = new QPushButton(QString::fromUtf8("▶ 自动"), this);
    m_autoBtn->setStyleSheet(CyberStyle::accentButton());
    ctrlRow1->addWidget(m_autoBtn);
    layout->addLayout(ctrlRow1);

    auto* ctrlRow2 = new QHBoxLayout;
    m_resetBtn = new QPushButton(QString::fromUtf8("↻ 重置"), this);
    m_resetBtn->setStyleSheet(CyberStyle::redButton());
    ctrlRow2->addWidget(m_resetBtn);

    m_compareBtn = new QPushButton(QString::fromUtf8("⚖ 对比"), this);
    m_compareBtn->setStyleSheet(CyberStyle::magentaButton());
    ctrlRow2->addWidget(m_compareBtn);
    layout->addLayout(ctrlRow2);

    layout->addStretch();

    connect(m_algoCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControlPanel::onAlgorithmChanged);
    connect(m_quantumSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ControlPanel::timeQuantumChanged);
    connect(m_speedSlider, &QSlider::valueChanged, this, [this](int val) {
        m_speedLabel->setText(QString("%1ms").arg(val));
        emit speedChanged(val);
    });
    connect(m_addBtn, &QPushButton::clicked, this, &ControlPanel::addProcessClicked);
    connect(m_randomBtn, &QPushButton::clicked, this, &ControlPanel::randomGenerateClicked);
    connect(m_singleBtn, &QPushButton::clicked, this, &ControlPanel::singleStepClicked);
    connect(m_autoBtn, &QPushButton::clicked, this, &ControlPanel::autoRunClicked);
    connect(m_resetBtn, &QPushButton::clicked, this, &ControlPanel::resetClicked);
    connect(m_compareBtn, &QPushButton::clicked, this, &ControlPanel::compareClicked);

    onAlgorithmChanged(m_algoCombo->currentIndex());
}

void ControlPanel::onAlgorithmChanged(int index) {
    Algorithm algo;
    switch(index) {
        case 0: algo = Algorithm::FCFS; break;
        case 1: algo = Algorithm::SJF; break;
        case 2: algo = Algorithm::Priority; break;
        case 3: algo = Algorithm::RoundRobin; break;
        case 4: algo = Algorithm::MLFQ; break;
        default: algo = Algorithm::Priority;
    }

    // RR显示时间片
    bool showQuantum = (algo == Algorithm::RoundRobin);
    m_quantumLabel->setVisible(showQuantum);
    m_quantumSpin->setVisible(showQuantum);

    // FCFS和RR不需要优先级
    bool showPriority = (algo == Algorithm::Priority || algo == Algorithm::MLFQ);
    m_priorityLabel->setVisible(showPriority);
    m_prioritySpin->setVisible(showPriority);

    emit algorithmChanged(algo);
}

QString ControlPanel::processName() const { return m_nameEdit->text(); }
int ControlPanel::processPriority() const { return m_prioritySpin->value(); }
int ControlPanel::processTime() const { return m_timeSpin->value(); }

Algorithm ControlPanel::selectedAlgorithm() const {
    switch(m_algoCombo->currentIndex()) {
        case 0: return Algorithm::FCFS;
        case 1: return Algorithm::SJF;
        case 2: return Algorithm::Priority;
        case 3: return Algorithm::RoundRobin;
        case 4: return Algorithm::MLFQ;
        default: return Algorithm::Priority;
    }
}

int ControlPanel::timeQuantum() const { return m_quantumSpin->value(); }
int ControlPanel::speedMs() const { return m_speedSlider->value(); }

void ControlPanel::setRunning(bool running) {
    m_addBtn->setEnabled(!running);
    m_randomBtn->setEnabled(!running);
    m_resetBtn->setEnabled(!running);
    m_algoCombo->setEnabled(!running);
    m_quantumSpin->setEnabled(!running);
    m_compareBtn->setEnabled(!running);
}

void ControlPanel::setAutoRunning(bool autoRunning) {
    m_autoBtn->setText(autoRunning ? QString::fromUtf8("停止运行") : QString::fromUtf8("自动运行"));
    m_singleBtn->setEnabled(!autoRunning);
}

void ControlPanel::clearInputs() {
    m_nameEdit->clear();
    m_prioritySpin->setValue(10);
    m_timeSpin->setValue(5);
}

void ControlPanel::setControlsEnabled(bool enabled) {
    m_singleBtn->setEnabled(enabled);
    m_autoBtn->setEnabled(enabled);
}
