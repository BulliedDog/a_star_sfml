cmake_minimum_required(VERSION 3.24)
project(a_star_sfml)

set(CMAKE_CXX_STANDARD 17)

add_executable(a_star_sfml main.cpp)

add_executable(run_tests test/test.cpp)

set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics network audio)

add_subdirectory(googletest-1.15.2)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_library(tests STATIC test/test.cpp)

target_link_libraries(a_star_sfml
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network
        gtest
        )
target_link_libraries(tests
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network
        gtest
        gtest_main
        )
target_link_libraries(run_tests tests)
add_test(NAME Test COMMAND run_tests)
