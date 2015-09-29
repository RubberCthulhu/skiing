cmake_minimum_required(VERSION 2.8)
set(PROJECT skiing)
project(${PROJECT} C)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CONFIGURATION_TYPES Debug Release)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message("PROJECT ${PROJECT}: Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_C_FLAGS_RELEASE "-O2")

add_library(skiinglib skiing.h skiing.c)
add_executable(skiing main.c)
target_link_libraries(skiing skiinglib)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_subdirectory(tests)
    enable_testing()
    add_test(NAME check_skiing COMMAND check_skiing)
endif()
