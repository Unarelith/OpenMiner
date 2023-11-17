#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
set(DEBUG_GCC_FLAGS
	-g -Og
	-Wall -Wextra -Wconversion -Wno-unused-parameter
	-Wfatal-errors
	-DGK_DEBUG
	-DOM_DEBUG
	-DOM_PROFILER_ENABLED
)

set(RELEASE_GCC_FLAGS
	-O3
	-Wall -Wextra -Wconversion -Wno-unused-parameter
	-Wfatal-errors
)

set(RELWITHDEB_GCC_FLAGS
	-g -O3
	-Wall -Wextra -Wconversion -Wno-unused-parameter
	-Wfatal-errors
)

#------------------------------------------------------------------------------
# Platform-specific flags
#------------------------------------------------------------------------------
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CLANG_FLAGS
		-Wno-sign-conversion
		-Wno-implicit-int-float-conversion
		-Wno-nested-anon-types
	)
elseif (MSVC)
	# Disable iterator debug, improves performance of debug builds
	add_compile_definitions($<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=0>)
endif()

#------------------------------------------------------------------------------
# Additional setup
#------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)

include_directories(external)
