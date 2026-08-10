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
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

add_library(${COMMONLIB} STATIC ${COMMONLIB_SRC})

if(DEFINED ENV{ARCH})
    set(TARGET_ARCH "$ENV{ARCH}")
else()
    execute_process(
        COMMAND dpkg-architecture -qDEB_HOST_ARCH
        OUTPUT_VARIABLE TARGET_ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()
if(TARGET_ARCH MATCHES "x86_64|amd64")
    set(WIDEVINE_SOURCE_FILE_URL "https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb")
    set(WIDEVINE_LIB_PATH "/morph-browser/WidevineCdm/_platform_specific/linux_x64/libwidevinecdm.so")
elseif(TARGET_ARCH MATCHES "aarch64|arm64")
    set(WIDEVINE_SOURCE_FILE_URL "https://commondatastorage.googleapis.com/chromeos-localmirror/distfiles/chromeos-lacros-arm64-squash-zstd-120.0.6098.0")
    set(WIDEVINE_LIB_PATH "/morph-browser/WidevineCdm/_platform_specific/cros_arm64/libwidevinecdm.so")
else()
    set(WIDEVINE_SOURCE_FILE_URL "")
    set(WIDEVINE_LIB_PATH "")
endif()

configure_file(
    widevine-plugin-helper-consts.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/widevine-plugin-helper-consts.h
    @ONLY)

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
install(DIRECTORY assets DESTINATION ${CMAKE_INSTALL_DATADIR}/morph-browser)

# Copy QML files for tests; mimic file structure once installed.
add_custom_target(src_app-copy_files_to_build_dir ALL
    DEPENDS qml-qt${QT_VERSION_MAJOR}/

    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/qml-qt${QT_VERSION_MAJOR}/
                ${CMAKE_CURRENT_BINARY_DIR}/
)

add_subdirectory(webbrowser)
add_subdirectory(webcontainer)
