project(morph-web-plugin)

set(MORPH_WEB_IMPORTS_DIR "${QT_INSTALL_QML}/Morph/Web")
set(ASSETS
    assets/handle@27.png
    assets/scrollbar-theme.js
)

set(PLUGIN morph-web-plugin)

set(PLUGIN_SRC plugin.cpp)

add_library(${PLUGIN} MODULE ${PLUGIN_SRC})
target_link_libraries(${PLUGIN}
    Qt::Core
    Qt::Gui
    Qt::Qml
)

install(TARGETS ${PLUGIN} DESTINATION ${MORPH_WEB_IMPORTS_DIR})
install(DIRECTORY qml-qt${QT_VERSION_MAJOR}/ DESTINATION ${MORPH_WEB_IMPORTS_DIR})
install(FILES ${ASSETS} DESTINATION ${MORPH_WEB_IMPORTS_DIR}/assets)

if(NOT ${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  # copy qml files over to build dir to be able to import them uninstalled
  add_custom_target(morph_web-copy_files_to_build_dir
    DEPENDS qml-qt${QT_VERSION_MAJOR}/ ${ASSETS}

    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/qml-qt${QT_VERSION_MAJOR}/
                ${CMAKE_CURRENT_BINARY_DIR}/

    COMMAND ${CMAKE_COMMAND} -E make_directory
                ${CMAKE_CURRENT_BINARY_DIR}/assets/

    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/assets/
                ${CMAKE_CURRENT_BINARY_DIR}/assets/
  )
  add_dependencies(${PLUGIN} morph_web-copy_files_to_build_dir)
endif()
