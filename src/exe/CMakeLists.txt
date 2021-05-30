


# -------------
# Target
# -------------
add_executable(demo)

target_sources(demo
	PRIVATE
	src/demo.cpp
)

target_link_libraries(demo PUBLIC engine)
