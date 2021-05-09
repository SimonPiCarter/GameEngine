
target_sources(unit_tests
	PRIVATE
	src/message.test.cpp)

target_link_libraries(unit_tests PRIVATE
	engine
	Threads::Threads)
