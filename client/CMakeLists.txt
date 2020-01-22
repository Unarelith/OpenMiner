#------------------------------------------------------------------------------
# Get source files
#------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES source/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.hpp ../common/*.hpp ../external/*.hpp)

foreach(HEADER_FILE ${HEADER_FILES})
	get_filename_component(HEADER_DIRECTORY ${HEADER_FILE} DIRECTORY)
	include_directories(${HEADER_DIRECTORY})
endforeach(HEADER_FILE)

#------------------------------------------------------------------------------
# Add executables
#------------------------------------------------------------------------------
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
add_dependencies(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_common)

#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
if (NOT MSVC)
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -O3 -ffast-math)
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g -Wall -Wextra -Wfatal-errors -Wno-variadic-macros)
#else ()
#target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /O2)
endif ()

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -DDEBUG_ENABLED)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -DGLM_FORCE_RADIANS)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -DSOL_CHECK_ARGUMENTS
	# -DSOL_SAFE_USERTYPE=1
	# -DSOL_SAFE_REFERENCES=1
	# -DSOL_SAFE_FUNCTION_CALLS=1
	# -DSOL_SAFE_FUNCTION=1
	# -DSOL_SAFE_NUMERICS=1
	# -DSOL_SAFE_GETTER=1
	-DSOL_PRINT_ERRORS=1
)

set(CMAKE_CXX_STANDARD 14)

# target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -std=c++14)
# target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)

# target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -pg)
# target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -pg)

#------------------------------------------------------------------------------
# Link options
#------------------------------------------------------------------------------
if (UNIX)
	set (UNIX_LIBS -ldl)
endif ()

target_link_libraries(${CMAKE_PROJECT_NAME}
                      ${OPENGL_LIBRARIES}
                      ${SDL2_LIBRARIES}
                      ${SDL2_IMAGE_LIBRARIES}
                      ${SDL2_MIXER_LIBRARIES}
                      ${SDL2_TTF_LIBRARIES}
                      ${TINYXML2_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      ${GAMEKIT_LIBRARIES}
                      ${LUA_LIBRARIES}
                      sfml-system
                      sfml-network
					  ${UNIX_LIBS}
                      ${CMAKE_PROJECT_NAME}_common)

