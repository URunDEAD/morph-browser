project(webapp-container)

include_directories(
    ${CMAKE_BINARY_DIR}
    ${webbrowser-common_SOURCE_DIR}
    ${webbrowser-common_BINARY_DIR}
)

set(WEBAPP_CONTAINER webapp-container)

set(WEBAPP_CONTAINER_SRC
    chrome-cookie-store.cpp
    cookie-store.cpp
    online-accounts-cookie-store.cpp
    oxide-cookie-helper.cpp
    local-cookie-store.cpp
    webapp-container.cpp
    webapp-container-helper.cpp
    session-utils.cpp
    url-pattern-utils.cpp
    scheme-filter.cpp
    intent-parser.cpp
)

add_executable(${WEBAPP_CONTAINER} ${WEBAPP_CONTAINER_SRC})

target_link_libraries(${WEBAPP_CONTAINER}
    Qt::Core
    Qt::DBus
    Qt::Gui
    Qt::Qml
    Qt::Quick
    Qt::Sql
    ${COMMONLIB}
)

install(TARGETS ${WEBAPP_CONTAINER}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY qml-qt${QT_VERSION_MAJOR}/ DESTINATION ${CMAKE_INSTALL_DATADIR}/morph-browser/webcontainer)
install(DIRECTORY assets
        DESTINATION ${CMAKE_INSTALL_DATADIR}/morph-browser/webcontainer)
