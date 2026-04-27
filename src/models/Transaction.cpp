#include "Transaction.h"

QString typeToString(TransactionType type) {
    return type == TransactionType::INCOME ? "Income" : "Expense";
}

Transaction::Transaction(int id, const QString& title, double amount,
                         TransactionType type, const QString& category, const QDate& date)
    : m_id(id), m_title(title), m_amount(amount),
      m_type(type), m_category(category), m_date(date) {}

Transaction::Transaction(const QString& title, double amount,
                         TransactionType type, const QString& category, const QDate& date)
    : m_id(0), m_title(title), m_amount(amount),
      m_type(type), m_category(category), m_date(date) {}

QString Transaction::formattedAmount() const {
    return QString("$%1").arg(m_amount, 0, 'f', 2);
}
