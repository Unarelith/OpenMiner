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
# Packages
#------------------------------------------------------------------------------
# - OpenGL
#------------------------------------------------------------------------------
set(OpenGL_GL_PREFERENCE "LEGACY")

find_package(OpenGL REQUIRED)

if(NOT OPENGL_FOUND)
	message(FATAL_ERROR "OpenGL not found!")
endif(NOT OPENGL_FOUND)

#------------------------------------------------------------------------------
# Submodules
# from https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html
#------------------------------------------------------------------------------
find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if(GIT_SUBMODULE)
		message(STATUS "Submodule update")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule sync --recursive WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/gamekit/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

set(BUILD_SHARED_LIBS OFF)

#------------------------------------------------------------------------------
# - EnTT
#------------------------------------------------------------------------------
add_subdirectory(external/entt)
include_directories(external/entt/src)

#------------------------------------------------------------------------------
# - SFML
#------------------------------------------------------------------------------
set(SFML_STATIC TRUE)

set(SFML_BUILD_AUDIO FALSE CACHE BOOL "")
set(SFML_BUILD_GRAPHICS FALSE CACHE BOOL "")
set(SFML_BUILD_NETWORK TRUE CACHE BOOL "")
set(SFML_BUILD_WINDOW FALSE CACHE BOOL "")

add_subdirectory(external/SFML)

#------------------------------------------------------------------------------
# - gamekit
#------------------------------------------------------------------------------
add_subdirectory(external/gamekit)
include_directories(external/gamekit/include)

#------------------------------------------------------------------------------
# - zlib
#------------------------------------------------------------------------------
set(ZLIB_STATIC ON)

add_subdirectory(external/zlib ${CMAKE_CURRENT_BINARY_DIR}/external/zlib)
include_directories(external/zlib ${CMAKE_CURRENT_BINARY_DIR}/external/zlib)

#------------------------------------------------------------------------------
# - lua
#------------------------------------------------------------------------------
add_subdirectory(external/lua)
include_directories(external/lua/src)

#------------------------------------------------------------------------------
# - sol3 (sol2 v3.x)
#------------------------------------------------------------------------------
add_subdirectory(external/sol2)

#------------------------------------------------------------------------------
# - FastNoiseLite
#------------------------------------------------------------------------------
include_directories(SYSTEM external/FastNoiseLite)

#------------------------------------------------------------------------------
# Subdirectories
#------------------------------------------------------------------------------
add_subdirectory(source/common)
add_subdirectory(source/server)
add_subdirectory(source/client)

#------------------------------------------------------------------------------
# Unit testing
#------------------------------------------------------------------------------
if(OM_BUILD_TESTS)
	find_package(CxxTest QUIET)

	if(CXXTEST_FOUND)
		include_directories(${CXXTEST_INCLUDE_DIRS})
		enable_testing()

		file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp")
		foreach(HEADER_FILE ${HEADER_FILES})
			get_filename_component(HEADER_DIRECTORY ${HEADER_FILE} DIRECTORY)
			include_directories(${HEADER_DIRECTORY})
		endforeach(HEADER_FILE)

		file(GLOB_RECURSE TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.hpp")
		CXXTEST_ADD_TEST(${CMAKE_PROJECT_NAME}_tests unit-test.cpp ${TEST_FILES})
		target_link_libraries(${CMAKE_PROJECT_NAME}_tests ${CMAKE_PROJECT_NAME}_common ${CMAKE_PROJECT_NAME}_server_lib)
	else()
		message(WARNING "CxxTest not found!")
	endif()
endif()

