#------------------------------------------------------------------------------
# CMakeLists.txt
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1)

project(openminer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#------------------------------------------------------------------------------
# Packages
#------------------------------------------------------------------------------
# - Lua
#------------------------------------------------------------------------------
find_package(Lua REQUIRED)
if(NOT LUA_FOUND)
	message(FATAL_ERROR "Lua is needed to build the project. Please install it correctly.")
endif()

include_directories(${LUA_INCLUDE_DIRS})
link_directories(${LUA_LIBRARY_DIRS})

#------------------------------------------------------------------------------
# - gamekit
#------------------------------------------------------------------------------
find_package(GameKit REQUIRED)
if(NOT GAMEKIT_FOUND)
	message(FATAL_ERROR "gamekit is needed to build the project. Please install it correctly.")
endif()

include_directories(${GAMEKIT_INCLUDE_DIRS})
link_directories(${GAMEKIT_LIBRARY_DIRS})

#------------------------------------------------------------------------------
# - tinyxml2
#------------------------------------------------------------------------------
find_package(TinyXml2 REQUIRED)
if(NOT TINYXML2_FOUND)
	message(FATAL_ERROR "tinyxml2 is needed to build the project. Please install it correctly.")
endif()

include_directories(${TINYXML2_INCLUDE_DIRS})
link_directories(${TINYXML2_LIBRARY_DIRS})

#------------------------------------------------------------------------------
# - OpenGL
#------------------------------------------------------------------------------
set(OpenGL_GL_PREFERENCE "LEGACY")

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
# - SDL2, SDL2_image, SDL2_mixer
#------------------------------------------------------------------------------
include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2MIXER REQUIRED SDL2_mixer)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)

if(NOT SDL2_FOUND)
	message(FATAL_ERROR "SDL2 not found!")
endif(NOT SDL2_FOUND)

if(NOT SDL2IMAGE_FOUND)
	message(FATAL_ERROR "SDL2_image not found!")
endif(NOT SDL2IMAGE_FOUND)

if(NOT SDL2MIXER_FOUND)
	message(FATAL_ERROR "SDL2_mixer not found!")
endif(NOT SDL2MIXER_FOUND)

if(NOT SDL2TTF_FOUND)
	message(FATAL_ERROR "SDL2_ttf not found!")
endif(NOT SDL2TTF_FOUND)

include_directories(${SDL2_INCLUDE_DIRS}
                    ${SDL2IMAGE_INCLUDE_DIRS}
                    ${SDL2MIXER_INCLUDE_DIRS}
                    ${SDL2TTF_INCLUDE_DIRS})

#------------------------------------------------------------------------------
# - SFML network
#------------------------------------------------------------------------------
find_package(SFML COMPONENTS system network)
if(NOT SFML_FOUND)
	message(FATAL_ERROR "SFML is needed to build the project. Please install it correctly.")
endif()

include_directories(${SFML_INCLUDE_DIRS})

#------------------------------------------------------------------------------
# Subdirectories
#------------------------------------------------------------------------------
add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)

