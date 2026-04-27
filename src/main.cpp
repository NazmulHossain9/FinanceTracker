#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include "database/DatabaseManager.h"
#include "ui/MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("FinanceTracker");
    app.setApplicationVersion("1.0");

    QString dbPath = QDir::homePath() + "/finance_tracker.sqlite";
    auto& db = DatabaseManager::instance();

    if (!db.initialize(dbPath)) {
        QMessageBox::critical(nullptr, "Database Error",
            "Failed to open database:\n" + db.lastError());
        return 1;
    }

    MainWindow window;
    window.show();

    return app.exec();
}
