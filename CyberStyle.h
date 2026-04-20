#ifndef CYBERSTYLE_H
#define CYBERSTYLE_H

#include <QString>
#include <QColor>

/**
 * @namespace Cyber
 * @brief 赛博朋克风格主题的颜色常量定义
 *
 * 定义了赛博朋克风格UI所需的所有颜色常量，采用深蓝黑色调配合霓虹色点缀
 * 颜色命名规范：
 * - BG_ 开头：背景色
 * - BORDER_ 开头：边框色
 * - 基本颜色：CYAN(青色)、BLUE(蓝色)、GREEN(绿色)、MAGENTA(洋红)、ORANGE(橙色)、RED(红色)、YELLOW(黄色)、PURPLE(紫色)
 * - TEXT_ 开头：文本颜色
 * - _DARK 后缀：深色变体
 */
namespace Cyber {
    constexpr auto BG_DEEP    = "#050810";      // 最深层背景色，用于主窗口背景
    constexpr auto BG_PANEL   = "#0a1020";      // 面板背景色，用于控制面板等
    constexpr auto BG_CARD    = "#0d1525";      // 卡片背景色，用于卡片式UI元素
    constexpr auto BORDER     = "#1a2744";      // 标准边框色
    constexpr auto BORDER_LIT = "#3a5f8f";      // 高亮边框色，用于焦点状态
    constexpr auto CYAN       = "#00ffff";      // 主青色，赛博朋克标志性颜色
    constexpr auto CYAN_DARK  = "#00aaaa";      // 深青色，用于阴影或禁用状态
    constexpr auto BLUE       = "#1e90ff";      // 亮蓝色，用于选中状态
    constexpr auto BLUE_DARK  = "#1060a0";      // 深蓝色
    constexpr auto GREEN      = "#00ff88";      // 荧光绿，用于成功状态
    constexpr auto GREEN_DARK = "#00aa55";      // 深绿色
    constexpr auto MAGENTA    = "#ff00ff";      // 洋红色，用于警告状态
    constexpr auto MAGENTA_DARK = "#aa00aa";    // 深洋红色
    constexpr auto ORANGE     = "#ff8800";      // 橙色，用于重要操作
    constexpr auto ORANGE_DARK = "#aa5500";     // 深橙色
    constexpr auto RED        = "#ff3366";      // 红色，用于危险操作
    constexpr auto RED_DARK   = "#aa2244";      // 深红色
    constexpr auto YELLOW     = "#ffff00";      // 黄色
    constexpr auto PURPLE     = "#aa00ff";      // 紫色
    constexpr auto TEXT       = "#b0c0d0";      // 主要文本颜色
    constexpr auto TEXT_DIM   = "#405060";      // 暗淡文本，用于次要信息
    constexpr auto TEXT_BRIGHT= "#ffffff";      // 明亮文本，用于高亮显示
    constexpr auto TEXT_NEON  = "#e0f8ff";      // 霓虹文本，用于标题或强调
}

/**
 * @class CyberStyle
 * @brief 赛博朋克风格UI主题样式类
 *
 * 提供赛博朋克风格的Qt UI样式，包括全局样式表和各种预定义样式
 * 该类使用静态方法，无需实例化即可使用
 *
 * 使用示例：
 * @code
 * // 应用全局样式
 * qApp->setStyleSheet(CyberStyle::globalStyleSheet());
 *
 * // 使用特定样式
 * QString buttonStyle = CyberStyle::accentButton();
 * @endcode
 */
