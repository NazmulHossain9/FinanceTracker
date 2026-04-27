#pragma once
#include <QSqlDatabase>
#include <QVector>
#include "models/Transaction.h"

class DatabaseManager {
public:
    static DatabaseManager& instance();  // Singleton

    bool initialize(const QString& dbPath);
    bool createTables();

    // CRUD operations
    bool addTransaction(const Transaction& t);
    bool deleteTransaction(int id);
    QVector<Transaction> getAllTransactions();
    QVector<Transaction> getTransactionsByMonth(int year, int month);

    double getTotalIncome();
    double getTotalExpenses();
    double getSpentByCategory(const QString& category);

private:
    DatabaseManager() = default;
    QSqlDatabase m_db;
};