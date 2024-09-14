add_library(stb STATIC)

target_sources(
	stb
	PRIVATE
	"src/stb/stb_image.c"
)

target_include_directories(
	stb
	PUBLIC
	"include"
)
