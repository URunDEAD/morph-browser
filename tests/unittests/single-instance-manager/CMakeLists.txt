set(TEST tst_SingleInstanceManagerTests)
set(SOURCES
    ${webbrowser-common_SOURCE_DIR}/single-instance-manager.cpp
    tst_SingleInstanceManagerTests.cpp
)
add_executable(${TEST} ${SOURCES})
include_directories(${webbrowser-common_SOURCE_DIR})
target_link_libraries(${TEST}
    Qt::Core
    Qt::Network
    Qt::Test
)
add_test(${TEST} ${CMAKE_CURRENT_BINARY_DIR}/${TEST})
set_tests_properties(${TEST} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=minimal")
