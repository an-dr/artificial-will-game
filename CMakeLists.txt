cmake_minimum_required(VERSION 4.1)
project(artificial_will)

set(CMAKE_CXX_STANDARD 23)

add_executable(artificial_will src/main.cpp)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

target_link_libraries(artificial_will PRIVATE
        SDL2::SDL2main
        SDL2::SDL2
        SDL2_image::SDL2_image
)

# Copy SDL2.dll to build directory
add_custom_command(TARGET artificial_will POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:artificial_will>
)