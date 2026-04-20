#include "mainwindow.h"
#include "CyberStyle.h"
#include "ColorGenerator.h"
#include "ComparisonDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_timer(new QTimer(this))
{
    m_scheduler = std::make_shared<Scheduler>();

    setupUI();
    connectSignals();
    applyWindowChrome();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    auto* central = new QWidget(this);
    central->setStyleSheet(QString(
        "background-color: %1; border-radius: 10px; border: 1px solid %2;")
        .arg(Cyber::BG_DEEP).arg(Cyber::BORDER_LIT));
    setCentralWidget(central);

    auto* rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    rootLayout->addWidget(createTitleBar(central));
    rootLayout->addLayout(createBodyLayout(central), 1);
    rootLayout->addLayout(createBottomLayout(central));
}

void MainWindow::applyWindowChrome() {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowIcon(QIcon(":/icon/process.png"));
    setMinimumSize(1100, 750);
    resize(1200, 800);
}

QPushButton* MainWindow::createWindowButton(const QString& iconPath, const QString& hoverStyle, QWidget* parent) {
    auto* button = new QPushButton(parent);
    button->setIcon(QIcon(iconPath));
    button->setIconSize(QSize(18, 18));
    button->setFixedSize(35, 35);
    button->setStyleSheet(QString(
        "QPushButton { border: none; background: transparent; border-radius: 4px; }")
        + hoverStyle);
    return button;
}

QWidget* MainWindow::createTitleBar(QWidget* parent) {
    auto* titleBar = new QWidget(parent);
    titleBar->setFixedHeight(50);
    titleBar->setStyleSheet(QString(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 %1, stop:1 %2); border-bottom: 1px solid %3;")
        .arg(Cyber::BG_DEEP).arg(Cyber::BG_PANEL).arg(Cyber::BORDER_LIT));

    auto* titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(15, 5, 10, 5);

    auto* iconBtn = new QPushButton(titleBar);
    iconBtn->setIcon(QIcon(":/icon/process.png"));
    iconBtn->setIconSize(QSize(26, 26));
    iconBtn->setFixedSize(35, 35);
    iconBtn->setStyleSheet("border: none; background: transparent;");
    titleLayout->addWidget(iconBtn);

    auto* titleLabel = new QLabel(QString::fromUtf8("⚡ 进程调度仿真系统"), titleBar);
    titleLabel->setStyleSheet(CyberStyle::neonTitle());
    auto* titleGlow = new QGraphicsDropShadowEffect;
    titleGlow->setColor(QColor(Cyber::CYAN));
    titleGlow->setBlurRadius(25);
    titleGlow->setOffset(0, 0);
    titleLabel->setGraphicsEffect(titleGlow);
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    auto* miniBtn = createWindowButton(":/icon/mini.png",
        "QPushButton:hover { background-color: rgba(0,255,255,0.15); }", titleBar);
    connect(miniBtn, &QPushButton::clicked, this, &QMainWindow::showMinimized);
    titleLayout->addWidget(miniBtn);

    auto* closeBtn = createWindowButton(":/icon/close.png",
        "QPushButton:hover { background-color: rgba(255,51,102,0.3); }", titleBar);
    connect(closeBtn, &QPushButton::clicked, this, &QMainWindow::close);
    titleLayout->addWidget(closeBtn);
    return titleBar;
}

QHBoxLayout* MainWindow::createBodyLayout(QWidget* parent) {
    auto* bodyLayout = new QHBoxLayout;
    bodyLayout->setContentsMargins(10, 0, 10, 10);
    bodyLayout->setSpacing(10);

    m_controlPanel = new ControlPanel(parent);
    bodyLayout->addWidget(m_controlPanel);

    auto* centerLayout = new QVBoxLayout;
    centerLayout->setSpacing(8);

    m_cpuWidget = new CpuWidget(parent);
    m_readyQueue = new ReadyQueueWidget(parent);
    m_ganttScroll = new QScrollArea(parent);
    m_ganttWidget = new GanttWidget;
    m_ganttScroll->setWidgetResizable(true);
    m_ganttScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_ganttScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ganttScroll->setStyleSheet("QScrollArea { background: transparent; border: none; }");
    m_ganttScroll->setWidget(m_ganttWidget);

    centerLayout->addWidget(m_cpuWidget);
    centerLayout->addWidget(m_readyQueue);
    centerLayout->addWidget(m_ganttScroll, 1);
    bodyLayout->addLayout(centerLayout, 1);

    m_statsPanel = new StatsPanel(parent);
    bodyLayout->addWidget(m_statsPanel);
    return bodyLayout;
}

