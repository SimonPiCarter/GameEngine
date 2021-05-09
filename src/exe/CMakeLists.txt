
cmake_minimum_required( VERSION 3.14 )

# -------------
# Target
# -------------
add_executable(demo)

target_sources(demo
	PRIVATE
	src/demo.cpp
)

target_link_libraries(demo PUBLIC engine)
