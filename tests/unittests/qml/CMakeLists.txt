find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5QuickTest REQUIRED)
find_package(Qt5Sql REQUIRED)

find_program(XVFBRUN xvfb-run)
if(NOT XVFBRUN)
  message(FATAL_ERROR "Could not find xvfb-run, please install the xvfb package")
endif()
set(XVFB_COMMAND ${XVFBRUN} -s "-screen 0 640x480x24" -a)

set(TEST tst_QmlTests)
set(SOURCES
    ${webbrowser-common_SOURCE_DIR}/favicon-fetcher.cpp
    ${webbrowser-app_SOURCE_DIR}/bookmarks-model.cpp
    ${webbrowser-app_SOURCE_DIR}/bookmarks-folder-model.cpp
    ${webbrowser-app_SOURCE_DIR}/bookmarks-folderlist-model.cpp
    ${webbrowser-common_SOURCE_DIR}/file-operations.cpp
    ${webbrowser-common_SOURCE_DIR}/url-utils.cpp
    ${webbrowser-app_SOURCE_DIR}/history-domain-model.cpp
    ${webbrowser-app_SOURCE_DIR}/history-domainlist-model.cpp
    ${webbrowser-app_SOURCE_DIR}/history-model.cpp
    ${webbrowser-app_SOURCE_DIR}/history-lastvisitdatelist-model.cpp
    ${webbrowser-app_SOURCE_DIR}/limit-proxy-model.cpp
    ${webbrowser-app_SOURCE_DIR}/reparenter.cpp
    ${webbrowser-app_SOURCE_DIR}/searchengine.cpp
    ${webbrowser-app_SOURCE_DIR}/tabs-model.cpp
    ${webbrowser-app_SOURCE_DIR}/text-search-filter-model.cpp
    tst_QmlTests.cpp
)
add_executable(${TEST} ${SOURCES})
include_directories(
    ${webbrowser-common_SOURCE_DIR}
    ${webbrowser-app_SOURCE_DIR}
    ${unity8_SOURCE_DIR}/plugins
)
target_link_libraries(${TEST}
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickTest
    Qt5::Sql
)

# Copy QML test files to build directory. This lets test files resolve import
# path relatively independent of Qt version.
file(GLOB QML_FILES *.qml)
add_custom_target(unittests_qml-copy_files_to_build_dir
    DEPENDS ${QML_FILES}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${QML_FILES}
            ${CMAKE_CURRENT_BINARY_DIR}/
)
add_dependencies(${TEST} unittests_qml-copy_files_to_build_dir)

add_test(${TEST} ${XVFB_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/${TEST}
         -input ${CMAKE_CURRENT_BINARY_DIR}
         -import ${CMAKE_BINARY_DIR}/src)
