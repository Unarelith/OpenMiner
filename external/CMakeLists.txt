#------------------------------------------------------------------------------
# Packages
#------------------------------------------------------------------------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(BUILD_STATIC_LIBS ON CACHE BOOL "")

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
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/zlib/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

set(BUILD_SHARED_LIBS OFF)

#------------------------------------------------------------------------------
# - EnTT
#------------------------------------------------------------------------------
add_subdirectory(entt)

#------------------------------------------------------------------------------
# - SFML
#------------------------------------------------------------------------------
set(SFML_STATIC TRUE)

set(SFML_BUILD_AUDIO FALSE CACHE BOOL "")
set(SFML_BUILD_GRAPHICS FALSE CACHE BOOL "")
set(SFML_BUILD_NETWORK TRUE CACHE BOOL "")
set(SFML_BUILD_WINDOW FALSE CACHE BOOL "")

add_subdirectory(SFML)

#------------------------------------------------------------------------------
# - zlib
#------------------------------------------------------------------------------
add_subdirectory(zlib)

#------------------------------------------------------------------------------
# - lua
#------------------------------------------------------------------------------
add_subdirectory(lua)

#------------------------------------------------------------------------------
# - sol3 (sol2 v3.x)
#------------------------------------------------------------------------------
add_subdirectory(sol2)

#-----------------------------------------------------------------------------------------
# BGFX
#-----------------------------------------------------------------------------------------
set(BGFX_BUILD_TOOLS    ON  CACHE BOOL "Build bgfx tools."                   FORCE)
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "Build bgfx examples."                FORCE)
set(BGFX_INSTALL        OFF CACHE BOOL "Create installation target."         FORCE)
set(BGFX_CUSTOM_TARGETS OFF CACHE BOOL "Include convenience custom targets." FORCE)

add_subdirectory(bgfx.cmake)

#------------------------------------------------------------------------------
# - SDL
#------------------------------------------------------------------------------
set(MESSAGE_QUIET ON)

add_subdirectory(SDL)

set(SDL2IMAGE_INSTALL OFF CACHE BOOL "")

add_subdirectory(SDL_image)

unset(MESSAGE_QUIET)

#------------------------------------------------------------------------------
# - glm
#------------------------------------------------------------------------------
add_subdirectory(glm)

#------------------------------------------------------------------------------
# - tinyxml2
#------------------------------------------------------------------------------
add_subdirectory(tinyxml2)
