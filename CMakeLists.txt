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
option(OM_BUILD_TESTS "Enable building tests if CxxTest is available" ON)
option(GIT_SUBMODULE  "Check git submodules during build"             ON)

#------------------------------------------------------------------------------
# Additional cmake files
#------------------------------------------------------------------------------
include(compiler_flags)
include(git_submodules)
include(om_target)
include(utils)

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
