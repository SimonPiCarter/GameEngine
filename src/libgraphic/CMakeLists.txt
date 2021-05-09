
cmake_minimum_required( VERSION 3.14 )

# -------------
# Target
# -------------
add_library(graphic)

target_sources(graphic
	PRIVATE
	src/tmp.cpp
)

target_link_libraries(graphic PUBLIC ${OGRE_LIBRARIES} ${SDL2_LIBRARIES} common)
target_include_directories(graphic PUBLIC ${SDL2_INCLUDE_DIRS})
