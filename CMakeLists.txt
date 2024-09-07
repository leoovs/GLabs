cmake_minimum_required(VERSION 3.19)

project(OpenGL_Labs)

add_executable(glabs)

target_sources(
	glabs
	PRIVATE
	"src/glabs/main.cpp"
)

