cmake_minimum_required(VERSION 4.1)
project(artificial_will)

set(CMAKE_CXX_STANDARD 23)


find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(abcmake REQUIRED)

add_main_component(${PROJECT_NAME})

target_link_libraries(artificial_will PRIVATE
        SDL2::SDL2main
        SDL2::SDL2
        box2d::box2d
        SDL2_image::SDL2_image
)

target_compile_definitions(microlog PRIVATE
        ULOG_BUILD_COLOR=1
        ULOG_BUILD_TIME=1
        ULOG_BUILD_SOURCE_LOCATION=1
        ULOG_BUILD_TOPICS_MODE=ULOG_BUILD_TOPICS_MODE_DYNAMIC
)

# Copy SDL2.dll to build directory
add_custom_command(TARGET artificial_will POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:artificial_will>
)

add_custom_command(TARGET artificial_will POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:artificial_will>/assets"
        COMMENT "Copying assets to build directory"
)
