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

include_directories(external)

#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
set(DEBUG_GCC_FLAGS -g -Og -Wall -Wextra -Wfatal-errors -Wno-variadic-macros)
set(RELEASE_GCC_FLAGS -O3)
set(RELWITHDEB_GCC_FLAGS -g -O3 -Wall -Wextra -Wfatal-errors -Wno-variadic-macros)

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
# - gamekit
#------------------------------------------------------------------------------
add_subdirectory(external/gamekit)
include_directories(external/gamekit/include)

#------------------------------------------------------------------------------
# - zlib
#------------------------------------------------------------------------------
add_subdirectory(external/zlib ${CMAKE_CURRENT_BINARY_DIR}/external/zlib)
include_directories(external/zlib ${CMAKE_CURRENT_BINARY_DIR}/external/zlib)

#------------------------------------------------------------------------------
# - lua
#------------------------------------------------------------------------------
set(LUA_BUILD_WLUA OFF CACHE BOOL "")

add_subdirectory(external/lua)
include_directories(external/lua/src ${CMAKE_CURRENT_BINARY_DIR}/external/lua)

#------------------------------------------------------------------------------
# - sol3 (sol2 v3.x)
#------------------------------------------------------------------------------
set(LUA_LOCAL_DIR external/lua)

add_subdirectory(external/sol2)

#------------------------------------------------------------------------------
# Subdirectories
#------------------------------------------------------------------------------
add_subdirectory(source/common)
add_subdirectory(source/server)
add_subdirectory(source/client)
