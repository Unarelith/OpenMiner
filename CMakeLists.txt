#------------------------------------------------------------------------------
# CMakeLists.txt
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1)

project(openminer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (WIN32)
	set(WIN_LIBRARIES_PATH "C:/Libraries")
endif ()

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
# - Lua
#------------------------------------------------------------------------------
if (WIN32)
	set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${WIN_LIBRARIES_PATH}/lua)
endif ()

find_package(Lua REQUIRED)

if(NOT LUA_FOUND)
	message(FATAL_ERROR "Lua is needed to build the project. Please install it correctly.")
endif()

include_directories(${LUA_INCLUDE_DIR})
link_directories(${LUA_LIBRARY_DIRS})

#------------------------------------------------------------------------------
# - tinyxml2
#------------------------------------------------------------------------------
if (WIN32)
	set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${WIN_LIBRARIES_PATH}/tinyxml2)
endif ()

find_package(TinyXml2 REQUIRED)

if(NOT TINYXML2_FOUND)
	message(FATAL_ERROR "tinyxml2 is needed to build the project. Please install it correctly.")
endif()

include_directories(${TINYXML2_INCLUDE_DIRS})

#------------------------------------------------------------------------------
# - OpenGL
#------------------------------------------------------------------------------
set(OpenGL_GL_PREFERENCE "LEGACY")

if (WIN32)
	set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${WIN_LIBRARIES_PATH}/glm)
endif ()

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)

if(NOT OPENGL_FOUND)
	message(FATAL_ERROR "OpenGL not found!")
endif(NOT OPENGL_FOUND)

if(NOT GLM_FOUND)
	message(FATAL_ERROR "glm not found!")
endif(NOT GLM_FOUND)

include_directories(${GLM_INCLUDE_DIRS})

#------------------------------------------------------------------------------
# - GLEW
#------------------------------------------------------------------------------
if (WIN32)
	set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${WIN_LIBRARIES_PATH}/glew)

	find_package(GLEW REQUIRED)

	if(NOT GLEW_FOUND)
		message(FATAL_ERROR "GLEW not found!")
	endif(NOT GLEW_FOUND)

	include_directories(${GLEW_INCLUDE_DIRS})
endif ()

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

#------------------------------------------------------------------------------
# - EnTT
#------------------------------------------------------------------------------
add_subdirectory(external/entt)
include_directories(external/entt/single_include)

#------------------------------------------------------------------------------
# - gamekit
#------------------------------------------------------------------------------
add_subdirectory(external/gamekit)
include_directories(external/gamekit/include)

#------------------------------------------------------------------------------
# - zlib
#------------------------------------------------------------------------------
add_subdirectory(external/zlib ${CMAKE_CURRENT_BINARY_DIR}/external/zlib)

#------------------------------------------------------------------------------
# - sol3 (sol2 v3.x)
#------------------------------------------------------------------------------
include_directories(external/sol2/single/include/sol)

#------------------------------------------------------------------------------
# Subdirectories
#------------------------------------------------------------------------------
add_subdirectory(source/common)
add_subdirectory(source/server)
add_subdirectory(source/client)
