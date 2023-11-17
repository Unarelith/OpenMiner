#------------------------------------------------------------------------------
# CMakeLists.txt
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.6)

project(openminer)

#------------------------------------------------------------------------------
# Setup
#------------------------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------
set(OM_BUILD_TESTS ON CACHE BOOL "Enable building tests if CxxTest is available")

#------------------------------------------------------------------------------
# Additional cmake files
#------------------------------------------------------------------------------
include(compiler_flags)
include(om_target)

#------------------------------------------------------------------------------
# Subdirectories
#------------------------------------------------------------------------------
add_subdirectory(external)

add_subdirectory(resources/shaders)

add_subdirectory(source/common)
add_subdirectory(source/server)
add_subdirectory(source/client)

add_subdirectory(tests)

if (MSVC)
	include(vs_folders)
endif()
