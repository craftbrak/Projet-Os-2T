cmake_minimum_required(VERSION 3.0.0)

project(FrancCorpsChamps C)


add_subdirectory(Libs)
add_executable(fcc main.c ${SOURCES})
add_custom_command(TARGET fcc
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/config.txt ${CMAKE_BINARY_DIR}/config.txt
)