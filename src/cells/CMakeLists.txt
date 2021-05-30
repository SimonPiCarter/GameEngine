


##########################
#		LIBRARY			 #
##########################

# -------------
# Target
# -------------
add_library(libcells)

target_sources(libcells
	PRIVATE
	src/lib/tmp.cpp
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
