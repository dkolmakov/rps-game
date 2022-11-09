cmake_minimum_required(VERSION 3.16)
project(rps_game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -O3)

add_compile_options(
        "-Wall" "-Wpedantic" "-Wextra" "-fexceptions"
        "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
)

add_executable(rps_game
        main.cpp tlist.hpp shape.hpp previous_versions_of_code.hpp)
