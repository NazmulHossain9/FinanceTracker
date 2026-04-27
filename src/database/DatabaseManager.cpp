#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager dm;
    return dm;
}

bool DatabaseManager::initialize(const QString& dbPath) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);
    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        return false;
    }
    return createTables();
}

bool DatabaseManager::createTables() {
    QSqlQuery query;
    bool ok = query.exec(R"(
        CREATE TABLE IF NOT EXISTS transactions (
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            title       TEXT NOT NULL,
            amount      REAL NOT NULL,
            type        TEXT NOT NULL,
            category    TEXT,
            date        TEXT NOT NULL
        )
    )");
    if (!ok) m_lastError = query.lastError().text();
    return ok;
}

bool DatabaseManager::addTransaction(const Transaction& t) {
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO transactions (title, amount, type, category, date)
        VALUES (:title, :amount, :type, :category, :date)
    )");
    query.bindValue(":title",    t.getTitle());
    query.bindValue(":amount",   t.getAmount());
    query.bindValue(":type",     t.isExpense() ? "EXPENSE" : "INCOME");
    query.bindValue(":category", t.getCategory());
    query.bindValue(":date",     t.getDate().toString(Qt::ISODate));
    bool ok = query.exec();
    if (!ok) m_lastError = query.lastError().text();
    return ok;
}

bool DatabaseManager::updateTransaction(const Transaction& t) {
    QSqlQuery query;
    query.prepare(R"(
        UPDATE transactions
        SET title=:title, amount=:amount, type=:type, category=:category, date=:date
        WHERE id=:id
    )");
    query.bindValue(":id",       t.getId());
    query.bindValue(":title",    t.getTitle());
    query.bindValue(":amount",   t.getAmount());
    query.bindValue(":type",     t.isExpense() ? "EXPENSE" : "INCOME");
    query.bindValue(":category", t.getCategory());
    query.bindValue(":date",     t.getDate().toString(Qt::ISODate));
    bool ok = query.exec();
    if (!ok) m_lastError = query.lastError().text();
    return ok;
}

bool DatabaseManager::deleteTransaction(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM transactions WHERE id = :id");
    query.bindValue(":id", id);
    bool ok = query.exec();
    if (!ok) m_lastError = query.lastError().text();
    return ok;
}

QVector<Transaction> DatabaseManager::getAllTransactions() {
    QVector<Transaction> list;
    QSqlQuery query("SELECT * FROM transactions ORDER BY date DESC");
    while (query.next()) {
        TransactionType type = (query.value("type").toString() == "EXPENSE")
            ? TransactionType::EXPENSE : TransactionType::INCOME;
        list.append(Transaction(
            query.value("id").toInt(),
            query.value("title").toString(),
            query.value("amount").toDouble(),
            type,
            query.value("category").toString(),
            QDate::fromString(query.value("date").toString(), Qt::ISODate)
        ));
    }
    return list;
}

QVector<Transaction> DatabaseManager::getTransactionsByMonth(int year, int month) {
    QVector<Transaction> list;
    QSqlQuery query;
    query.prepare(R"(
        SELECT * FROM transactions
        WHERE strftime('%Y', date) = :y AND strftime('%m', date) = :m
        ORDER BY date DESC
    )");
    query.bindValue(":y", QString::number(year));
    query.bindValue(":m", QString("%1").arg(month, 2, 10, QChar('0')));
    query.exec();
    while (query.next()) {
        TransactionType type = (query.value("type").toString() == "EXPENSE")
            ? TransactionType::EXPENSE : TransactionType::INCOME;
        list.append(Transaction(
            query.value("id").toInt(),
            query.value("title").toString(),
            query.value("amount").toDouble(),
            type,
            query.value("category").toString(),
            QDate::fromString(query.value("date").toString(), Qt::ISODate)
        ));
    }
    return list;
}

double DatabaseManager::getTotalIncome() {
    QSqlQuery query("SELECT SUM(amount) FROM transactions WHERE type = 'INCOME'");
    return query.next() ? query.value(0).toDouble() : 0.0;
}

double DatabaseManager::getTotalExpenses() {
    QSqlQuery query("SELECT SUM(amount) FROM transactions WHERE type = 'EXPENSE'");
    return query.next() ? query.value(0).toDouble() : 0.0;
}

double DatabaseManager::getBalance() {
    return getTotalIncome() - getTotalExpenses();
}

double DatabaseManager::getSpentByCategory(const QString& category) {
    QSqlQuery query;
    query.prepare("SELECT SUM(amount) FROM transactions WHERE type='EXPENSE' AND category=:cat");
    query.bindValue(":cat", category);
    query.exec();
    return query.next() ? query.value(0).toDouble() : 0.0;
}

QStringList DatabaseManager::getAllCategories() {
    QStringList cats;
    QSqlQuery query(
        "SELECT DISTINCT category FROM transactions "
        "WHERE category IS NOT NULL AND category != '' ORDER BY category");
    while (query.next())
        cats << query.value(0).toString();
    return cats;
}

QString DatabaseManager::lastError() const {
    return m_lastError;
}
