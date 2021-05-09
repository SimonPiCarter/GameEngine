
cmake_minimum_required( VERSION 3.14 )

# -------------
# Target
# -------------
add_library(engine)

target_sources(engine
	PRIVATE
	src/tmp.cpp
)

target_link_libraries(engine PUBLIC graphic logic)
