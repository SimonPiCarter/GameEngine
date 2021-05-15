
cmake_minimum_required( VERSION 3.14 )

# -------------
# Target
# -------------
add_library(common)

target_sources(common
	PRIVATE
	src/resource/Resource.cpp
)

target_include_directories (common
PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>
)
