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
    src/models/Budget.cpp
    src/database/DatabaseManager.cpp
    src/ui/MainWindow.cpp
    src/ui/AddTransactionDialog.cpp
    src/ui/DashboardWidget.cpp
    src/utils/CSVExporter.cpp
)

target_include_directories(FinanceTracker PRIVATE src)

target_link_libraries(FinanceTracker
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    Qt6::Charts
)

# ── Optional unit-test target ─────────────────────────────────────────────────
find_package(Qt6 QUIET COMPONENTS Test)
if(Qt6Test_FOUND)
    add_executable(FinanceTrackerTests
        tests/test_transaction.cpp
        src/models/Transaction.cpp
        src/models/Budget.cpp
    )
    target_include_directories(FinanceTrackerTests PRIVATE src)
    target_link_libraries(FinanceTrackerTests
        Qt6::Core
        Qt6::Test
    )
    enable_testing()
    add_test(NAME UnitTests COMMAND FinanceTrackerTests)
endif()
