cmake_minimum_required(VERSION 3.19)

project(OpenGL_Labs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CPM)

add_subdirectory("external/glew")
add_subdirectory("external/imgui")
add_subdirectory("external/stb")

CPMAddPackage("gh:glfw/glfw#3.4")
CPMAddPackage("gh:g-truc/glm#1.0.1")

add_executable(glabs)

target_sources(
	glabs
	PRIVATE
	"src/glabs/graphics/glfw_life.cpp"
	"src/glabs/graphics/image2d.cpp"
	"src/glabs/graphics/ogl_buffer.cpp"
	"src/glabs/graphics/ogl_buffers.cpp"
	"src/glabs/graphics/ogl_geometry_input.cpp"
	"src/glabs/graphics/ogl_program_pipeline.cpp"
	"src/glabs/graphics/ogl_shader_program.cpp"
	"src/glabs/graphics/ogl_texture2d.cpp"
	"src/glabs/graphics/window.cpp"

	"src/glabs/rendering/dear_imgui.cpp"
	"src/glabs/rendering/shader_library.cpp"

	"src/glabs/main.cpp"
)

target_include_directories(
	glabs
	PRIVATE
	"src"
)

target_precompile_headers(
	glabs
	PRIVATE
	"src/glabs/pch.hpp"
)

target_link_libraries(
	glabs
	PRIVATE
	glfw
	glew
	imgui
	stb
	glm
)

set_target_properties(
	glabs
	PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED TRUE
)

add_custom_command(
	TARGET glabs
	POST_BUILD COMMAND
	${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/glsl" $<TARGET_FILE_DIR:glabs>/glsl
)

