
cmake_minimum_required( VERSION 3.14 )

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
	src/message/entity/AnimateGraphicEntityMessage.cpp
	src/message/entity/DestroyGraphicEntityMessage.cpp
	src/message/entity/NewGraphicEntityMessage.cpp
	src/message/entity/MoveGraphicEntityMessage.cpp
	src/message/entity/RotateGraphicEntityMessage.cpp
	src/message/scene/DestroySceneMessage.cpp
	src/message/scene/MoveSceneMessage.cpp
	src/message/scene/NewSceneMessage.cpp
	src/message/scene/RotateSceneMessage.cpp
	src/message/scene/VisibilitySceneMessage.cpp
	src/resource/ResourceLoader.cpp
)

target_link_libraries(graphic PUBLIC ${OGRE_LIBRARIES} common)

target_include_directories (graphic
PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>
)
