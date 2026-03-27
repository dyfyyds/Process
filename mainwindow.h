#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AddProcessWidget.h"
#include "ScheDuler.h"

#include <QMainWindow>
#include <QTimer>
#include <memory>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_AddProcessButton_clicked();

    void on_SetSingleButton_clicked();

    void on_SetAutoButton_clicked();

    void on_RestButton_clicked();

    void on_CloseButton_clicked();

    void on_MiniButton_clicked();

protected:
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

private:
    void GuiInit();
    void AddTableValue(QString);
    bool containsDuplicateNames(QString name);

signals:
    void isExecute(bool);

private:
    Ui::MainWindow *ui;
    std::shared_ptr<Scheduler> m_scheduler;
    QTimer *m_timer;

    bool isStop;

    bool isDrag;
    QPointF dVal;

    bool isExist;

    QSet<QString> m_existName;

};
#endif // MAINWINDOW_H
