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
add_executable(${CMAKE_PROJECT_NAME}_server ${SOURCE_FILES})
add_dependencies(${CMAKE_PROJECT_NAME}_server ${CMAKE_PROJECT_NAME}_common)

#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
target_compile_options(${CMAKE_PROJECT_NAME}_server PRIVATE -O3 -ffast-math)
target_compile_options(${CMAKE_PROJECT_NAME}_server PRIVATE -g -Wall -Wextra -Wfatal-errors -Wno-variadic-macros)
target_compile_options(${CMAKE_PROJECT_NAME}_server PRIVATE -DDEBUG_ENABLED)
target_compile_options(${CMAKE_PROJECT_NAME}_server PRIVATE -DSOL_CHECK_ARGUMENTS
	# -DSOL_SAFE_USERTYPE=1
	# -DSOL_SAFE_REFERENCES=1
	# -DSOL_SAFE_FUNCTION_CALLS=1
	# -DSOL_SAFE_FUNCTION=1
	# -DSOL_SAFE_NUMERICS=1
	# -DSOL_SAFE_GETTER=1
	-DSOL_PRINT_ERRORS=1
)

target_compile_features(${CMAKE_PROJECT_NAME}_server PRIVATE cxx_std_17)

target_compile_options(${CMAKE_PROJECT_NAME}_server PRIVATE -pg)
target_link_options(${CMAKE_PROJECT_NAME}_server PRIVATE -pg)

#------------------------------------------------------------------------------
# Link options
#------------------------------------------------------------------------------
target_link_libraries(${CMAKE_PROJECT_NAME}_server
                      ${OPENGL_LIBRARIES}
                      ${SDL2_LIBRARIES}
                      ${SDL2IMAGE_LIBRARIES}
                      ${SDL2MIXER_LIBRARIES}
                      ${SDL2TTF_LIBRARIES}
                      ${TINYXML2_LIBRARIES}
                      ${GAMEKIT_LIBRARIES}
                      ${LUA_LIBRARIES}
                      sfml-network
                      ${CMAKE_PROJECT_NAME}_common)

