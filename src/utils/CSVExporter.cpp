#include "CSVExporter.h"
#include <QFile>
#include <QTextStream>

bool CSVExporter::exportTransactions(const QVector<Transaction>& transactions,
                                     const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out << "Date,Title,Category,Type,Amount\n";

    for (const auto& t : transactions) {
        QString title = t.getTitle();
        title.replace('"', "\"\"");   // escape double-quotes per RFC 4180
        out << t.getDate().toString("yyyy-MM-dd") << ","
            << "\"" << title << "\","
            << t.getCategory() << ","
            << (t.isExpense() ? "Expense" : "Income") << ","
            << QString::number(t.getAmount(), 'f', 2) << "\n";
    }
    return true;
}
