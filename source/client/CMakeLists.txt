#------------------------------------------------------------------------------
# Set project name
#------------------------------------------------------------------------------
set(PROJECT_NAME ${CMAKE_PROJECT_NAME})

#------------------------------------------------------------------------------
# Get source files
#------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.hpp ../server/*.hpp ../common/*.hpp)

foreach(HEADER_FILE ${HEADER_FILES})
	get_filename_component(HEADER_DIRECTORY ${HEADER_FILE} DIRECTORY)
	include_directories(${HEADER_DIRECTORY})
endforeach(HEADER_FILE)

#------------------------------------------------------------------------------
# Add executables
#------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}_server_lib ${CMAKE_PROJECT_NAME}_common)

#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
if (NOT MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_GCC_FLAGS}>")
	target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_GCC_FLAGS}>")
	target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:${RELWITHDEB_GCC_FLAGS}>")
endif ()

target_compile_options(${PROJECT_NAME} PRIVATE -DDEBUG_ENABLED)
target_compile_options(${PROJECT_NAME} PRIVATE -DGLM_FORCE_RADIANS)
target_compile_options(${PROJECT_NAME} PRIVATE -DSOL_CHECK_ARGUMENTS -DSOL_PRINT_ERRORS=0)

if(MINGW)
	target_link_options(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc -mconsole)
endif()

# target_compile_options(${PROJECT_NAME} PRIVATE -pg)
# target_link_options(${PROJECT_NAME} PRIVATE -pg)

#------------------------------------------------------------------------------
# Link options
#------------------------------------------------------------------------------
if (UNIX)
	set (UNIX_LIBS -ldl -lpthread)
endif ()

target_link_libraries(${PROJECT_NAME}
                      ${CMAKE_PROJECT_NAME}_server_lib
                      ${CMAKE_PROJECT_NAME}_common
                      gamekit
                      ${OPENGL_LIBRARIES}
                      ${SDL2_LIBRARIES}
                      ${SDL2_IMAGE_LIBRARIES}
                      ${SDL2_MIXER_LIBRARIES}
                      ${SDL2_TTF_LIBRARIES}
                      ${TINYXML2_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      ${LUA_LIBRARIES}
                      sfml-system
                      sfml-network
                      ${UNIX_LIBS})

