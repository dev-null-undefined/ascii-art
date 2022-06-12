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

execute_process(COMMAND nix build --no-link nixpkgs\#libjpeg_original.dev
        OUTPUT_VARIABLE tmp
        COMMAND nix path-info nixpkgs\#libjpeg_original.dev
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE libjpeg-dev)
execute_process(COMMAND nix build --no-link nixpkgs\#libjpeg_original.out
        OUTPUT_VARIABLE tmp
        COMMAND nix path-info nixpkgs\#libjpeg_original.out
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE libjpeg)
execute_process(COMMAND nix build --no-link nixpkgs\#libpng.dev
        OUTPUT_VARIABLE tmp
        COMMAND nix path-info nixpkgs\#libpng.dev
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE libpng-dev)
execute_process(COMMAND nix build --no-link nixpkgs\#libpng.out
        OUTPUT_VARIABLE tmp
        COMMAND nix path-info nixpkgs\#libpng.out
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE libpng)
execute_process(COMMAND nix build --no-link nixpkgs\#zlib.dev
        OUTPUT_VARIABLE tmp
        COMMAND nix path-info nixpkgs\#zlib.dev
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE zlib-dev)
execute_process(COMMAND nix build --no-link nixpkgs\#zlib.out
        OUTPUT_VARIABLE tmp
        COMMAND nix path-info nixpkgs\#zlib.out
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE zlib)
execute_process(COMMAND nix build --no-link nixpkgs\#ncurses.dev
        OUTPUT_VARIABLE tmp
        COMMAND nix path-info nixpkgs\#ncurses.dev
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE ncurses-dev)
execute_process(COMMAND nix build --no-link nixpkgs\#ncurses.out
        OUTPUT_VARIABLE tmp
        COMMAND nix path-info nixpkgs\#ncurses.out
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE ncurses)

message(STATUS "ncures: ${ncurses}")
message(STATUS "ncurses-dev: ${ncurses-dev}")
message(STATUS "libjpeg: ${libjpeg}")
message(STATUS "libjpeg-dev: ${libjpeg-dev}")
message(STATUS "libpng: ${libpng}")
message(STATUS "libpng-dev: ${libpng-dev}")
message(STATUS "zlib: ${zlib}")
message(STATUS "zlib-dev: ${zlib-dev}")

include_directories(${libjpeg-dev}/include ${ncurses-dev}/include ${libpng-dev}/include ${zlib-dev}/include)
link_directories(${libjpeg}/lib ${ncurses}/lib ${libpng}/lib ${zlib}/lib)

target_link_libraries(ascii_art jpeg ncurses ncursesw png z form stdc++fs)
target_link_directories(ascii_art PUBLIC ${libjpeg}/lib ${ncurses}/lib ${libpng}/lib ${zlib}/lib)

set(CMAKE_LIBRARY_PATH ${libjpeg}/lib ${ncurses}/lib ${libpng}/lib ${zlib}/lib)


