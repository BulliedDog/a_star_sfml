cmake_minimum_required(VERSION 3.24)
project(a_star_sfml)

set(CMAKE_CXX_STANDARD 17)

add_executable(a_star_sfml main.cpp Griglia.h Personaggio.h Nodo.h test/test.cpp)

set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics network audio)

add_subdirectory(googletest-1.15.2)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

target_link_libraries(a_star_sfml
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network
        gtest
        gtest_main
        )
