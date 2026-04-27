cmake_minimum_required(VERSION 3.16)
project(FinanceTracker VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core Widgets Sql Charts
)

add_executable(FinanceTracker
    src/main.cpp
    src/models/Transaction.cpp
    src/models/Account.cpp
    src/database/DatabaseManager.cpp
    src/ui/MainWindow.cpp
    src/ui/AddTransactionDialog.cpp
    src/ui/DashboardWidget.cpp
    src/utils/CSVExporter.cpp
)

target_link_libraries(FinanceTracker
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    Qt6::Charts
)