cmake_minimum_required(VERSION 3.19)

project(OpenGL_Labs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CPM)

add_subdirectory("external/glew")

CPMAddPackage("gh:glfw/glfw#3.4")

add_executable(glabs)

target_sources(
	glabs
	PRIVATE
	"src/glabs/graphics/ogl_buffer.cpp"
	"src/glabs/graphics/ogl_buffers.cpp"
	"src/glabs/graphics/ogl_geometry_input.cpp"
	"src/glabs/graphics/ogl_program_pipeline.cpp"
	"src/glabs/graphics/window.cpp"

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
)

set_target_properties(
	glabs
	PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED TRUE
)

