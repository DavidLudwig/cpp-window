
# Use the implementations's parent-directory-name within its target's name
get_filename_component(DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" DIR ${DIR})

find_package(SDL2 REQUIRED)

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
target_link_libraries(ui_proposal-${DIR} PUBLIC ${SDL2_LIBRARY})
set_property(TARGET ui_proposal-${DIR} PROPERTY CXX_STANDARD 17)
