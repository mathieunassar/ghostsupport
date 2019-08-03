cmake_minimum_required(VERSION 3.8)

set(projectName ghost_support)
set(GHOST_SUPPORT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE FILEPATH "Root directory of ghost_support")

# define the project
project(${projectName} CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#####################################################################

set(GHOST_SUPPORT_INCLUDE_DIRECTORIES
	${GHOST_SUPPORT_ROOT_DIR}/include
	CACHE FILEPATH "Include directories of ghost_support"
)

include_directories(
	${GHOST_SUPPORT_INCLUDE_DIRECTORIES}
)

file(GLOB header_internal_commons_lib
./include/BlockingQueue.hpp
./include/BlockingQueue.impl.hpp
)

#####################################################################

if (${APPLE})
	add_library(ghostcommons INTERFACE)
	target_sources(ghostcommons INTERFACE ${header_internal_commons_lib})
	target_include_directories(ghostcommons INTERFACE include/)
else()
	add_library(ghostcommons ${header_internal_commons_lib})

	set_target_properties(ghostcommons PROPERTIES LINKER_LANGUAGE CXX)
	set_target_properties(ghostcommons PROPERTIES FOLDER "ghost_support")
endif()
