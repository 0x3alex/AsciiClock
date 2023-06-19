cmake_minimum_required(VERSION 3.23)
include_directories(include)
project(AsciiClock C)

set(CMAKE_C_STANDARD 17)

add_executable(AsciiClock main.c)
