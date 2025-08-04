set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(${CMAKE_PROJECT_NAME}
    glad.c
    main.cpp
    PerlinNoise.cpp
    PixelBatchRenderer.cpp
    Rectangle.cpp
    SandBox.cpp
    Shader.cpp
)