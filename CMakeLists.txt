cmake_minimum_required(VERSION 3.13)
project(klotski C)

set(CMAKE_C_STANDARD 11)

set(SRC_DIR ./src)
include_directories(${SRC_DIR})

add_executable(klotski ${SRC_DIR}/main.c ${SRC_DIR}/config.c ${SRC_DIR}/board.c ${SRC_DIR}/utils.c ${SRC_DIR}/tests.c ${SRC_DIR}/puzzle.c ${SRC_DIR}/queue.c ${SRC_DIR}/bst.c)
