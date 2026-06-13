
cmake_minimum_required(VERSION 3.15)
project(Chess LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(dependencies/raylib)

add_library(chess_core
    source/core/board.cpp
    source/core/movegen.cpp
    source/core/perft.cpp
    source/engine/engine.cpp
)

add_executable(seachess_uci
    source/main_uci.cpp
)

target_link_libraries(seachess_uci PRIVATE chess_core)

add_executable(seachess
    source/main_gui.cpp
    source/window.cpp
    source/assets.cpp
    source/render.cpp
    source/game_state.cpp
)

target_link_libraries(seachess PRIVATE chess_core raylib)

if (WIN32)
    target_compile_options(seachess PRIVATE /MP)
    target_compile_options(seachess_uci PRIVATE /MP)

    set_target_properties(seachess PROPERTIES WIN32_EXECUTABLE $<$<CONFIG:Release>:TRUE>)
    target_link_options(seachess PRIVATE $<$<CONFIG:Release>:/ENTRY:mainCRTStartup>)
endif()