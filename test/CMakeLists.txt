


# ----------------------------
# External dependencies
# ----------------------------

find_package(GTest REQUIRED)

add_executable(unit_tests)

target_link_libraries(unit_tests
	PRIVATE
	GTest::gtest_main
)
