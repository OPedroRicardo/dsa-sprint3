cmake_minimum_required(VERSION 3.10)
project(blua_diagnostics C)

set(CMAKE_C_STANDARD 11)

add_executable(blua_diagnostics
    main.c
    structures/stack.c
    structures/queue.c
    structures/linked_list.c
)

target_include_directories(blua_diagnostics PRIVATE ${CMAKE_SOURCE_DIR})
