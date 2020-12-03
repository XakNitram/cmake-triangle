add_executable(
    hello-triangle Application.cpp
    Triangle.h
    Triangle.cpp
    
    # Core
    Core/Utils.h
    Core/Utils.cpp
    Core/Simulation.h
    Core/Simulation.cpp
    
    # Renderer
    Renderer/GLDebug.h
    Renderer/GLDebug.cpp
    Renderer/Buffer.h
    Renderer/VertexArray.h
    Renderer/VertexArray.cpp
    Renderer/Shader.h
    Renderer/Shader.cpp
)

target_link_libraries(hello-triangle PRIVATE glad)
target_link_libraries(hello-triangle PRIVATE glfw)
target_precompile_headers(hello-triangle PRIVATE pch.h pch.cpp)

add_custom_command(
    TARGET hello-triangle POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    ${CMAKE_SOURCE_DIR}/res/Shaders/
    $<TARGET_FILE_DIR:hello-triangle>/Shaders/
)
message(STATUS "Copied shaders to output")
