#pragma once
#include <QString>

enum class AccountType { CHECKING, SAVINGS, CREDIT };

class Account {
public:
    Account(int id, const QString& name, AccountType type, double balance = 0.0);

    int         getId()      const { return m_id; }
    QString     getName()    const { return m_name; }
    AccountType getType()    const { return m_type; }
    double      getBalance() const { return m_balance; }

    void deposit(double amount);
    void withdraw(double amount);

private:
    int         m_id;
    QString     m_name;
    AccountType m_type;
    double      m_balance;
};
