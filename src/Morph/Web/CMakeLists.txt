project(morph-web-plugin)

set(ASSETS
    assets/handle@27.png
)

set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

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
install(FILES ${ASSETS} DESTINATION ${MORPH_WEB_IMPORTS_DIR})

if(NOT ${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  # copy qml files over to build dir to be able to import them uninstalled
  foreach(_file ${PLUGIN_FILES})
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_file}
                       DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_file}
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${_file} ${CMAKE_CURRENT_BINARY_DIR}/${_file})
  endforeach(_file)
  add_custom_target(copy_files_to_build_dir DEPENDS ${PLUGIN_FILES})
  add_dependencies(${PLUGIN} copy_files_to_build_dir)
endif()
