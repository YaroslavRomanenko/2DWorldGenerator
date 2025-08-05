cmake_minimum_required(VERSION 3.28.3)
project(OpenGLSandBox LANGUAGES C CXX)

add_subdirectory(vendors)
add_subdirectory(src)

set(CMAKE_CXX_STANDART 14)
set(CMAKE_CXX_STANDART_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PUBLIC
    glad
    imgui
    glfw
    glm
    OpenGL::GL
)