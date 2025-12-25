cmake_minimum_required(VERSION 3.10)
project(file_splitter C)

set(CMAKE_C_STANDARD 11)

# Automatically include all .h files from include/
include_directories(include)

# Automatically find all .c files in src/
file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})