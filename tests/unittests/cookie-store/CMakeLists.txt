set(TEST tst_CookieStoreTests)
set(SOURCES
    ${webapp-container_SOURCE_DIR}/chrome-cookie-store.cpp
    ${webapp-container_SOURCE_DIR}/cookie-store.cpp
    ${webapp-container_SOURCE_DIR}/oxide-cookie-helper.cpp
    tst_CookieStore.cpp
)
add_executable(${TEST} ${SOURCES})
include_directories(${webapp-container_SOURCE_DIR})
target_link_libraries(${TEST}
    Qt::Core
    Qt::Network
    Qt::Sql
    Qt::Test
)
add_test(${TEST} ${CMAKE_CURRENT_BINARY_DIR}/${TEST})
