#include <QTest>
#include "models/Transaction.h"
#include "models/Budget.h"

class TestTransaction : public QObject {
    Q_OBJECT

private slots:
    void constructorWithId();
    void constructorWithoutId();
    void isExpense();
    void formattedAmount();
    void typeToStringHelper();
    void budgetSetAndGet();
    void budgetOverLimit();
    void budgetNoLimitSet();
};

void TestTransaction::constructorWithId() {
    Transaction t(42, "Groceries", 75.50, TransactionType::EXPENSE, "Food", QDate(2026, 4, 1));
    QCOMPARE(t.getId(),       42);
    QCOMPARE(t.getTitle(),    QString("Groceries"));
    QCOMPARE(t.getAmount(),   75.50);
    QCOMPARE(t.getCategory(), QString("Food"));
    QCOMPARE(t.getDate(),     QDate(2026, 4, 1));
}

void TestTransaction::constructorWithoutId() {
    Transaction t("Salary", 3000.0, TransactionType::INCOME, "Salary", QDate::currentDate());
    QCOMPARE(t.getId(), 0);
    QVERIFY(!t.isExpense());
}

void TestTransaction::isExpense() {
    Transaction expense(1, "Rent", 1200.0, TransactionType::EXPENSE, "Rent", QDate::currentDate());
    Transaction income (2, "Pay",  2500.0, TransactionType::INCOME,  "Job",  QDate::currentDate());
    QVERIFY( expense.isExpense());
    QVERIFY(!income.isExpense());
}

void TestTransaction::formattedAmount() {
    Transaction t(1, "Test", 99.5, TransactionType::EXPENSE, "Other", QDate::currentDate());
    QCOMPARE(t.formattedAmount(), QString("$99.50"));

    Transaction t2(2, "Test2", 1000.0, TransactionType::INCOME, "Salary", QDate::currentDate());
    QCOMPARE(t2.formattedAmount(), QString("$1000.00"));
}

void TestTransaction::typeToStringHelper() {
    QCOMPARE(typeToString(TransactionType::INCOME),  QString("Income"));
    QCOMPARE(typeToString(TransactionType::EXPENSE), QString("Expense"));
}

void TestTransaction::budgetSetAndGet() {
    Budget b;
    b.setCategoryLimit("Food", 300.0);
    QCOMPARE(b.getLimit("Food"),  300.0);
    QCOMPARE(b.getLimit("Other"), 0.0);
}

void TestTransaction::budgetOverLimit() {
    Budget b;
    b.setCategoryLimit("Food", 300.0);
    QVERIFY(!b.isOverBudget("Food", 200.0));
    QVERIFY(!b.isOverBudget("Food", 300.0));   // exactly at limit — not over
    QVERIFY( b.isOverBudget("Food", 300.01));
}

void TestTransaction::budgetNoLimitSet() {
    Budget b;
    // No limit means never over budget
    QVERIFY(!b.isOverBudget("Transport", 9999.0));
}

QTEST_MAIN(TestTransaction)
#include "test_transaction.moc"
