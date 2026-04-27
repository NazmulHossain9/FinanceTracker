#include "Budget.h"

void Budget::setCategoryLimit(const QString& category, double limit) {
    m_limits[category] = limit;
}

double Budget::getLimit(const QString& category) const {
    return m_limits.value(category, 0.0);
}

bool Budget::isOverBudget(const QString& category, double spent) const {
    double limit = getLimit(category);
    return limit > 0.0 && spent > limit;
}
