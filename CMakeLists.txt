cmake_minimum_required(VERSION 3.16)

project(gl_project CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)
find_package(OpenGL REQUIRED)

qt_add_executable(gl_project main.cpp geometry.cpp glslprogram.cpp glwidget.cpp primitives.cpp
    geometry.h glslprogram.h glwidget.h primitives.h shaders/vs_skybox.glsl shaders/fs_skybox.glsl
                    shaders/fs_ADS_phong.glsl shaders/vs_ADS_phong.glsl

                )

target_link_libraries(gl_project PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    OpenGL::GL
)

target_include_directories(gl_project PRIVATE .)

add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders
            ${CMAKE_BINARY_DIR}/shaders
)

add_dependencies(gl_project copy_shaders)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/res"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
)
