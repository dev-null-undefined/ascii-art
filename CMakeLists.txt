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

execute_process(COMMAND nix eval .\#cmake-helper.x86_64-linux.includes --json
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE includes)

execute_process(COMMAND nix eval .\#cmake-helper.x86_64-linux.libs --json
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE libs)

string(JSON libs-length LENGTH ${libs})
string(JSON includes-length LENGTH ${includes})

message(STATUS "includes: ${includes-length} ${includes}")
message(STATUS "libs: ${libs-length} ${libs}")

math(EXPR libs-length "${libs-length} - 1")
math(EXPR includes-length "${includes-length} - 1")

foreach (IDX RANGE ${libs-length})
        string(JSON libs-${IDX} GET ${libs} ${IDX})
        message(STATUS "libs-${IDX}: ${libs-${IDX}}")
        link_directories(${libs-${IDX}}/lib)
        target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC ${libs-${IDX}}/lib)
        set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${libs-${IDX}}/lib)
endforeach ()

foreach (IDX RANGE ${includes-length})
        string(JSON includes-${IDX} GET ${includes} ${IDX})
        message(STATUS "includes-${IDX}: ${includes-${IDX}}")
        include_directories(${includes-${IDX}}/include)
endforeach ()

target_link_libraries(ascii_art jpeg ncurses ncursesw png z form stdc++fs)

