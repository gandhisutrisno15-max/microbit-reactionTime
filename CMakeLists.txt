cmake_minimum_required(VERSION 3.15)

project(reaction_time_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

enable_language(ASM)

set(SOURCES
    src/main.cpp
    src/timer.s
)

add_executable(reaction_time_game ${SOURCES})

target_include_directories(reaction_time_game PRIVATE src)
