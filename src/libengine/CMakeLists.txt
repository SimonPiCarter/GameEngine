


# -------------
# Target
# -------------
add_library(engine)

target_sources(engine
	PRIVATE
	src/engine/GameEngine.cpp
)

target_link_libraries(engine PUBLIC graphic logic)

target_include_directories (engine
PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>
)
