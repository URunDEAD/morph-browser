project(morph-tabsbar-plugin)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

set(MORPH_TABSBAR_IMPORTS_DIR "${QT_INSTALL_QML}/Morph/TabsBar")

set(PLUGIN morph-tabsbar-plugin)

set(PLUGIN_SRC
    drag-helper.cpp
    tabsbar-plugin.cpp
)

add_library(${PLUGIN} MODULE ${PLUGIN_SRC})
target_link_libraries(${PLUGIN}
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
)

file(GLOB PLUGIN_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.qml qmldir *.js *.png)
install(TARGETS ${PLUGIN} DESTINATION ${MORPH_TABSBAR_IMPORTS_DIR})
install(FILES ${PLUGIN_FILES} DESTINATION ${MORPH_TABSBAR_IMPORTS_DIR})

if(NOT ${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  # copy qml files over to build dir to be able to import them uninstalled
  foreach(_file ${PLUGIN_FILES})
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_file}
                       DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_file}
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${_file} ${CMAKE_CURRENT_BINARY_DIR}/${_file})
  endforeach(_file)
  add_custom_target(copy_tabsbar_files_to_build_dir DEPENDS ${PLUGIN_FILES})
  add_dependencies(${PLUGIN} copy_tabsbar_files_to_build_dir)
endif()
