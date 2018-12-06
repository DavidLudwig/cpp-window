
# Use the sample's parent-directory-name within its target's name
get_filename_component(DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" DIR ${DIR})

foreach(UI_IMPLEMENTATION IN LISTS UI_IMPLEMENTATIONS)
    set(APP_TARGET "${DIR}-${UI_IMPLEMENTATION}")
    add_executable(${APP_TARGET} main.cpp)
    target_include_directories(${APP_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../implementations/${UI_IMPLEMENTATION})
    target_link_libraries(${APP_TARGET} ui_proposal-${UI_IMPLEMENTATION})
    set_property(TARGET ${APP_TARGET} PROPERTY CXX_STANDARD 17)
    install (TARGETS ${APP_TARGET} RUNTIME DESTINATION bin)
endforeach()
