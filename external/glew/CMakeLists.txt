find_package(OpenGL REQUIRED)

add_library(glew STATIC)

target_sources(
	glew
	PRIVATE
	"src/glew.c"
)

target_include_directories(
	glew
	PUBLIC
	"include"
)

target_compile_definitions(
	glew
	PUBLIC
	"GLEW_STATIC"
)

target_link_libraries(
	glew
	PUBLIC
	OpenGL::GL
)

