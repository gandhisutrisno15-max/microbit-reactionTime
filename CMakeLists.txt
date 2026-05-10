cmake_minimum_required(VERSION 3.10)
project(reaction_time_game)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

# Enable ASM language
enable_language(ASM)

# Add source files
set(SOURCES
    src/main.cpp
    src/timer.s
)

# Create executable
add_executable(reaction_time_game ${SOURCES})

# Link with micro:bit libraries (adjust path as needed)
target_link_libraries(reaction_time_game microbit)

# Include directories
target_include_directories(reaction_time_game PRIVATE src/)