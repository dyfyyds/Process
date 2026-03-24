#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ScheDuler.h"

#include <QMainWindow>
#include <memory>

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

private:
    void GuiInit();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<Scheduler> m_scheduler;

};
#endif // MAINWINDOW_H
