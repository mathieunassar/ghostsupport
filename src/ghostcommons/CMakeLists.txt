file(GLOB header_internal_commons_lib
	${GHOST_SUPPORT_ROOT_DIR}/include/ghost/support/BlockingQueue.hpp
	${GHOST_SUPPORT_ROOT_DIR}/include/ghost/support/BlockingQueue.impl.hpp
	${GHOST_SUPPORT_ROOT_DIR}/include/ghost/support/Split.hpp
)

file(GLOB header_commons_lib
	
)

file(GLOB source_commons_lib
	Split.cpp
)

add_library(ghostcommons
	${header_internal_commons_lib}
	${header_commons_lib}
	${source_commons_lib})
set_target_properties(ghostcommons PROPERTIES FOLDER "ghost_support")
