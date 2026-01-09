set(TEST tst_BookmarksFolderModelTests)
add_executable(${TEST} tst_BookmarksFolderModelTests.cpp)
include_directories(${webbrowser-app_SOURCE_DIR})
target_link_libraries(${TEST}
    Qt::Core
    Qt::Sql
    Qt::Test
    morph-browser-models
)
add_test(${TEST} ${CMAKE_CURRENT_BINARY_DIR}/${TEST})
