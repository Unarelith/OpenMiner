#------------------------------------------------------------------------------
# Get source files
#------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES source/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.hpp ../external/*.hpp)

foreach(HEADER_FILE ${HEADER_FILES})
	get_filename_component(HEADER_DIRECTORY ${HEADER_FILE} DIRECTORY)
	include_directories(${HEADER_DIRECTORY})
endforeach(HEADER_FILE)

#------------------------------------------------------------------------------
# Add library
#------------------------------------------------------------------------------
add_library(${CMAKE_PROJECT_NAME}_common STATIC ${SOURCE_FILES})

#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
if (NOT MSVC)
	target_compile_options(${CMAKE_PROJECT_NAME}_common PRIVATE -O3 -ffast-math)
	target_compile_options(${CMAKE_PROJECT_NAME}_common PRIVATE -g -Wall -Wextra -Wfatal-errors -Wno-variadic-macros)
else ()
	target_compile_options(${CMAKE_PROJECT_NAME}_common PRIVATE /O2)
endif ()

target_compile_options(${CMAKE_PROJECT_NAME}_common PRIVATE -DDEBUG_ENABLED)
target_compile_options(${CMAKE_PROJECT_NAME}_common PRIVATE -DGLM_FORCE_RADIANS)
target_compile_options(${CMAKE_PROJECT_NAME}_common PRIVATE -DSOL_CHECK_ARGUMENTS
	# -DSOL_SAFE_USERTYPE=1
	# -DSOL_SAFE_REFERENCES=1
	# -DSOL_SAFE_FUNCTION_CALLS=1
	# -DSOL_SAFE_FUNCTION=1
	# -DSOL_SAFE_NUMERICS=1
	# -DSOL_SAFE_GETTER=1
	-DSOL_PRINT_ERRORS=1
)

set(CMAKE_CXX_STANDARD 14)

# target_compile_options(${CMAKE_PROJECT_NAME}_common PRIVATE -std=c++14)
# target_compile_features(${CMAKE_PROJECT_NAME}_common PRIVATE cxx_std_17)

# target_compile_options(${CMAKE_PROJECT_NAME}_common PRIVATE -pg)

target_link_libraries(${CMAKE_PROJECT_NAME}_common sfml-system sfml-network)

