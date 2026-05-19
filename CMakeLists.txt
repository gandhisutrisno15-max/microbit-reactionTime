cmake_minimum_required(VERSION 3.15)

project(reaction_time_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

enable_language(ASM)

# Source files
set(SOURCES
    source/main.cpp
    source/timer.s
)

# Create executable
add_executable(MICROBIT ${SOURCES})

# Include source directory
target_include_directories(MICROBIT PRIVATE
    source
)

# Link CODAL micro:bit library
target_link_libraries(MICROBIT codal-microbit-v2)
