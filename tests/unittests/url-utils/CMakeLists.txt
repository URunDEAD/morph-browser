set(TEST tst_UrlUtilsTests)
add_executable(${TEST} tst_UrlUtilsTests.cpp)
include_directories(${webbrowser-app_SOURCE_DIR} ${webbrowser-common_SOURCE_DIR})
target_link_libraries(${TEST}
    Qt::Core
    Qt::Sql
    Qt::Test
    webbrowser-common
)
add_test(${TEST} ${CMAKE_CURRENT_BINARY_DIR}/${TEST})
set_tests_properties(${TEST} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=minimal")
