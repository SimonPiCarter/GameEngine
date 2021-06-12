


##########################
#		LIBRARY			 #
##########################

# -------------
# Target
# -------------
add_library(libcells)

target_sources(libcells
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/game/CellsEngine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/display/MapDisplay.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/effect/Effect.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/effect/Buff.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/effect/buff/Slow.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/entity/MobEntity.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/entity/Tower.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/layout/MapLayout.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/ui/HeaderUI.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/ui/MobSelectionUI.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/ui/TowerSelectionUI.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/utils/ArrayUtils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/utils/Hitbox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/slot/AttackModifier.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/slot/Slot.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/slot/effect_maker/SlowSlot.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/wave/AttackBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/wave/BoundingBox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/wave/MobMover.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/wave/MobSpawner.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/LogicEngine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/logic/WaveEngine.cpp
)

target_link_libraries(libcells PUBLIC engine Threads::Threads)

target_include_directories (libcells
PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/lib>
	$<INSTALL_INTERFACE:include>
)

##########################
#		EXECUTABLE		 #
##########################

# -------------
# Target
# -------------
add_executable(cells)

target_sources(cells
	PRIVATE
	src/exe/main.cpp
)

target_link_libraries(cells PUBLIC libcells)

target_include_directories (cells
PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/exe>
	$<INSTALL_INTERFACE:include>
)

##########################
#		TESTS			 #
##########################

add_subdirectory(test)
