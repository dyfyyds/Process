#ifndef COMPARISONDIALOG_H
#define COMPARISONDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <vector>
#include "Process.h"
#include "ScheDuler.h"

class ComparisonDialog : public QDialog {
    Q_OBJECT
public:
    explicit ComparisonDialog(const std::vector<Process*>& processes, QWidget* parent = nullptr);

private:
    void runComparison(const std::vector<Process*>& processes);
    QTableWidget* m_table;
};

#endif
