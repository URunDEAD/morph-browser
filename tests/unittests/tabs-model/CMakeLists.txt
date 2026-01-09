set(TEST tst_TabsModelTests)
add_executable(${TEST} tst_TabsModelTests.cpp)
include_directories(${webbrowser-app_SOURCE_DIR})
target_link_libraries(${TEST}
    Qt::Core
    Qt::Qml
    Qt::Quick
    Qt::Sql
    Qt::Test
    morph-browser-models
)
add_test(${TEST} ${CMAKE_CURRENT_BINARY_DIR}/${TEST})
set_tests_properties(${TEST} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=minimal")
