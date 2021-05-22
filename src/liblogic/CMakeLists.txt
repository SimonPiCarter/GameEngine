
cmake_minimum_required( VERSION 3.14 )

# -------------
# Target
# -------------
add_library(logic)

target_sources(logic
	PRIVATE
	src/message/LogicMessage.cpp
	src/message/LogicMessageHandler.cpp
)

target_link_libraries(logic PUBLIC common)

target_include_directories (logic
PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>
)

# -------------
# Test
# -------------

#add_subdirectory(test)
