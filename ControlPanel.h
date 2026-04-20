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

class QVBoxLayout;

/**
 * @brief 控制面板组件 - 提供进程调度的用户交互界面
 *
 * 该类继承自QWidget，负责：
 * - 调度算法选择和参数配置
 * - 新进程创建和参数设置
 * - 调度过程控制（单步执行、自动运行、重置等）
 * - 运行速度调节
 *
 * 主要设计特点：
 * - 模块化UI布局（算法配置、进程创建、控制按钮、速度调节）
 * - 信号驱动架构，解耦UI操作和业务逻辑
 * - 动态UI状态管理（运行/停止状态切换）
 * - 输入验证和默认值设置
 *
 * ## UI模块划分：
 * - **算法配置区**：算法选择、时间片设置
 * - **进程创建区**：进程名称、优先级、执行时间设置
 * - **控制按钮区**：单步执行、自动运行、重置、对比分析
 * - **速度调节区**：运行速度滑块
 *
 * @see SchedulingStrategy
 * @see Algorithm
 */
class ControlPanel : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针
     *
     * 创建控制面板实例，初始化UI组件和信号槽连接
     */
    explicit ControlPanel(QWidget* parent = nullptr);

    /**
     * @name 进程参数获取方法
     * @{
     */

    /**
     * @brief 获取进程名称
     * @return 用户输入的进程名称字符串
     */
    QString processName() const;

    /**
     * @brief 获取进程优先级
     * @return 进程优先级数值（整数）
     */
    int processPriority() const;

    /**
     * @brief 获取进程执行时间
     * @return 进程需要的执行时间（时钟周期数）
     */
    int processTime() const;

    /**
     * @brief 获取选中的调度算法
     * @return 当前选择的调度算法枚举值
     */
    Algorithm selectedAlgorithm() const;

    /**
     * @brief 获取时间片大小
     * @return RoundRobin算法的时间片长度（仅对RoundRobin有效）
     */
    int timeQuantum() const;

    /**
     * @brief 获取运行速度
     * @return 自动运行时的延迟毫秒数
     */
    int speedMs() const;

    /**
     * @} // 进程参数获取方法
     */

    /**
     * @name UI状态控制方法
     * @{
     */

    /**
     * @brief 设置运行状态
     * @param running 是否正在运行
     *
     * 根据运行状态更新UI控件的可操作性，运行期间禁用某些配置控件
     */
    void setRunning(bool running);

    /**
     * @brief 设置自动运行状态
     * @param autoRunning 是否正在自动运行
     *
     * 更新自动运行按钮的按下状态和文本
     */
    void setAutoRunning(bool autoRunning);

    /**
     * @brief 清空输入框
     *
     * 重置进程参数输入框到默认状态
     */
    void clearInputs();

    /**
     * @brief 设置控件整体可用性
     * @param enabled 是否启用所有控件
     *
     * 批量启用或禁用面板内的所有交互控件
     */
    void setControlsEnabled(bool enabled);

    /**
     * @} // UI状态控制方法
     */

signals:
    /**
     * @name 用户操作信号
     * 这些信号在用户执行相应操作时发出，通知其他组件响应
     * @{
     */

    /**
     * @brief 添加进程按钮点击信号
     * 当用户点击"添加进程"按钮时发出
     */
    void addProcessClicked();

    /**
     * @brief 随机生成按钮点击信号
     * 当用户点击"随机生成"按钮时发出
     */
    void randomGenerateClicked();

    /**
     * @brief 单步执行按钮点击信号
     * 当用户点击"单步执行"按钮时发出
     */
    void singleStepClicked();

    /**
     * @brief 自动运行按钮点击信号
     * 当用户点击"自动运行"按钮时发出
     */
    void autoRunClicked();

    /**
     * @brief 重置按钮点击信号
     * 当用户点击"重置"按钮时发出
     */
    void resetClicked();

    /**
     * @brief 对比分析按钮点击信号
     * 当用户点击"对比分析"按钮时发出
     */
    void compareClicked();

    /**
     * @brief 算法变更信号
     * @param algo 新的调度算法
     * 当用户选择不同的调度算法时发出
     */
    void algorithmChanged(Algorithm algo);

    /**
     * @brief 时间片变更信号
     * @param q 新的时间片大小
     * 当用户调整时间片大小时发出
     */
    void timeQuantumChanged(int q);

    /**
     * @brief 速度变更信号
     * @param ms 新的延迟毫秒数
     * 当用户调整运行速度时发出
     */
    void speedChanged(int ms);

    /**
     * @} // 用户操作信号
     */

private:
    /**
     * @brief 设置和初始化UI
     *
     * 创建所有子控件，建立布局，连接信号槽
     * @note 仅在构造函数中调用一次
     */
    void setupUI();
    void buildAlgorithmSection(QVBoxLayout* layout);
    void buildInputSection(QVBoxLayout* layout);
    void buildSpeedSection(QVBoxLayout* layout);
    void buildControlSection(QVBoxLayout* layout);
    void connectSignals();
    QLabel* createSeparator();
    Algorithm algorithmFromIndex(int index) const;

    /**
     * @brief 算法选择变更处理
     * @param index 新的算法索引
     *
     * 响应算法选择变化，更新时间片控件可见性
     * 并发出algorithmChanged信号
     */
    void onAlgorithmChanged(int index);

    // 算法选择下拉框
    QComboBox* m_algoCombo;
    // 时间片设置数字框
    QSpinBox* m_quantumSpin;
    // 时间片标签（用于显示/隐藏）
    QLabel* m_quantumLabel;
    // 进程名称输入框
    QLineEdit* m_nameEdit;
    // 优先级设置数字框
    QSpinBox* m_prioritySpin;
    // 执行时间设置数字框
    QSpinBox* m_timeSpin;
    // 添加进程按钮
    QPushButton* m_addBtn;
    // 随机生成按钮
    QPushButton* m_randomBtn;
    // 运行速度滑块
    QSlider* m_speedSlider;
    // 速度显示标签
    QLabel* m_speedLabel;
    // 单步执行按钮
    QPushButton* m_singleBtn;
    // 自动运行按钮
    QPushButton* m_autoBtn;
    // 重置按钮
    QPushButton* m_resetBtn;
    // 对比分析按钮
    QPushButton* m_compareBtn;
    // 优先级标签（用于布局）
    QLabel* m_priorityLabel;
};

#endif
