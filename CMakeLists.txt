cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(CMAKE_BUILD_TYPE Debug)

project("bataille_navale")

set(TGT "bataille_navale")
set(${TGT}_VERSION_MAJOR 0)
set(${TGT}_VERSION_MINOR 1)

set(CC_COVERAGE_COMPILE_FLAGS "-pedantic -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS_DEBUG "${CC_COVERAGE_COMPILE_FLAGS} -DDebug -g -pg")
set(CMAKE_C_FLAGS_RELEASE "${CC_COVERAGE_COMPILE_FLAGS} -O3")

set(CMAKE_CC_STANDARD 11)
set(CMAKE_CC_STANDARD_REQUIRED YES)
set(CMAKE_CC_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "../debug/")

set(CMAKE_CC_FLAGS "${CMAKE_CC_FLAGS} ${CC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CC_COVERAGE_COMPILE_FLAGS}")

include_directories(include)

add_executable(${TGT} src/dumb_player.c src/camp_allocator.c src/camp.c src/darray.c src/game_logic.c src/main.c src/player.c src/utils.c)
add_executable(test_${TGT} test/test.c test/bataille.c src/dumb_player.c  src/camp_allocator.c src/camp.c src/darray.c src/game_logic.c src/utils.c src/player.c)
target_link_libraries(${TGT} ncurses)
target_link_libraries(test_${TGT} ncurses)