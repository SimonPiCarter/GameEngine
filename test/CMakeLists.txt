
cmake_minimum_required( VERSION 3.14 )

# ----------------------------
# External dependencies
# ----------------------------

find_package(GTest REQUIRED CONFIG)

add_executable(unit_tests)

target_link_libraries(unit_tests
	PRIVATE
	GTest::gtest_main
)
