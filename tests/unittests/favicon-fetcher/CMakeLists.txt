set(TEST tst_FaviconFetcherTests)
set(SOURCES
    ${webbrowser-common_SOURCE_DIR}/favicon-fetcher.cpp
    tst_FaviconFetcherTests.cpp
)
add_executable(${TEST} ${SOURCES})
include_directories(${webbrowser-common_SOURCE_DIR})
target_link_libraries(${TEST}
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::Test
)
add_test(${TEST} ${CMAKE_CURRENT_BINARY_DIR}/${TEST})
set_tests_properties(${TEST} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=minimal")
