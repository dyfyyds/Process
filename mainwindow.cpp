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

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowIcon(QIcon(":/icon/process.png"));
    setMinimumSize(1100, 750);
    resize(1200, 800);
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

    // === 标题栏 ===
    auto* titleBar = new QWidget(central);
    titleBar->setFixedHeight(50);
    titleBar->setStyleSheet(QString(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 %1, stop:1 %2); "
        "border-bottom: 1px solid %3;")
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
    titleLayout->addWidget(titleLabel);

    // 标题发光效果
    auto* titleGlow = new QGraphicsDropShadowEffect;
    titleGlow->setColor(QColor(Cyber::CYAN));
    titleGlow->setBlurRadius(25);
    titleGlow->setOffset(0, 0);
    titleLabel->setGraphicsEffect(titleGlow);

    titleLayout->addStretch();

    auto* miniBtn = new QPushButton(titleBar);
    miniBtn->setIcon(QIcon(":/icon/mini.png"));
    miniBtn->setIconSize(QSize(18, 18));
    miniBtn->setFixedSize(35, 35);
    miniBtn->setStyleSheet(
        "QPushButton { border: none; background: transparent; border-radius: 4px; }"
        "QPushButton:hover { background-color: rgba(0,255,255,0.15); }");
    connect(miniBtn, &QPushButton::clicked, this, &QMainWindow::showMinimized);
    titleLayout->addWidget(miniBtn);

    auto* closeBtn = new QPushButton(titleBar);
    closeBtn->setIcon(QIcon(":/icon/close.png"));
    closeBtn->setIconSize(QSize(18, 18));
    closeBtn->setFixedSize(35, 35);
    closeBtn->setStyleSheet(
        "QPushButton { border: none; background: transparent; border-radius: 4px; }"
        "QPushButton:hover { background-color: rgba(255,51,102,0.3); }");
    connect(closeBtn, &QPushButton::clicked, this, &QMainWindow::close);
    titleLayout->addWidget(closeBtn);

    rootLayout->addWidget(titleBar);

    // === 主体区域 ===
    auto* bodyLayout = new QHBoxLayout;
    bodyLayout->setContentsMargins(10, 0, 10, 10);
    bodyLayout->setSpacing(10);

    // 左侧控制面板
    m_controlPanel = new ControlPanel(central);
    bodyLayout->addWidget(m_controlPanel);

    // 中央区域
    auto* centerLayout = new QVBoxLayout;
    centerLayout->setSpacing(8);

    m_cpuWidget = new CpuWidget(central);
    centerLayout->addWidget(m_cpuWidget);

    m_readyQueue = new ReadyQueueWidget(central);
    centerLayout->addWidget(m_readyQueue);

    // 甘特图放在ScrollArea中
    m_ganttScroll = new QScrollArea(central);
    m_ganttScroll->setWidgetResizable(true);
    m_ganttScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_ganttScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ganttScroll->setStyleSheet("QScrollArea { background: transparent; border: none; }");
    m_ganttWidget = new GanttWidget;
    m_ganttScroll->setWidget(m_ganttWidget);
    centerLayout->addWidget(m_ganttScroll, 1);

    bodyLayout->addLayout(centerLayout, 1);

    // 右侧统计面板
    m_statsPanel = new StatsPanel(central);
    bodyLayout->addWidget(m_statsPanel);

    rootLayout->addLayout(bodyLayout, 1);

    // === 底部解释栏 ===
    m_explanationBar = new ExplanationBar(central);
    auto* bottomLayout = new QHBoxLayout;
    bottomLayout->setContentsMargins(10, 0, 10, 10);
    bottomLayout->addWidget(m_explanationBar);
    rootLayout->addLayout(bottomLayout);
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
        // 重新排列就绪队列
        // 取出所有进程重新插入
        std::vector<Process*> procs;
        while(!m_scheduler->getReadyList().empty()) {
            Process* p = m_scheduler->getReadyList().front();
            m_scheduler->getReadyList().pop_front();
            procs.push_back(p);
        }
        // 清空allProcesses中的指针（addProcess会重新添加）
        // 注意：这里需要小心内存管理
        m_scheduler->reset();
        m_existNames.clear();
        for(auto* p : procs) {
            Process* newP = new Process(p->getName(), p->getOriginalPriority(), p->getNTime());
            newP->setColor(p->getColor());
            m_scheduler->addProcess(newP);
            m_existNames.insert(QString::fromStdString(p->getName()));
            delete p;
        }
        m_readyQueue->refreshAll(m_scheduler->getReadyList());
        m_controlPanel->setControlsEnabled(!m_scheduler->getReadyList().empty());
    }
}

void MainWindow::onSpeedChanged(int ms) {
    if(m_timer->isActive()) {
        m_timer->setInterval(ms);
    }
}

void MainWindow::executeStep() {
    if(m_scheduler->getReadyList().empty() && !m_scheduler->getCurrentProcess()) {
        // 执行完毕
        m_timer->stop();
        m_autoRunning = false;
        m_cpuWidget->clearProcess();
        m_controlPanel->setAutoRunning(false);
        m_controlPanel->setRunning(false);
        m_controlPanel->setControlsEnabled(false);
        m_explanationBar->setText(QString::fromUtf8("所有进程执行完毕！"));
        return;
    }

    m_controlPanel->setRunning(true);

    StepResult result = m_scheduler->stepRun();

    // 更新CPU显示
    if(result.process) {
        m_cpuWidget->setProcess(result.process);
    } else {
        m_cpuWidget->clearProcess();
    }
    m_cpuWidget->setTick(m_scheduler->getTick());

    // 更新就绪队列
    m_readyQueue->refreshAll(m_scheduler->getReadyList());

    // 更新甘特图
    m_ganttWidget->setGanttData(m_scheduler->getGanttData(), m_scheduler->getTick());
    // 自动滚动到右侧
    if(m_ganttScroll->horizontalScrollBar()) {
        m_ganttScroll->horizontalScrollBar()->setValue(
            m_ganttScroll->horizontalScrollBar()->maximum());
    }

    // 更新统计
    m_statsPanel->updateStats(m_scheduler.get());

    // 更新解释
    m_explanationBar->setText(result.explanation);

    // 检查是否全部完成
    if(m_scheduler->getReadyList().empty() && !m_scheduler->getCurrentProcess()) {
        if(!m_autoRunning) {
            m_controlPanel->setRunning(false);
            m_controlPanel->setControlsEnabled(false);
        }
    } else {
        if(!m_autoRunning) {
            m_controlPanel->setRunning(false);
        }
    }
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
