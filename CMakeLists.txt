cmake_minimum_required(VERSION 3.23)
include_directories(include)
project(AsciiClock C)

set(CMAKE_C_STANDARD 17)

add_executable(AsciiClock main.c)
find_package( Curses REQUIRED )
include_directories( ${CURSES_INCLUDE_DIRS} )
target_link_libraries( AsciiClock ${CURSES_LIBRARIES} )
