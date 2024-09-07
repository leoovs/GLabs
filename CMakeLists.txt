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
	"src/glabs/main.cpp"
)

target_link_libraries(
	glabs
	PRIVATE
	glfw
	glew
)

