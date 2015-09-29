include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})

add_executable(check_skiing check_skiing.c)
target_link_libraries(check_skiing skiinglib ${CHECK_LIBRARIES} m rt pthread)
