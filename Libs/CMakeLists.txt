cmake_minimum_required(VERSION 3.0.0)

file(GLOB SOURCES "*.h" "*.c")
set(SOURCES ${SOURCES} PARENT_SCOPE) # On ouvre les yeux au dossier parent