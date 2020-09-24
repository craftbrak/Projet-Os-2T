cmake_minimum_required(VERSION 3.0.0)

project(FrancCorpsChamps C)


add_subdirectory(Libs)
add_executable(fcc main.c ${SOURCES})