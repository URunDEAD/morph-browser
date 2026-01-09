set(TEST tst_WebappContainerColorTests)
set(SOURCES
    ${webapp-container_SOURCE_DIR}/webapp-container-helper.cpp
    tst_WebappContainerColorTests.cpp
)
add_executable(${TEST} ${SOURCES})
include_directories(${webapp-container_SOURCE_DIR})
target_link_libraries(${TEST}
    Qt::Core
    Qt::Gui
    Qt::Test
)
add_test(${TEST} ${CMAKE_CURRENT_BINARY_DIR}/${TEST})
set_tests_properties(${TEST} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=minimal")
