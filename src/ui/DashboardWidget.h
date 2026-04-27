#pragma once
#include <QWidget>

QT_BEGIN_NAMESPACE
class QChartView;
QT_END_NAMESPACE

class DashboardWidget : public QWidget {
    Q_OBJECT
public:
    explicit DashboardWidget(QWidget* parent = nullptr);

public slots:
    void refresh();

private:
    QChartView* createExpensePieChart();
    QChartView* createMonthlyBarChart();

    QChartView* m_pieView;
    QChartView* m_barView;
};
