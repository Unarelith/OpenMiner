cmake_minimum_required(VERSION 2.8)

project(KubKraft)

file(GLOB SOURCE_FILES source/*/*.cpp source/*.cpp external/*.cpp)
file(GLOB HEADER_FILES include/*)

add_executable(KubKraft
               ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wfatal-errors -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-variadic-macros")

include_directories(external)
include_directories(${HEADER_FILES})

find_package(OpenGL REQUIRED)

if(NOT OPENGL_FOUND)
	message(FATAL_ERROR "OpenGL not found !")
endif(NOT OPENGL_FOUND)

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2MIXER REQUIRED SDL2_mixer)

if(NOT SDL2_FOUND)
	message(FATAL_ERROR "SDL2 not found !")
endif(NOT SDL2_FOUND)

if(NOT SDL2IMAGE_FOUND)
	message(FATAL_ERROR "SDL2_image not found !")
endif(NOT SDL2IMAGE_FOUND)

if(NOT SDL2MIXER_FOUND)
	message(FATAL_ERROR "SDL2_mixer not found !")
endif(NOT SDL2MIXER_FOUND)

include_directories(${SDL2_INCLUDE_DIRS}
                    ${SDL2IMAGE_INCLUDE_DIRS}
					${SDL2MIXER_INCLUDE_DIRS})

target_link_libraries(KubKraft
					  ${OPENGL_LIBRARIES}
					  ${SDL2_LIBRARIES}
					  ${SDL2IMAGE_LIBRARIES}
					  ${SDL2MIXER_LIBRARIES})

