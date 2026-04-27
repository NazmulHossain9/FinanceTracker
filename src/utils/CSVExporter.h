#pragma once
#include <QString>
#include <QVector>
#include "models/Transaction.h"

class CSVExporter {
public:
    static bool exportTransactions(const QVector<Transaction>& transactions,
                                   const QString& filePath);
};
