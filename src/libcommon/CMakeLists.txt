
cmake_minimum_required( VERSION 3.14 )

# -------------
# Target
# -------------
add_library(common)

target_sources(common
	PRIVATE
	src/tmp.cpp
)

target_link_libraries(common PUBLIC ${OGRE_LIBRARIES} ${SDL2_LIBRARIES})
target_include_directories(common PUBLIC ${SDL2_INCLUDE_DIRS})