QHBoxLayout* MainWindow::createBottomLayout(QWidget* parent) {
    m_explanationBar = new ExplanationBar(parent);

    auto* bottomLayout = new QHBoxLayout;
    bottomLayout->setContentsMargins(10, 0, 10, 10);
    bottomLayout->addWidget(m_explanationBar);
    return bottomLayout;
}

void MainWindow::connectSignals() {
    connect(m_controlPanel, &ControlPanel::addProcessClicked, this, &MainWindow::onAddProcess);
    connect(m_controlPanel, &ControlPanel::randomGenerateClicked, this, &MainWindow::onRandomGenerate);
    connect(m_controlPanel, &ControlPanel::singleStepClicked, this, &MainWindow::onSingleStep);
    connect(m_controlPanel, &ControlPanel::autoRunClicked, this, &MainWindow::onAutoRun);
    connect(m_controlPanel, &ControlPanel::resetClicked, this, &MainWindow::onReset);
    connect(m_controlPanel, &ControlPanel::compareClicked, this, &MainWindow::onCompare);
    connect(m_controlPanel, &ControlPanel::algorithmChanged, this, &MainWindow::onAlgorithmChanged);
    connect(m_controlPanel, &ControlPanel::speedChanged, this, &MainWindow::onSpeedChanged);
    connect(m_controlPanel, &ControlPanel::timeQuantumChanged, this, [this](int q) {
        m_scheduler->setTimeQuantum(q);
    });
    connect(m_timer, &QTimer::timeout, this, &MainWindow::executeStep);
}

void MainWindow::onAddProcess() {
    QString name = m_controlPanel->processName().trimmed();
    if(name.isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("提示"), QString::fromUtf8("请输入进程名称！"));
        return;
    }
    if(m_existNames.contains(name)) {
        QMessageBox::warning(this, QString::fromUtf8("提示"), QString::fromUtf8("进程名称已存在！"));
        return;
    }

    int priority = m_controlPanel->processPriority();
    int ntime = m_controlPanel->processTime();

    Process* proc = new Process(name.toStdString(), priority, ntime);
    proc->setColor(ColorGenerator::next());

    int index = m_scheduler->addProcess(proc);
    m_existNames.insert(name);
    m_controlPanel->clearInputs();

    // 刷新就绪队列显示
    m_readyQueue->refreshAll(m_scheduler->getReadyList());
    m_statsPanel->updateStats(m_scheduler.get());

    m_controlPanel->setControlsEnabled(!m_scheduler->getReadyList().empty());
}

void MainWindow::onRandomGenerate() {
    static const QStringList names = {
        "P1","P2","P3","P4","P5","P6","P7","P8","P9","P10",
        "A","B","C","D","E","X","Y","Z","Alpha","Beta"
    };

    int count = QRandomGenerator::global()->bounded(3, 6); // 3~5个
    int added = 0;

    for(int i = 0; i < count && added < count; i++) {
        QString name = names[QRandomGenerator::global()->bounded(names.size())];
        if(m_existNames.contains(name)) continue;

        int priority = QRandomGenerator::global()->bounded(1, 20);
        int ntime = QRandomGenerator::global()->bounded(1, 8);

        Process* proc = new Process(name.toStdString(), priority, ntime);
        proc->setColor(ColorGenerator::next());
        m_scheduler->addProcess(proc);
        m_existNames.insert(name);
        added++;
    }

    m_readyQueue->refreshAll(m_scheduler->getReadyList());
    m_statsPanel->updateStats(m_scheduler.get());
    m_controlPanel->setControlsEnabled(!m_scheduler->getReadyList().empty());

    m_explanationBar->setText(QString::fromUtf8("随机生成了 %1 个进程，准备就绪。").arg(added));
}

void MainWindow::onSingleStep() {
    executeStep();
}

void MainWindow::onAutoRun() {
    if(!m_autoRunning) {
        if(m_scheduler->getReadyList().empty() && !m_scheduler->getCurrentProcess()) return;
        m_autoRunning = true;
        m_timer->start(m_controlPanel->speedMs());
        m_controlPanel->setAutoRunning(true);
        m_controlPanel->setRunning(true);
    } else {
        m_autoRunning = false;
        m_timer->stop();
        m_controlPanel->setAutoRunning(false);
        m_controlPanel->setRunning(false);
    }
}

