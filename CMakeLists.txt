cmake_minimum_required(VERSION 3.23)
project(tetris C)

set(CMAKE_C_STANDARD 17)

add_executable(tetris main.c points2D.c points2D.h)
