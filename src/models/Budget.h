#pragma once
#include <QString>
#include <QMap>

class Budget {
public:
    void setCategoryLimit(const QString& category, double limit);
    double getLimit(const QString& category) const;
    bool isOverBudget(const QString& category, double spent) const;
    QMap<QString, double> getAllLimits() const { return m_limits; }

private:
    QMap<QString, double> m_limits;
};