


# -------------
# Target
# -------------
add_executable(bloc)

target_sources(bloc
	PRIVATE
	src/bloc/Bloc.cpp
	src/bloc/BlocForm.cpp
	src/bloc/BlocMap.cpp
	src/bloc/BlocModel.cpp
	src/engine/BlocEngine.cpp
	src/message/BlocMessage.cpp
	src/message/BlocMessageHandler.cpp
	src/utils/DataReader.cpp
	src/demo.cpp
)

target_link_libraries(bloc PUBLIC engine Threads::Threads)

target_include_directories (bloc
PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>
)
