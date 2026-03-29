#ifndef CYBERSTYLE_H
#define CYBERSTYLE_H

#include <QString>
#include <QColor>

namespace Cyber {
    constexpr auto BG_DEEP    = "#050810";
    constexpr auto BG_PANEL   = "#0a1020";
    constexpr auto BG_CARD    = "#0d1525";
    constexpr auto BORDER     = "#1a2744";
    constexpr auto BORDER_LIT = "#3a5f8f";
    constexpr auto CYAN       = "#00ffff";
    constexpr auto CYAN_DARK  = "#00aaaa";
    constexpr auto BLUE       = "#1e90ff";
    constexpr auto BLUE_DARK  = "#1060a0";
    constexpr auto GREEN      = "#00ff88";
    constexpr auto GREEN_DARK = "#00aa55";
    constexpr auto MAGENTA    = "#ff00ff";
    constexpr auto MAGENTA_DARK = "#aa00aa";
    constexpr auto ORANGE     = "#ff8800";
    constexpr auto ORANGE_DARK = "#aa5500";
    constexpr auto RED        = "#ff3366";
    constexpr auto RED_DARK   = "#aa2244";
    constexpr auto YELLOW     = "#ffff00";
    constexpr auto PURPLE     = "#aa00ff";
    constexpr auto TEXT       = "#b0c0d0";
    constexpr auto TEXT_DIM   = "#405060";
    constexpr auto TEXT_BRIGHT= "#ffffff";
    constexpr auto TEXT_NEON  = "#e0f8ff";
}

class CyberStyle {
public:
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

    static QString panelStyle() {
        return QString("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                      "stop:0 %1, stop:1 %2); "
                      "border: 1px solid %3; border-radius: 8px;")
            .arg(Cyber::BG_PANEL).arg(Cyber::BG_DEEP).arg(Cyber::BORDER_LIT);
    }

    static QString neonTitle() {
        return QString("color: %1; font-size: 18px; font-weight: bold; "
                      "background: transparent; border: none; "
                      "padding: 4px 8px;")
            .arg(Cyber::CYAN);
    }

    static QString sectionTitle() {
        return QString("color: %1; font-size: 13px; font-weight: bold; "
                      "background: transparent; border: none; "
                      "padding: 2px 0px; border-left: 3px solid %1; "
                      "margin-left: 4px;")
            .arg(Cyber::CYAN);
    }

    static QString accentButton() {
        return QString(
            "QPushButton { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0,255,255,0.05), stop:1 rgba(0,255,255,0.15)); "
            "  color: %1; border: 1px solid %1; border-radius: 4px; "
            "}"
            "QPushButton:hover { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0,255,255,0.2), stop:1 rgba(0,255,255,0.35)); "
            "  border-color: %1; box-shadow: 0 0 8px rgba(0,255,255,0.4); "
            "}"
            "QPushButton:pressed { "
            "  background: rgba(0,255,255,0.4); border-color: %1; "
            "}"
        ).arg(Cyber::CYAN);
    }

    static QString greenButton() {
        return QString(
            "QPushButton { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0,255,136,0.05), stop:1 rgba(0,255,136,0.15)); "
            "  color: %1; border: 1px solid %1; border-radius: 4px; "
            "}"
            "QPushButton:hover { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0,255,136,0.2), stop:1 rgba(0,255,136,0.35)); "
            "  border-color: %1; box-shadow: 0 0 8px rgba(0,255,136,0.4); "
            "}"
            "QPushButton:pressed { "
            "  background: rgba(0,255,136,0.4); border-color: %1; "
            "}"
        ).arg(Cyber::GREEN);
    }

    static QString redButton() {
        return QString(
            "QPushButton { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,51,102,0.05), stop:1 rgba(255,51,102,0.15)); "
            "  color: %1; border: 1px solid %1; border-radius: 4px; "
            "}"
            "QPushButton:hover { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,51,102,0.2), stop:1 rgba(255,51,102,0.35)); "
            "  border-color: %1; box-shadow: 0 0 8px rgba(255,51,102,0.4); "
            "}"
            "QPushButton:pressed { "
            "  background: rgba(255,51,102,0.4); border-color: %1; "
            "}"
        ).arg(Cyber::RED);
    }

    static QString orangeButton() {
        return QString(
            "QPushButton { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,136,0,0.05), stop:1 rgba(255,136,0,0.15)); "
            "  color: %1; border: 1px solid %1; border-radius: 4px; "
            "}"
            "QPushButton:hover { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,136,0,0.2), stop:1 rgba(255,136,0,0.35)); "
            "  border-color: %1; box-shadow: 0 0 8px rgba(255,136,0,0.4); "
            "}"
            "QPushButton:pressed { "
            "  background: rgba(255,136,0,0.4); border-color: %1; "
            "}"
        ).arg(Cyber::ORANGE);
    }

    static QString magentaButton() {
        return QString(
            "QPushButton { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,0,255,0.05), stop:1 rgba(255,0,255,0.15)); "
            "  color: %1; border: 1px solid %1; border-radius: 4px; "
            "}"
            "QPushButton:hover { "
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,0,255,0.2), stop:1 rgba(255,0,255,0.35)); "
            "  border-color: %1; box-shadow: 0 0 8px rgba(255,0,255,0.4); "
            "}"
            "QPushButton:pressed { "
            "  background: rgba(255,0,255,0.4); border-color: %1; "
            "}"
        ).arg(Cyber::MAGENTA);
    }

    static QString holographicCard() {
        return QString(
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
            "stop:0 rgba(13,21,37,0.9), stop:1 rgba(5,8,16,0.95)); "
            "border: 1px solid %1; border-radius: 6px;"
        ).arg(Cyber::BORDER_LIT);
    }
};

#endif
