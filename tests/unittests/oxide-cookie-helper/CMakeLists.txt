set(TEST tst_OxideCookieHelperTests)
set(SOURCES
    ${webapp-container_SOURCE_DIR}/oxide-cookie-helper.cpp
    tst_OxideCookieHelper.cpp
)
add_executable(${TEST} ${SOURCES})
include_directories(${webapp-container_SOURCE_DIR})
target_link_libraries(${TEST}
    Qt::Core
    Qt::Network
    Qt::Test
)
add_test(${TEST} ${CMAKE_CURRENT_BINARY_DIR}/${TEST})
