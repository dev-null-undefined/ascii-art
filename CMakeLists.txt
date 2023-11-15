cmake_minimum_required(VERSION 3.12)
project(ascii_art)

# Tests
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wno-long-long -D DEBUG -D LOG_LEVEL=10 -rdynamic -D __TESTING__")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE ascii_art_SRC CONFIGURE_DEPENDS
        "src/*.h"
        "src/*.cpp"
        )

add_executable(ascii_art ${ascii_art_SRC})

execute_process(COMMAND nix build .\#cmake-helper.x86_64-linux.cmake-file --no-link --print-out-paths
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE cmake-flake-include)

include(${cmake-flake-include})

target_link_libraries(ascii_art jpeg ncurses ncursesw png z form stdc++fs)

