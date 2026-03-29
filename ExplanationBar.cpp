#include "ExplanationBar.h"
#include "CyberStyle.h"
#include <QHBoxLayout>
#include <QPropertyAnimation>

ExplanationBar::ExplanationBar(QWidget* parent) : QWidget(parent) {
    setFixedHeight(45);
    setStyleSheet(QString(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 %1, stop:1 %2); "
        "border: 1px solid %3; border-radius: 6px;")
        .arg(Cyber::BG_PANEL).arg(Cyber::BG_DEEP).arg(Cyber::BORDER_LIT));

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(15, 6, 15, 6);
    layout->setSpacing(10);

    m_icon = new QLabel(QString::fromUtf8("◈"), this);
    m_icon->setStyleSheet(QString(
        "color: %1; font-size: 16px; font-weight: bold; "
        "background: transparent; border: none;")
        .arg(Cyber::GREEN));
    m_icon->setFixedWidth(25);
    layout->addWidget(m_icon);

    m_text = new QLabel(this);
    m_text->setStyleSheet(QString(
        "color: %1; font-size: 12px; background: transparent; border: none; "
        "font-family: 'Consolas', monospace;")
        .arg(Cyber::TEXT_NEON));
    m_text->setWordWrap(true);
    m_text->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    layout->addWidget(m_text, 1);

    m_typeTimer = new QTimer(this);
    m_typeTimer->setInterval(30);
    connect(m_typeTimer, &QTimer::timeout, this, &ExplanationBar::typeNextChar);

    clear();
}

void ExplanationBar::typeNextChar() {
    if(m_charIndex < m_fullText.length()) {
        m_displayedText += m_fullText[m_charIndex];
        m_text->setText(m_displayedText);
        m_charIndex++;
    } else {
        m_typeTimer->stop();
        m_isTyping = false;
    }
}

void ExplanationBar::setText(const QString& text) {
    m_fullText = text;
    m_displayedText.clear();
    m_charIndex = 0;
    
    if(m_isTyping) {
        m_typeTimer->stop();
    }
    
    m_isTyping = true;
    m_typeTimer->start();
}

void ExplanationBar::clear() {
    m_fullText = QString::fromUtf8("// 等待操作... 添加进程并开始调度以查看步骤解释。");
    m_displayedText = m_fullText;
    m_charIndex = m_fullText.length();
    m_text->setText(m_displayedText);
    m_isTyping = false;
    m_typeTimer->stop();
}
