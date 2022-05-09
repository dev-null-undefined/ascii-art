cmake_minimum_required(VERSION 3.0)
project(ascii_art)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wno-long-long -D DEBUG")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

set(CMAKE_CXX_STANDARD 17)

add_executable(ascii_art
        src/main.cpp
        src/ascii-art/sources/DataSourceFactory.cpp
        src/ascii-art/sources/Image.cpp
        src/ascii-art/sources/Video.cpp
        src/ascii-art/sources/DataSource.cpp
        src/ascii-art/sources/Filter.cpp
        src/ascii-art/sources/image-formats/PNGImage.cpp
        src/ascii-art/sources/image-formats/BMPImage.cpp
        src/ascii-art/sources/image-formats/JPEGImage.cpp
        src/ascii-art/sources/ImageFrame.cpp
        src/ascii-art/sources/Frame.cpp
        src/ascii-art/sources/Color.cpp
        src/ascii-art/Settings.cpp
        src/ascii-art/Menu.cpp)


include_directories(/nix/store/0qj6w5zxqac428ipmc7yfjx9y50pv1b5-libjpeg-9e-dev/include /nix/store/01qrnjnd4iiagrjl308305g9ghghbswr-ncurses-6.3-dev/include /nix/store/2wxhxw9l5z4d1wvbnbmlwlnbgrf6p3sg-libpng-1.2.59-dev/include /nix/store/1my9xr1s1nfjmqwyi46pzdrvny7hm66x-zlib-1.2.11-dev/include)
link_directories(/nix/store/43iz38yxmjn8am7ji6mlidkl599kxc39-libjpeg-9e/lib /nix/store/6zk45l0g8fri5q3hdvw0vwrw0cs34snc-ncurses-6.3/lib /nix/store/bl0dac72m1ibaicsdm8y3jshkq2vjmya-libpng-1.2.59/lib /nix/store/bqjh8dc6m5plswxrmmn9x4cpxrrw94lv-zlib-1.2.11/lib)

target_link_libraries(ascii_art jpeg ncurses png z)
target_link_directories(ascii_art PUBLIC /nix/store/43iz38yxmjn8am7ji6mlidkl599kxc39-libjpeg-9e/lib /nix/store/6zk45l0g8fri5q3hdvw0vwrw0cs34snc-ncurses-6.3/lib /nix/store/bl0dac72m1ibaicsdm8y3jshkq2vjmya-libpng-1.2.59/lib /nix/store/bqjh8dc6m5plswxrmmn9x4cpxrrw94lv-zlib-1.2.11/lib)

set(CMAKE_LIBRARY_PATH /nix/store/43iz38yxmjn8am7ji6mlidkl599kxc39-libjpeg-9e/lib /nix/store/6zk45l0g8fri5q3hdvw0vwrw0cs34snc-ncurses-6.3/lib /nix/store/bl0dac72m1ibaicsdm8y3jshkq2vjmya-libpng-1.2.59/lib /nix/store/bqjh8dc6m5plswxrmmn9x4cpxrrw94lv-zlib-1.2.11/lib)