void MainWindow::onReset() {
    m_timer->stop();
    m_autoRunning = false;
    m_scheduler->reset();
    m_existNames.clear();

    m_cpuWidget->clearProcess();
    m_cpuWidget->setTick(0);
    m_readyQueue->clear();
    m_ganttWidget->clear();
    m_statsPanel->clear();
    m_explanationBar->clear();
    m_controlPanel->setAutoRunning(false);
    m_controlPanel->setRunning(false);
    m_controlPanel->setControlsEnabled(false);
}

void MainWindow::onCompare() {
    const auto& allProcs = m_scheduler->getAllProcesses();
    if(allProcs.empty()) {
        QMessageBox::warning(this, QString::fromUtf8("提示"),
                             QString::fromUtf8("请先添加进程再进行算法对比！"));
        return;
    }

    ComparisonDialog dlg(allProcs, this);
    dlg.exec();
}

void MainWindow::onAlgorithmChanged(Algorithm algo) {
    // 只在没开始运行时允许切换
    if(m_scheduler->getTick() == 0) {
        m_scheduler->setStrategy(algo);
        m_statsPanel->updateStats(m_scheduler.get());
        rebuildProcessesForAlgorithmChange();
        m_readyQueue->refreshAll(m_scheduler->getReadyList());
        m_controlPanel->setControlsEnabled(!m_scheduler->getReadyList().empty());
    }
}

void MainWindow::onSpeedChanged(int ms) {
    if(m_timer->isActive()) {
        m_timer->setInterval(ms);
    }
}

bool MainWindow::isExecutionComplete() const {
    return m_scheduler->getReadyList().empty() && !m_scheduler->getCurrentProcess();
}

void MainWindow::handleExecutionComplete() {
    m_timer->stop();
    m_autoRunning = false;
    m_cpuWidget->clearProcess();
    m_controlPanel->setAutoRunning(false);
    m_controlPanel->setRunning(false);
    m_controlPanel->setControlsEnabled(false);
    m_explanationBar->setText(QString::fromUtf8("所有进程执行完毕！"));
}

void MainWindow::updateExecutionViews(const StepResult& result) {
    if(result.process) {
        m_cpuWidget->setProcess(result.process);
    } else {
        m_cpuWidget->clearProcess();
    }

    m_cpuWidget->setTick(m_scheduler->getTick());
    m_readyQueue->refreshAll(m_scheduler->getReadyList());
    m_ganttWidget->setGanttData(m_scheduler->getGanttData(), m_scheduler->getTick());
    m_statsPanel->updateStats(m_scheduler.get());
    m_explanationBar->setText(result.explanation);

    if(m_ganttScroll->horizontalScrollBar()) {
        m_ganttScroll->horizontalScrollBar()->setValue(
            m_ganttScroll->horizontalScrollBar()->maximum());
    }
}

void MainWindow::updateStepControls() {
    if(m_autoRunning) return;

    m_controlPanel->setRunning(false);
    if(isExecutionComplete()) {
        m_controlPanel->setControlsEnabled(false);
    }
}

void MainWindow::rebuildProcessesForAlgorithmChange() {
    std::vector<Process*> procs;
    while(!m_scheduler->getReadyList().empty()) {
        Process* p = m_scheduler->getReadyList().front();
        m_scheduler->getReadyList().pop_front();
        procs.push_back(p);
    }

    m_scheduler->reset();
    m_existNames.clear();
    for(auto* p : procs) {
        Process* newP = new Process(p->getName(), p->getOriginalPriority(), p->getNTime());
        newP->setColor(p->getColor());
        m_scheduler->addProcess(newP);
        m_existNames.insert(QString::fromStdString(p->getName()));
        delete p;
    }
}

void MainWindow::executeStep() {
    if(isExecutionComplete()) {
        handleExecutionComplete();
        return;
    }

    m_controlPanel->setRunning(true);
    const StepResult result = m_scheduler->stepRun();
    updateExecutionViews(result);
    updateStepControls();
}

void MainWindow::refreshUI() {
    m_readyQueue->refreshAll(m_scheduler->getReadyList());
    m_statsPanel->updateStats(m_scheduler.get());
    m_ganttWidget->setGanttData(m_scheduler->getGanttData(), m_scheduler->getTick());
}

void MainWindow::mousePressEvent(QMouseEvent* ev) {
    if(ev->button() == Qt::LeftButton) {
        m_isDrag = true;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        m_dragVal = ev->globalPosition() - pos();
#else
        m_dragVal = ev->globalPos() - pos();
#endif
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* ev) {
    if(m_isDrag) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        move((ev->globalPosition() - m_dragVal).toPoint());
#else
        move((ev->globalPos() - m_dragVal).toPoint());
#endif
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* ev) {
    if(ev->button() == Qt::LeftButton)
        m_isDrag = false;
}
