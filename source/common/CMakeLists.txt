#------------------------------------------------------------------------------
# Set project name
#------------------------------------------------------------------------------
set(PROJECT_NAME ${CMAKE_PROJECT_NAME}_common)

#------------------------------------------------------------------------------
# Get source files
#------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${SOURCE_FILES})

#------------------------------------------------------------------------------
# Add library
#------------------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

#------------------------------------------------------------------------------
# Setup include directories
#------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# FIXME: Use target_include_directories(PUBLIC ...) in zlib/lua
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/external/entt/src)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/external/zlib ${CMAKE_CURRENT_BINARY_DIR}/external/zlib)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/external/lua/src)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/external/FastNoiseLite)

#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
if (NOT MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_GCC_FLAGS}>")
	target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_GCC_FLAGS}>")
	target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:${RELWITHDEB_GCC_FLAGS}>")
	target_compile_options(${PROJECT_NAME} PRIVATE ${CLANG_FLAGS})
endif ()

target_compile_options(${PROJECT_NAME} PRIVATE -D_USE_MATH_DEFINES)
target_compile_options(${PROJECT_NAME} PRIVATE -DSOL_CHECK_ARGUMENTS -DSOL_PRINT_ERRORS=0)

# target_compile_options(${PROJECT_NAME} PRIVATE -pg)

if (UNIX)
	set (UNIX_LIBS -ldl -lpthread)
endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC
	sfml-network
	sfml-system
	sol2
	SDL2_image
	SDL2-static
	SDL2main
	tinyxml2
	glm_static
	libluajit
	zlib
	EnTT
	${UNIX_LIBS}
)

