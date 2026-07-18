cmake_minimum_required(VERSION 3.16)

project(statdump C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_FLAGS_RELEASE "-O3")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

set(STATDUMP_SOURCES

    src/dump.c
    src/error.c
    src/join.c
    src/sort.c
    src/util.c
)

add_executable(statdump

    apps/statdump.c

    ${STATDUMP_SOURCES}
)

add_executable(test_statdump

    apps/test_statdump.c

    tests/test_cases.c
    tests/test_utils.c
    ${STATDUMP_SOURCES}
)

target_include_directories(statdump PRIVATE include)
target_include_directories(test_statdump PRIVATE include tests)

target_link_libraries(statdump m)
target_link_libraries(test_statdump m)