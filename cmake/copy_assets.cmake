macro(copy_assets)
    foreach(ASSET IN ITEMS ${ARGN})
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${ASSET} ${CMAKE_CURRENT_BINARY_DIR}/${ASSET} COPYONLY)
    endforeach()
endmacro()