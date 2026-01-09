set(TEST tst_ContainerUrlPatternsTests)
set(SOURCES
    ${webapp-container_SOURCE_DIR}/url-pattern-utils.cpp
    tst_ContainerUrlPatternsTests.cpp
)
add_executable(${TEST} ${SOURCES})
include_directories(${webapp-container_SOURCE_DIR})
target_link_libraries(${TEST}
    Qt::Core
    Qt::Test
)
add_test(${TEST} ${CMAKE_CURRENT_BINARY_DIR}/${TEST})
