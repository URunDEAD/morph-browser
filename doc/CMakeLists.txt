project(morph-web-doc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR}Tools CONFIG REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ToolsTools CONFIG REQUIRED)
    get_target_property(QDOC_BIN Qt${QT_VERSION_MAJOR}::qdoc LOCATION)
    if (NOT QDOC_BIN)
        message(WARNING "qdoc not found, documentation cannot be built")
    else()
        add_custom_target(doc ALL
                          COMMAND ${QDOC_BIN}
                          -outputdir ${CMAKE_CURRENT_BINARY_DIR}/html
                        ${CMAKE_CURRENT_SOURCE_DIR}/morph-web.qdocconf)
        install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
        DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/morph-web)
    endif()
else()
    find_program(QDOC_BIN qdoc)
    if(QDOC_BIN STREQUAL "QDOC_BIN-NOTFOUND")
        message(WARNING "qdoc not found, documentation cannot be built")
    else()
        add_custom_target(doc ALL
                          COMMAND ${QDOC_BIN} -qt5
                          -outputdir ${CMAKE_CURRENT_BINARY_DIR}/html
                        ${CMAKE_CURRENT_SOURCE_DIR}/morph-web.qdocconf)
        install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
        DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/morph-web)
    endif()
endif()
