#ifndef EXPLANATIONBAR_H
#define EXPLANATIONBAR_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

/**
 * @brief 解释说明栏组件 - 显示调度过程的详细解释和教学说明
 *
 * 该类继承自QWidget，负责：
 * - 展示调度步骤的详细解释说明
 * - 支持打字机动画效果，逐字显示文本
 * - 提供教学功能，帮助用户理解调度算法
 *
 * 主要设计特点：
 * - 图标和文本分离的显示方式
 * - 逐字显示的打字机动画效果
 * - 自动换行和文本适配
 * - 支持动态文本更新和清除
 *
 * ## 使用场景：
 * - 单步执行时显示当前调度决策的解释
 * - 自动运行时显示重要调度事件的说明
 * - 教学演示时提供详细的算法步骤说明
 */
class ExplanationBar : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针
     *
     * 创建解释栏实例，初始化图标和文本标签，设置定时器
     */
    explicit ExplanationBar(QWidget* parent = nullptr);

    /**
     * @brief 设置解释文本
     * @param text 要显示的解释说明文本
     *
     * 设置完整的解释文本，并开始逐字显示动画
     * 如果当前正在显示其他文本，会先清除现有内容
     *
     * @note 支持长文本，会自动处理换行
     * @see typeNextChar()
     */
    void setText(const QString& text);

    /**
     * @brief 清空解释文本
     *
     * 立即清除所有显示的文本和图标，恢复到初始状态
     * 同时停止打字机动画
     */
    void clear();

private slots:
    /**
     * @brief 打字机动画下一字符显示
     *
     * 定时器槽函数，每次调用显示一个字符，实现逐字显示效果
     * 当所有字符显示完成后，停止定时器
     *
     * @note 动画速度通过定时器间隔控制
     * @see setText()
     */
    void typeNextChar();

private:
    // 解释栏图标标签（显示提示图标）
    QLabel* m_icon;
    // 解释文本标签（显示详细的解释说明）
    QLabel* m_text;
    // 完整的解释文本内容
    QString m_fullText;
    // 当前已显示的文本内容
    QString m_displayedText;
    // 当前显示到的字符索引
    int m_charIndex = 0;
    // 打字机动画定时器
    QTimer* m_typeTimer;
    // 是否正在执行打字机动画的标志
    bool m_isTyping = false;
};

#endif