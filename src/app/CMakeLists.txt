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
    widevine-plugin-helper.cpp
)

add_library(${COMMONLIB} STATIC ${COMMONLIB_SRC})

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    target_compile_definitions(${COMMONLIB} PRIVATE
        "WIDEVINE_SOURCE_FILE_URL=\"https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb\"")
    target_compile_definitions(${COMMONLIB} PRIVATE
        "WIDEVINE_LIB_PATH=\"/morph-browser/WidevineCdm/_platform_specific/linux_x64/libwidevinecdm.so\"")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    target_compile_definitions(${COMMONLIB} PRIVATE
        "WIDEVINE_SOURCE_FILE_URL=\"https://commondatastorage.googleapis.com/chromeos-localmirror/distfiles/chromeos-lacros-arm64-squash-zstd-120.0.6098.0\"")
    target_compile_definitions(${COMMONLIB} PRIVATE
        "WIDEVINE_LIB_PATH=\"/morph-browser/WidevineCdm/_platform_specific/cros_arm64/libwidevinecdm.so\"")
else()
    target_compile_definitions(${COMMONLIB} PRIVATE "WIDEVINE_SOURCE_FILE_URL=\"\"")
    target_compile_definitions(${COMMONLIB} PRIVATE "WIDEVINE_LIB_PATH=\"\"")
endif()

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

# Copy QML files for tests; mimic file structure once installed.
add_custom_target(src_app-copy_files_to_build_dir ALL
    DEPENDS qml-qt${QT_VERSION_MAJOR}/

    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/qml-qt${QT_VERSION_MAJOR}/
                ${CMAKE_CURRENT_BINARY_DIR}/
)

add_subdirectory(webbrowser)
add_subdirectory(webcontainer)
