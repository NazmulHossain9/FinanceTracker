#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onAddTransaction();
    void onDeleteTransaction();
    void onExportCSV();
    void onShowDashboard();
    void refreshTable();
    void updateSummary();

private:
    void setupUI();
    void setupMenuBar();

    QTableWidget* m_table;
    QLabel*       m_balanceLabel;
    QLabel*       m_incomeLabel;
    QLabel*       m_expenseLabel;
    QPushButton*  m_addBtn;
    QPushButton*  m_deleteBtn;
};
