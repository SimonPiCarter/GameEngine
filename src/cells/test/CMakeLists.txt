


find_package(GTest REQUIRED)

# -------------
# Target
# -------------
add_executable(unit_tests_cells)

target_sources(unit_tests_cells
	PRIVATE
	src/wave/attack.test.cpp
	src/wave/mobDespawn.test.cpp
	src/wave/mobMover.test.cpp
	src/wave/mobSpawner.test.cpp
	src/wave/positionalTree.test.cpp
)

target_link_libraries(unit_tests_cells PUBLIC libcells GTest::gtest_main)

target_include_directories (unit_tests_cells
PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>
)
