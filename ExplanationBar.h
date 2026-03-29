#ifndef EXPLANATIONBAR_H
#define EXPLANATIONBAR_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

class ExplanationBar : public QWidget {
    Q_OBJECT
public:
    explicit ExplanationBar(QWidget* parent = nullptr);
    void setText(const QString& text);
    void clear();

private slots:
    void typeNextChar();

private:
    QLabel* m_icon;
    QLabel* m_text;
    QString m_fullText;
    QString m_displayedText;
    int m_charIndex = 0;
    QTimer* m_typeTimer;
    bool m_isTyping = false;
};

#endif
