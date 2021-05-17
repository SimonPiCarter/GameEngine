
cmake_minimum_required( VERSION 3.14 )

# -------------
# Target
# -------------
add_library(common)

target_sources(common
	PRIVATE
	src/message/game/GameMessage.cpp
	src/message/game/GameMessageHandler.cpp
	src/resource/Resource.cpp
)

target_link_libraries(common PUBLIC ${SDL2_LIBRARIES})

target_include_directories (common
PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>
	${SDL2_INCLUDE_DIRS}
)
