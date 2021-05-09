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

cmake_minimum_required( VERSION 3.14 )
project(GameEngine)

# ----------------------------
# Internal Cmake helpers
# ----------------------------

include(cmake/Dependencies/OGRE.cmake)

# ----------------------------
# Config
# ----------------------------
set( EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin/")

# ----------------------------
# External dependencies
# ----------------------------

# Ogre
add_definitions( -DOGRE_NO_DEBUG )
setupOgre( OGRE_SOURCE, OGRE_BINARIES, OGRE_LIBRARIES, FALSE )

# SDL2
find_package(SDL2 REQUIRED)

# ----------------------------
# Sub directories
# ----------------------------


add_subdirectory(src)
