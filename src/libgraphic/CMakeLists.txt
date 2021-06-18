


# -------------
# Target
# -------------
add_library(graphic)

target_sources(graphic
	PRIVATE
	src/engine/GraphicEngine.cpp
	src/engine/utils/StaticPluginLoader.cpp
	src/engine/utils/SdlInputHandler.cpp
	src/entity/GraphicEntity.cpp
	src/gui/ButtonData.cpp
	src/gui/CentralMenu.cpp
	src/gui/PopUpScreen.cpp
	src/gui/RichLabel.cpp
	src/message/GraphicMessage.cpp
	src/message/GraphicMessageHandler.cpp
	src/message/camera/LookAtCameraMessage.cpp
	src/message/camera/MoveCameraMessage.cpp
	src/message/camera/RotateCameraMessage.cpp
	src/message/entity/AnimateGraphicEntityMessage.cpp
	src/message/entity/DestroyGraphicEntityMessage.cpp
	src/message/entity/LinkGraphicEntityMessage.cpp
	src/message/entity/NewGraphicEntityMessage.cpp
	src/message/entity/MoveGraphicEntityMessage.cpp
	src/message/entity/RotateGraphicEntityMessage.cpp
	src/message/gui/CentralMenuMessage.cpp
	src/message/gui/CustomGuiMessage.cpp
	src/message/gui/RichLabelMessage.cpp
	src/message/light/DestroyLightMessage.cpp
	src/message/light/MoveLightMessage.cpp
	src/message/light/NewLightMessage.cpp
	src/message/light/RotateLightMessage.cpp
	src/message/particle/DestroyParticleMessage.cpp
	src/message/particle/NewParticleMessage.cpp
	src/message/scene/DestroySceneMessage.cpp
	src/message/scene/MoveSceneMessage.cpp
	src/message/scene/NewSceneMessage.cpp
	src/message/scene/RotateSceneMessage.cpp
	src/message/scene/VisibilitySceneMessage.cpp
	src/resource/ResourceLoader.cpp
)

set( COLIBRIGUI_LIB_ONLY "ON" )
add_subdirectory(${COLIBRI_DEPS} ${CMAKE_BINARY_DIR}/dependencies/)

target_link_libraries(graphic PUBLIC ${OGRE_LIBRARIES} common ${COLIBRI_LIB} icucommon ${HARFBUZZ_LIBRARIES} ${FREETYPE_LIBRARIES} ${ZLIB_LIBRARIES})

if( UNIX )
	target_link_libraries(graphic PUBLIC dl )
endif()

target_include_directories (graphic
PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>
	${COLIBRI_INCLUDE}
	${ZLIB_INCLUDE_DIRS}
	${HARFBUZZ_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIRS}
	${COLIBRI_DEPS}/MSVC_Fix/2012/
)
