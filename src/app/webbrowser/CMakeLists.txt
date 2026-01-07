project(webbrowser-app)

include_directories(
    ${CMAKE_BINARY_DIR}
    ${webbrowser-common_SOURCE_DIR}
    ${webbrowser-common_BINARY_DIR}
)

set(WEBBROWSER_APP_MODELS_SRC
    bookmarks-model.cpp
    bookmarks-folder-model.cpp
    bookmarks-folderlist-model.cpp
    history-domain-model.cpp
    history-domainlist-model.cpp
    history-lastvisitdatelist-model.cpp
    history-model.cpp
    limit-proxy-model.cpp
    tabs-model.cpp
    text-search-filter-model.cpp
)

set(WEBBROWSER_APP_MODELS morph-browser-models)

set(ASSETS
    assets/private-browsing-exit.svg
    assets/stock_link.svg
    assets/stock_website.png
    assets/toolbar-dropshadow.png
    assets/tab-artwork.png
    assets/bottom_edge_hint@27.png
    assets/broken_lock@27.png
    assets/tab-error@27.png
    assets/tab-shadow-narrow@27.png
    assets/readability-readerable.js
    assets/readability.js
)

add_library(${WEBBROWSER_APP_MODELS} STATIC ${WEBBROWSER_APP_MODELS_SRC})
target_link_libraries(${WEBBROWSER_APP_MODELS}
    Qt::Core
    Qt::Sql
)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    target_include_directories(${WEBBROWSER_APP_MODELS} PRIVATE
        ${LIBPSL_INCLUDE_DIRS}
    )
    target_link_libraries(${WEBBROWSER_APP_MODELS}
        ${LIBPSL_LDFLAGS}
        ${LIBPSL_LIBRARIES}
    )
endif()

set(WEBBROWSER_APP_SRC
    reparenter.cpp
    searchengine.cpp
    morph-browser.cpp
)

set(WEBBROWSER_APP morph-browser)

add_executable(${WEBBROWSER_APP} ${WEBBROWSER_APP_SRC})
target_link_libraries(${WEBBROWSER_APP}
    Qt::Concurrent
    Qt::Core
    Qt::Qml
    Qt::Quick
    ${COMMONLIB}
    ${WEBBROWSER_APP_MODELS}
)

install(TARGETS ${WEBBROWSER_APP}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY qml-qt${QT_VERSION_MAJOR}/ DESTINATION ${CMAKE_INSTALL_DATADIR}/morph-browser/webbrowser)

install(FILES ${ASSETS}
        DESTINATION ${CMAKE_INSTALL_DATADIR}/morph-browser/webbrowser/assets)

install(DIRECTORY searchengines
        DESTINATION ${CMAKE_INSTALL_DATADIR}/morph-browser/webbrowser
        FILES_MATCHING PATTERN *.xml)

configure_file(${DESKTOP_FILE}.in.in ${DESKTOP_FILE}.in @ONLY)
add_custom_target(${DESKTOP_FILE} ALL
    COMMENT "Merging translations into ${DESKTOP_FILE}"
    COMMAND ${GETTEXT_MSGFMT_EXECUTABLE}
            --desktop --template=${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE}.in
            -o ${DESKTOP_FILE}
            -d ${CMAKE_SOURCE_DIR}/po
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE}
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)

install(FILES "morph-browser.url-dispatcher"
        DESTINATION ${CMAKE_INSTALL_DATADIR}/lomiri-url-dispatcher/urls)

install(FILES "morph-browser-content-hub.json"
    DESTINATION ${CMAKE_INSTALL_DATADIR}/lomiri-content-hub/peers
    RENAME morph-browser
    )

if(${CLICK_MODE})
  message("-- Configuring manifest.json")

  execute_process(
    COMMAND dpkg-architecture -qDEB_HOST_ARCH
    OUTPUT_VARIABLE CLICK_ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  configure_file(manifest.json.in ${CMAKE_CURRENT_BINARY_DIR}/manifest.json)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest.json DESTINATION ${CMAKE_INSTALL_PREFIX})
  install(FILES morph-browser.apparmor DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

# Copy QML files for tests; mimic file structure once installed.
add_custom_target(src_app_webbrowser-copy_files_to_build_dir ALL
    DEPENDS qml-qt${QT_VERSION_MAJOR}/

    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/qml-qt${QT_VERSION_MAJOR}/
                ${CMAKE_CURRENT_BINARY_DIR}/

    COMMAND ${CMAKE_COMMAND} -E make_directory
                ${CMAKE_CURRENT_BINARY_DIR}/assets/

    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/assets/
                ${CMAKE_CURRENT_BINARY_DIR}/assets/
)
