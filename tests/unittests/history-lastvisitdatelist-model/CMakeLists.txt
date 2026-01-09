set(TEST tst_HistoryLastVisitDateListModelTests)
add_executable(${TEST} tst_HistoryLastVisitDateListModelTests.cpp)
include_directories(${webbrowser-common_SOURCE_DIR} ${webbrowser-app_SOURCE_DIR})
target_link_libraries(${TEST}
    morph-browser-models
    Qt::Sql
    Qt::Test
)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    target_include_directories(${TEST} PRIVATE
        ${LIBPSL_INCLUDE_DIRS}
    )
    target_link_libraries(${TEST}
        ${LIBPSL_LDFLAGS}
        ${LIBPSL_LIBRARIES}
    )
endif()

add_test(${TEST} ${CMAKE_CURRENT_BINARY_DIR}/${TEST})
