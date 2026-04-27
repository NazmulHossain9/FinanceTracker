#include "DashboardWidget.h"
#include "database/DatabaseManager.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

DashboardWidget::DashboardWidget(QWidget* parent) : QWidget(parent) {
    auto* layout = new QHBoxLayout(this);
    m_pieView = createExpensePieChart();
    m_barView = createMonthlyBarChart();
    layout->addWidget(m_pieView);
    layout->addWidget(m_barView);
}

void DashboardWidget::refresh() {
    auto* hbox = qobject_cast<QHBoxLayout*>(layout());
    if (!hbox) return;

    while (hbox->count())
        delete hbox->takeAt(0)->widget();

    m_pieView = createExpensePieChart();
    m_barView = createMonthlyBarChart();
    hbox->addWidget(m_pieView);
    hbox->addWidget(m_barView);
}

QChartView* DashboardWidget::createExpensePieChart() {
    auto* series = new QPieSeries();

    auto& db = DatabaseManager::instance();
    for (const QString& cat : db.getAllCategories()) {
        double spent = db.getSpentByCategory(cat);
        if (spent > 0.0)
            series->append(cat, spent);
    }
    series->setLabelsVisible(true);
    for (auto* slice : series->slices())
        slice->setLabel(QString("%1\n$%2")
            .arg(slice->label())
            .arg(slice->value(), 0, 'f', 0));

    auto* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Spending by Category");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    auto* view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    return view;
}

QChartView* DashboardWidget::createMonthlyBarChart() {
    auto* incomeSet  = new QBarSet("Income");
    auto* expenseSet = new QBarSet("Expenses");
    incomeSet->setColor(QColor(34, 139, 34));   // forest green
    expenseSet->setColor(QColor(200, 50, 50));   // red

    QStringList months;
    QDate today = QDate::currentDate();

    for (int i = 5; i >= 0; --i) {
        QDate d = today.addMonths(-i);
        auto txns = DatabaseManager::instance().getTransactionsByMonth(d.year(), d.month());
        double inc = 0.0, exp = 0.0;
        for (const auto& t : txns) {
            if (t.isExpense()) exp += t.getAmount();
            else               inc += t.getAmount();
        }
        *incomeSet  << inc;
        *expenseSet << exp;
        months << d.toString("MMM yy");
    }

    auto* series = new QBarSeries();
    series->append(incomeSet);
    series->append(expenseSet);

    auto* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Income vs Expenses — Last 6 Months");

    auto* axisX = new QBarCategoryAxis();
    axisX->append(months);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto* axisY = new QValueAxis();
    axisY->setTitleText("Amount ($)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);

    auto* view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    return view;
}
