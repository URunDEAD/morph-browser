project(webbrowser-common)

configure_file(
    config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    @ONLY)

set(COMMONLIB webbrowser-common)

set(COMMONLIB_SRC
    browserapplication.cpp
    browser-utils.cpp
    domain-permissions-model.cpp
    domain-settings-model.cpp
    domain-settings-sorted-model.cpp
    domain-settings-user-agents-model.cpp
    downloads-model.cpp
    favicon-fetcher.cpp
    file-operations.cpp
    input-method-handler.cpp
    meminfo.cpp
    mime-database.cpp
    session-storage.cpp
    single-instance-manager.cpp
    url-utils.cpp
)

add_library(${COMMONLIB} STATIC ${COMMONLIB_SRC})

include_directories(${LIBAPPARMOR_INCLUDE_DIRS})
target_link_libraries(${COMMONLIB}
    Qt::Core
    Qt::CorePrivate
    Qt::Gui
    Qt::Network
    Qt::NetworkPrivate
    Qt::Qml
    Qt::Quick
    Qt::Sql
    Qt::Widgets
    Qt::WebEngineCore
    ${LIBAPPARMOR_LDFLAGS}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    target_link_libraries(${COMMONLIB} Qt::WebEngineCore Qt::WebEngineQuick)
else()
    target_link_libraries(${COMMONLIB} Qt::WebEngine Qt::WebEngineCore)
endif()

file(GLOB QML_FILES *.qml qmldir)
install(FILES ${QML_FILES} DESTINATION ${CMAKE_INSTALL_DATADIR}/morph-browser)
file(GLOB JS_FILES *.js)
install(FILES ${JS_FILES} DESTINATION ${CMAKE_INSTALL_DATADIR}/morph-browser)
install(DIRECTORY actions DESTINATION ${CMAKE_INSTALL_DATADIR}/morph-browser
        FILES_MATCHING PATTERN *.qml)

add_subdirectory(webbrowser)
add_subdirectory(webcontainer)
