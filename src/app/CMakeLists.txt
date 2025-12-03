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
    Qt::Gui
    Qt::Network
    Qt::Qml
    Qt::Quick
    Qt::Sql
    Qt::Widgets
    Qt::WebEngineCore
    ${LIBAPPARMOR_LDFLAGS}
    ${LIBPSL_LDFLAGS}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    target_link_libraries(${COMMONLIB} Qt::WebEngineQuick ${LIBPSL_LIBRARIES})
else()
    target_link_libraries(${COMMONLIB} Qt::WebEngine)
endif()

install(DIRECTORY qml-qt${QT_VERSION_MAJOR}/ DESTINATION ${CMAKE_INSTALL_DATADIR}/morph-browser)

add_subdirectory(webbrowser)
add_subdirectory(webcontainer)
