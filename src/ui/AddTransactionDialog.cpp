#include "AddTransactionDialog.h"
#include "database/DatabaseManager.h"
#include "models/Transaction.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>

AddTransactionDialog::AddTransactionDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Add Transaction");
    setMinimumWidth(360);

    m_titleEdit = new QLineEdit(this);
    m_titleEdit->setPlaceholderText("e.g. Monthly rent");

    m_amountSpin = new QDoubleSpinBox(this);
    m_amountSpin->setRange(0.01, 1'000'000.00);
    m_amountSpin->setDecimals(2);
    m_amountSpin->setPrefix("$ ");

    m_typeCombo = new QComboBox(this);
    m_typeCombo->addItems({"Expense", "Income"});

    m_categoryCombo = new QComboBox(this);
    m_categoryCombo->setEditable(true);
    m_categoryCombo->addItems({
        "Food", "Transport", "Rent", "Entertainment",
        "Health", "Salary", "Freelance", "Utilities", "Other"
    });

    m_dateEdit = new QDateEdit(QDate::currentDate(), this);
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDisplayFormat("yyyy-MM-dd");

    auto* form = new QFormLayout();
    form->addRow("Title:",    m_titleEdit);
    form->addRow("Amount:",   m_amountSpin);
    form->addRow("Type:",     m_typeCombo);
    form->addRow("Category:", m_categoryCombo);
    form->addRow("Date:",     m_dateEdit);

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &AddTransactionDialog::onAccept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(buttons);
}

void AddTransactionDialog::onAccept() {
    QString title = m_titleEdit->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Please enter a title.");
        return;
    }

    TransactionType type = (m_typeCombo->currentText() == "Expense")
        ? TransactionType::EXPENSE : TransactionType::INCOME;

    Transaction t(title,
                  m_amountSpin->value(),
                  type,
                  m_categoryCombo->currentText().trimmed(),
                  m_dateEdit->date());

    auto& db = DatabaseManager::instance();
    if (!db.addTransaction(t)) {
        QMessageBox::critical(this, "Error",
            "Failed to save transaction:\n" + db.lastError());
        return;
    }
    accept();
}
