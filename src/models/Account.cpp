#include "Account.h"

Account::Account(int id, const QString& name, AccountType type, double balance)
    : m_id(id), m_name(name), m_type(type), m_balance(balance) {}

void Account::deposit(double amount) {
    if (amount > 0) m_balance += amount;
}

void Account::withdraw(double amount) {
    if (amount > 0) m_balance -= amount;
}