class CyberStyle {
public:
    /**
     * @brief 获取全局样式表
     *
     * 返回完整的赛博朋克风格样式表，包含所有标准Qt控件的样式定义
     * 适用于整个应用程序的样式设置
     *
     * 样式特点：
     * - 深色背景配合霓虹色点缀
     * - 统一的圆角设计
     * - 悬停和按下状态的动画效果
     * - 自定义滚动条和滑块样式
     *
     * @return 完整的样式表字符串
     *
     * @note 建议使用 qApp->setStyleSheet(CyberStyle::globalStyleSheet()) 设置应用全局样式
     */
    static QString globalStyleSheet() {
        return QString(R"(
            * {
                font-family: 'Consolas', 'Courier New', 'Microsoft YaHei', monospace;
            }
            QMainWindow, QDialog {
                background-color: %1;
                border: 1px solid %5;
            }
            QWidget {
                color: %2;
            }
            QLabel {
                color: %2;
                background: transparent;
            }
            QPushButton {
                background-color: %3;
                color: %4;
                border: 1px solid %5;
                border-radius: 4px;
                padding: 8px 16px;
                font-size: 13px;
                font-weight: bold;
                min-height: 20px;
            }
            QPushButton:hover {
                background-color: %5;
                border-color: %4;
                color: %6;
                border: 1px solid %4;
            }
            QPushButton:pressed {
                background-color: %7;
                border-color: %4;
            }
            QPushButton:disabled {
                background-color: #080a10;
                color: #283040;
                border-color: #151820;
            }
            QLineEdit, QSpinBox {
                background-color: %3;
                color: %4;
                border: 1px solid %5;
                border-radius: 4px;
                padding: 6px 10px;
                font-size: 13px;
                selection-background-color: %8;
            }
            QLineEdit:focus, QSpinBox:focus {
                border-color: %4;
                background-color: %7;
            }
            QComboBox {
                background-color: %3;
                color: %4;
                border: 1px solid %5;
                border-radius: 4px;
                padding: 6px 10px;
                font-size: 13px;
                min-height: 20px;
            }
            QComboBox::drop-down {
                border: none;
                width: 24px;
            }
            QComboBox::down-arrow {
                image: none;
                border-left: 5px solid transparent;
                border-right: 5px solid transparent;
                border-top: 6px solid %4;
                margin-right: 8px;
            }
            QComboBox QAbstractItemView {
                background-color: %3;
                color: %4;
                border: 1px solid %5;
                selection-background-color: %5;
                selection-color: %6;
                outline: none;
            }
            QSlider::groove:horizontal {
                background: %5;
                height: 6px;
                border-radius: 3px;
            }
            QSlider::handle:horizontal {
                background: %4;
                width: 16px;
                height: 16px;
                margin: -5px 0;
                border-radius: 8px;
            }
            QSlider::handle:horizontal:hover {
                background: %6;
            }
            QSlider::sub-page:horizontal {
                background: %8;
                border-radius: 3px;
            }
            QProgressBar {
                background-color: %3;
                border: 1px solid %5;
                border-radius: 4px;
                text-align: center;
                color: %4;
                font-size: 11px;
                min-height: 18px;
            }
            QProgressBar::chunk {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                    stop:0 %8, stop:1 %4);
                border-radius: 3px;
            }
            QTableWidget {
                background-color: %3;
                color: %2;
                border: 1px solid %5;
                border-radius: 4px;
                gridline-color: %5;
                selection-background-color: %5;
                font-size: 12px;
            }
            QTableWidget::item {
                padding: 4px;
                border-bottom: 1px solid #141e30;
            }
            QHeaderView::section {
                background-color: %7;
                color: %4;
                border: none;
                border-bottom: 2px solid %4;
                padding: 6px;
                font-weight: bold;
                font-size: 12px;
            }
            QScrollBar:vertical {
                background: %1;
                width: 8px;
                border-radius: 4px;
            }
            QScrollBar::handle:vertical {
                background: %5;
                border-radius: 4px;
                min-height: 30px;
            }
            QScrollBar::handle:vertical:hover {
                background: %4;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }
            QScrollBar:horizontal {
                background: %1;
                height: 8px;
                border-radius: 4px;
            }
            QScrollBar::handle:horizontal {
                background: %5;
                border-radius: 4px;
                min-width: 30px;
            }
            QScrollBar::handle:horizontal:hover {
                background: %4;
            }
            QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
                width: 0px;
            }
            QToolTip {
                background-color: %3;
                color: %4;
                border: 1px solid %4;
                padding: 4px;
                font-size: 12px;
            }
        )")
        .arg(Cyber::BG_DEEP)      // %1
        .arg(Cyber::TEXT)          // %2
        .arg(Cyber::BG_CARD)      // %3
        .arg(Cyber::CYAN)         // %4
        .arg(Cyber::BORDER_LIT)   // %5
        .arg(Cyber::TEXT_BRIGHT)   // %6
        .arg(Cyber::BG_PANEL)     // %7
        .arg(Cyber::BLUE);        // %8
    }

    /**
     * @brief 获取面板样式
     *
     * 返回面板控件的渐变背景样式，通常用于分组框、面板等容器控件
     * 采用从上到下的渐变效果，增强视觉层次感
     *
     * 样式效果：
     * - 顶部：BG_PANEL颜色
     * - 底部：BG_DEEP颜色
     * - 边框：BORDER_LIT颜色
     * - 圆角：8px
     *
     * @return 面板样式字符串
     *
     * @note 适用于 QGroupBox、QFrame 等容器控件
     */
    static QString panelStyle() {
        return QString("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                      "stop:0 %1, stop:1 %2); "
                      "border: 1px solid %3; border-radius: 8px;")
            .arg(Cyber::BG_PANEL).arg(Cyber::BG_DEEP).arg(Cyber::BORDER_LIT);
    }

    /**
     * @brief 获取霓虹标题样式
     *
     * 返回赛博朋克风格的标题样式，使用霓虹青色
     * 适用于主标题或重要标题文本
     *
     * 样式特点：
     * - 颜色：霓虹青色 (CYAN)
     * - 字体大小：18px
     * - 字体粗细：粗体
     * - 透明背景
     *
     * @return 霓虹标题样式字符串
     *
     * @note 适用于 QLabel 作为标题使用
     */
    static QString neonTitle() {
        return QString("color: %1; font-size: 18px; font-weight: bold; "
                      "background: transparent; border: none; "
                      "padding: 4px 8px;")
            .arg(Cyber::CYAN);
    }

    /**
     * @brief 获取分区标题样式
     *
     * 返回分区标题的样式，带有左侧边框装饰
     * 适用于分组标题或分区标题
     *
     * 样式特点：
     * - 颜色：霓虹青色 (CYAN)
     * - 字体大小：13px
     * - 字体粗细：粗体
     * - 左侧边框：3px实线
     *
     * @return 分区标题样式字符串
     *
     * @note 适用于 QLabel 作为分区标题使用
     */
    static QString sectionTitle() {
        return QString("color: %1; font-size: 13px; font-weight: bold; "
                      "background: transparent; border: none; "
                      "padding: 2px 0px; border-left: 3px solid %1; "
                      "margin-left: 4px;")
            .arg(Cyber::CYAN);
    }

    /**
     * @brief 获取强调按钮样式
     *
     * 返回赛博朋克风格的强调按钮样式，使用霓虹青色
     * 适用于主要操作按钮或需要突出显示的按钮
     *
     * 样式特点：
     * - 默认状态：半透明青色渐变背景
     * - 悬停状态：增加光晕效果
     * - 按下状态：背景变为实色
     *
     * @return 强调按钮样式字符串
     *
     * @note 用于主要操作按钮，如"确认"、"开始"等
     */
    static QString accentButton() {
        return QString(
            "QPushButton { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0,255,255,0.05), stop:1 rgba(0,255,255,0.15)); "
            "  color: %1; border: 1px solid %1; border-radius: 4px; "
            "}"
            "QPushButton:hover { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0,255,255,0.2), stop:1 rgba(0,255,255,0.35)); "
            "  border-color: %1; "
            "}"
            "QPushButton:pressed { "
            "  background: rgba(0,255,255,0.4); border-color: %1; "
            "}"
        ).arg(Cyber::CYAN);
    }

    /**
     * @brief 获取绿色按钮样式
     *
     * 返回绿色主题的按钮样式
     * 适用于成功、确认等正面操作
     *
     * 样式特点：
     * - 使用荧光绿色 (GREEN)
     * - 带有渐变和悬停效果
     *
     * @return 绿色按钮样式字符串
     *
     * @note 适用于成功操作，如"保存"、"应用"等
     */
    static QString greenButton() {
        return QString(
            "QPushButton { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0,255,136,0.05), stop:1 rgba(0,255,136,0.15)); "
            "  color: %1; border: 1px solid %1; border-radius: 4px; "
            "}"
            "QPushButton:hover { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0,255,136,0.2), stop:1 rgba(0,255,136,0.35)); "
            "  border-color: %1; "
            "}"
            "QPushButton:pressed { "
            "  background: rgba(0,255,136,0.4); border-color: %1; "
            "}"
        ).arg(Cyber::GREEN);
    }

    /**
     * @brief 获取红色按钮样式
     *
     * 返回红色主题的按钮样式
     * 适用于危险、删除等负面操作
     *
     * 样式特点：
     * - 使用红色 (RED)
     * - 带有渐变和悬停效果
     *
     * @return 红色按钮样式字符串
     *
     * @note 适用于危险操作，如"删除"、"取消"等
     */
    static QString redButton() {
        return QString(
            "QPushButton { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,51,102,0.05), stop:1 rgba(255,51,102,0.15)); "
            "  color: %1; border: 1px solid %1; border-radius: 4px; "
            "}"
            "QPushButton:hover { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,51,102,0.2), stop:1 rgba(255,51,102,0.35)); "
            "  border-color: %1; "
            "}"
            "QPushButton:pressed { "
            "  background: rgba(255,51,102,0.4); border-color: %1; "
            "}"
        ).arg(Cyber::RED);
    }

    /**
     * @brief 获取橙色按钮样式
     *
     * 返回橙色主题的按钮样式
     * 适用于警告、重要操作
     *
     * 样式特点：
     * - 使用橙色 (ORANGE)
     * - 带有渐变和悬停效果
     *
     * @return 橙色按钮样式字符串
     *
     * @note 适用于重要操作，如"警告"、"重要操作"等
     */
    static QString orangeButton() {
        return QString(
            "QPushButton { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,136,0,0.05), stop:1 rgba(255,136,0,0.15)); "
            "  color: %1; border: 1px solid %1; border-radius: 4px; "
            "}"
            "QPushButton:hover { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,136,0,0.2), stop:1 rgba(255,136,0,0.35)); "
            "  border-color: %1; "
            "}"
            "QPushButton:pressed { "
            "  background: rgba(255,136,0,0.4); border-color: %1; "
            "}"
        ).arg(Cyber::ORANGE);
    }

    /**
     * @brief 获取洋红色按钮样式
     *
     * 返回洋红色主题的按钮样式
     * 适用于特殊、魔法般的操作
     *
     * 样式特点：
     * - 使用洋红色 (MAGENTA)
     * - 带有渐变和悬停效果
     *
     * @return 洋红色按钮样式字符串
     *
     * @note 适用于特殊操作，如"魔法"、"特殊功能"等
     */
    static QString magentaButton() {
        return QString(
            "QPushButton { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,0,255,0.05), stop:1 rgba(255,0,255,0.15)); "
            "  color: %1; border: 1px solid %1; border-radius: 4px; "
            "}"
            "QPushButton:hover { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,0,255,0.2), stop:1 rgba(255,0,255,0.35)); "
            "  border-color: %1; "
            "}"
            "QPushButton:pressed { "
            "  background: rgba(255,0,255,0.4); border-color: %1; "
            "}"
        ).arg(Cyber::MAGENTA);
    }

    /**
     * @brief 获取全息卡片样式
     *
     * 返回全息效果的卡片样式
     * 适用于卡片式UI元素，如设置卡片、信息卡片等
     *
     * 样式特点：
     * - 半透明渐变背景
     * - 高亮边框
     * - 圆角设计
     *
     * @return 全息卡片样式字符串
     *
     * @note 适用于 QWidget、QFrame 等作为卡片容器使用
     */
    static QString holographicCard() {
        return QString(
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
            "stop:0 rgba(13,21,37,0.9), stop:1 rgba(5,8,16,0.95)); "
            "border: 1px solid %1; border-radius: 6px;"
        ).arg(Cyber::BORDER_LIT);
    }
};

#endif
