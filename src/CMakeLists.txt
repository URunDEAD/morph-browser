add_subdirectory(app)
add_subdirectory(Morph)

if(${QT_VERSION_MAJOR} EQUAL 5)
    # DEPRECATED
    add_subdirectory(Ubuntu)
endif()
