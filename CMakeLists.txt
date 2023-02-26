#------------------------------------------------------------------------------
# CMakeLists.txt
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1)

project(openminer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (APPLE)
	add_definitions(-DGL_SILENCE_DEPRECATION=1)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(OM_BUILD_TESTS ON CACHE BOOL "Enable building tests if CxxTest is available")

include_directories(external)

#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
set(DEBUG_GCC_FLAGS
	-g -Og
	-Wall -Wextra -Wconversion -Wno-unused-parameter
	-Wfatal-errors
	-DGK_DEBUG
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

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CLANG_FLAGS
		-Wno-sign-conversion
		-Wno-implicit-int-float-conversion
		-Wno-nested-anon-types
	)
endif()

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
	# Disable iterator debug, improves performance of debug builds
	add_compile_definitions($<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=0>)
endif ()

#------------------------------------------------------------------------------
# Setting default build type
#------------------------------------------------------------------------------
set(DEFAULT_BUILD_TYPE RelWithDebInfo)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
	set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)

	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

#------------------------------------------------------------------------------
# Subdirectories
#------------------------------------------------------------------------------
add_subdirectory(external)

add_subdirectory(source/common)
add_subdirectory(source/server)
add_subdirectory(source/client)

add_subdirectory(tests)
