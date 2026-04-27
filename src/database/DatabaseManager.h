#pragma once
#include <QSqlDatabase>
#include <QVector>
#include <QStringList>
#include "models/Transaction.h"

class DatabaseManager {
public:
    static DatabaseManager& instance();

    bool initialize(const QString& dbPath);
    bool createTables();

    bool addTransaction(const Transaction& t);
    bool updateTransaction(const Transaction& t);
    bool deleteTransaction(int id);
    QVector<Transaction> getAllTransactions();
    QVector<Transaction> getTransactionsByMonth(int year, int month);

    double getTotalIncome();
    double getTotalExpenses();
    double getBalance();
    double getSpentByCategory(const QString& category);
    QStringList getAllCategories();

    QString lastError() const;

private:
    DatabaseManager() = default;
    QSqlDatabase m_db;
    QString      m_lastError;
};
