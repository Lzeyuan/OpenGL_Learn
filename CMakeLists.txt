cmake_minimum_required(VERSION 3.5)
project(ZBufferTest)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

FILE(GLOB SOURCES "src/*.cpp" "main.cpp" "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

set(FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Shaders)
file(COPY ${FILE_PATH} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(${PROJECT_NAME}
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)