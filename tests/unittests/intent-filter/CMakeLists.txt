set(TEST tst_IntentFilterTests)
set(SOURCES
    ${webapp-container_SOURCE_DIR}/intent-parser.cpp
    ${webapp-container_SOURCE_DIR}/scheme-filter.cpp
    tst_IntentFilterTests.cpp
)
include_directories(${webapp-container_SOURCE_DIR})
add_executable(${TEST} ${SOURCES})
target_link_libraries(${TEST}
    Qt::Core
    Qt::Qml
    Qt::Test
)
add_test(${TEST} ${CMAKE_CURRENT_BINARY_DIR}/${TEST})
