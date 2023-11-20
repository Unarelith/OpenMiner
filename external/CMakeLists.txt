set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(BUILD_STATIC_LIBS ON CACHE BOOL "")

update_git_submodules()

#------------------------------------------------------------------------------
# Libraries
#------------------------------------------------------------------------------
add_subdirectory(entt)
add_subdirectory(glm)
add_subdirectory(lua)
add_subdirectory(sol2)
add_subdirectory(tinyxml2)
add_subdirectory(zlib)

#------------------------------------------------------------------------------
# - BGFX
#------------------------------------------------------------------------------
set(BGFX_BUILD_TOOLS    ON  CACHE BOOL "" FORCE)
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGFX_INSTALL        OFF CACHE BOOL "" FORCE)
set(BGFX_CUSTOM_TARGETS OFF CACHE BOOL "" FORCE)

add_subdirectory(bgfx.cmake)

#------------------------------------------------------------------------------
# - SDL
#------------------------------------------------------------------------------
set(MESSAGE_QUIET ON)
set(SDL2IMAGE_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(SDL)
add_subdirectory(SDL_image)

unset(MESSAGE_QUIET)

#------------------------------------------------------------------------------
# - SFML
#------------------------------------------------------------------------------
set(SFML_STATIC TRUE)

set(SFML_BUILD_AUDIO    FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK  TRUE  CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW   FALSE CACHE BOOL "" FORCE)

add_subdirectory(SFML)
