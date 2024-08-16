cmake_minimum_required(VERSION 3.24)
project(a_star_sfml)

set(CMAKE_CXX_STANDARD 17)

add_executable(a_star_sfml main.cpp)

# Set SFML directory path
set(SFML_DIR "C:/SFML/lib/cmake/SFML")

# Find SFML package
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics network audio)

# Link SFML libraries to the target
target_link_libraries(a_star_sfml
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network
        )
