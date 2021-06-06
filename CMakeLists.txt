#This scripts will add all the cpp and h files under src and include folders, and
#assumes that your Ogre source code is in Dependencies/Ogre and that:
# In Windows you built Ogre into Dependencies/Ogre/build
# In Linux you built Release into Dependencies/Ogre/build/Release
# In Linux you built Debug into Dependencies/Ogre/build/Debug
#
# If your source code is not at "Dependencies/Ogre"; you can use "mklink /D" to create
# a symbolic link to where the source code is located on Windows.
# On Linux, you can use "ln -s"

#set( CMAKE_TOOLCHAIN_FILE CMake/iOS.cmake )

cmake_minimum_required( VERSION 3.19 )
project(GameEngine)

# ----------------------------
# Internal Cmake helpers
# ----------------------------

include(cmake/Dependencies/OGRE.cmake)

# ----------------------------
# Config
# ----------------------------

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin/")

# thanks Ogre 3d
if( WIN32 )
add_compile_options("/IGNORE:no-ignored-attributes")
else()
add_compile_options("-Wno-ignored-attributes")
endif()

# ============================================================================
# External requirements
# ============================================================================

# ----------------------------------------------------------------------------
# Optional conan setup
# ----------------------------------------------------------------------------
if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	include (${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup (TARGETS)
elseif (EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake)
	include (${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif ()

# additional cmake modules
list (INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Ogre
add_definitions( -DOGRE_NO_DEBUG )
setupOgre( OGRE_SOURCE, OGRE_BINARIES, OGRE_LIBRARIES, FALSE )
set(OGRE_IGNORE_UNKNOWN_DEBUG ON)

# SDL2
find_package(SDL2 REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# ----------------------------
# Sub directories
# ----------------------------

#add_subdirectory(test)
add_subdirectory(src)
