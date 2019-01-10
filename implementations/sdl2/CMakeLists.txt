
# Use the implementations's parent-directory-name within its target's name
get_filename_component(DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" DIR ${DIR})

if(NOT EMSCRIPTEN)  # Our FindSDL2.cmake doesn't (yet, as of 2019-Feb-10) support Emscripten
    find_package(SDL2 REQUIRED)
endif()

add_library(ui_proposal-${DIR}
    ui.cpp
)
target_include_directories(ui_proposal-${DIR}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
        ${SDL2_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
if (EMSCRIPTEN)
    set_property(TARGET ui_proposal-${DIR} APPEND PROPERTY COMPILE_OPTIONS -s USE_SDL=2)
    #
    # HACK: Use target_link_libraries, and not set_property (w/ LINK_OPTIONS),
    # to make '-s USE_SDL=2' be passed to client apps
    #
    target_link_libraries(ui_proposal-${DIR} PUBLIC "-s USE_SDL=2")
else()
    target_link_libraries(ui_proposal-${DIR} PUBLIC ${SDL2_LIBRARY})
endif()
set_property(TARGET ui_proposal-${DIR} PROPERTY CXX_STANDARD 17)
