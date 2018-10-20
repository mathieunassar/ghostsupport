set(GHOST_SUPPORT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE FILEPATH "Root directory of ghost_support")

# MODULE LIBRARY

#####################################################################

file(GLOB header_internal_commons_lib
./include/BlockingQueue.hpp
./include/BlockingQueue.impl.hpp
)

file(GLOB sources_internal_commons_lib
./src/main.cpp
)

#####################################################################

add_library(Commons ${header_internal_commons_lib}
	${sources_internal_commons_lib})

set_property(TARGET Commons PROPERTY FOLDER "Internal")
