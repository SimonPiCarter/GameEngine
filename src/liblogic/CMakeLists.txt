
cmake_minimum_required( VERSION 3.14 )

# -------------
# Target
# -------------
add_library(logic)

target_sources(logic
	PRIVATE
	src/tmp.cpp
)

target_link_libraries(logic PUBLIC common)
