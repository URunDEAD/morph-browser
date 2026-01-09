set(TEST tst_HistoryDomainListModelTests)
add_executable(${TEST} tst_HistoryDomainListModelTests.cpp)
include_directories(${webbrowser-app_SOURCE_DIR} ${webbrowser-common_SOURCE_DIR})
target_link_libraries(${TEST}
    Qt::Core
    Qt::Sql
    Qt::Test
    morph-browser-models
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
