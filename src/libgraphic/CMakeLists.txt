
cmake_minimum_required( VERSION 3.14 )

add_compile_options("-Wno-ignored-attributes")

# -------------
# Target
# -------------
add_library(graphic)

target_sources(graphic
	PRIVATE
	src/engine/GraphicEngine.cpp
	src/engine/utils/StaticPluginLoader.cpp
	src/entity/GraphicEntity.cpp
	src/message/GraphicMessage.cpp
	src/message/GraphicMessageHandler.cpp
	src/message/implem/AnimateGraphicEntityMessage.cpp
	src/message/implem/NewGraphicEntityMessage.cpp
	src/message/implem/MoveGraphicEntityMessage.cpp
	src/resource/ResourceLoader.cpp
)

target_link_libraries(graphic PUBLIC ${OGRE_LIBRARIES} common)

target_include_directories (graphic
PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>
)
