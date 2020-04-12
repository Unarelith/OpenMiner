#------------------------------------------------------------------------------
# Set project name
#------------------------------------------------------------------------------
set(PROJECT_NAME ${CMAKE_PROJECT_NAME}_common)

#------------------------------------------------------------------------------
# Get source files
#------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.hpp)

include_directories(../../external ../../external/entt/single_include)

foreach(HEADER_FILE ${HEADER_FILES})
	get_filename_component(HEADER_DIRECTORY ${HEADER_FILE} DIRECTORY)
	include_directories(${HEADER_DIRECTORY})
endforeach(HEADER_FILE)

#------------------------------------------------------------------------------
# Add library
#------------------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} EnTT)

#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
if (NOT MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_GCC_FLAGS}>")
	target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_GCC_FLAGS}>")
	target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:${RELWITHDEB_GCC_FLAGS}>")
endif ()

target_compile_options(${PROJECT_NAME} PRIVATE -DDEBUG_ENABLED)
target_compile_options(${PROJECT_NAME} PRIVATE -DGLM_FORCE_RADIANS)
target_compile_options(${PROJECT_NAME} PRIVATE -DSOL_CHECK_ARGUMENTS -DSOL_PRINT_ERRORS=0)

# target_compile_options(${PROJECT_NAME} PRIVATE -pg)

target_link_libraries(${PROJECT_NAME} sfml-system sfml-network)

