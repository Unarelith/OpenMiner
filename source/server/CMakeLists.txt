#------------------------------------------------------------------------------
# Set project name
#------------------------------------------------------------------------------
set(PROJECT_NAME ${PROJECT_NAME}_server)

#------------------------------------------------------------------------------
# Get source files
#------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} */*.cpp)
file(GLOB_RECURSE HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${SOURCE_FILES} main.cpp)

#------------------------------------------------------------------------------
# Add executables
#------------------------------------------------------------------------------
add_library(${PROJECT_NAME}_lib STATIC ${HEADER_FILES} ${SOURCE_FILES})
add_executable(${PROJECT_NAME} main.cpp)

add_dependencies(${PROJECT_NAME}_lib ${CMAKE_PROJECT_NAME}_common)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_lib)

#------------------------------------------------------------------------------
# Setup include directories
#------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}_lib PUBLIC ../../external ../../external/entt/single_include)

foreach(HEADER_FILE ${HEADER_FILES})
	get_filename_component(HEADER_DIRECTORY ${HEADER_FILE} DIRECTORY)
	target_include_directories(${PROJECT_NAME}_lib PUBLIC ${HEADER_DIRECTORY})
endforeach(HEADER_FILE)

#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------
if (NOT MSVC)
	target_compile_options(${PROJECT_NAME}_lib PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_GCC_FLAGS}>")
	target_compile_options(${PROJECT_NAME}_lib PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_GCC_FLAGS}>")
	target_compile_options(${PROJECT_NAME}_lib PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:${RELWITHDEB_GCC_FLAGS}>")
	target_compile_options(${PROJECT_NAME}_lib PRIVATE ${CLANG_FLAGS})

	target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_GCC_FLAGS}>")
	target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_GCC_FLAGS}>")
	target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:${RELWITHDEB_GCC_FLAGS}>")
	target_compile_options(${PROJECT_NAME} PRIVATE ${CLANG_FLAGS})
endif ()

target_compile_options(${PROJECT_NAME}_lib PRIVATE -DGLM_FORCE_RADIANS)
target_compile_options(${PROJECT_NAME}_lib PRIVATE -D_USE_MATH_DEFINES)
target_compile_options(${PROJECT_NAME}_lib PRIVATE -DSOL_CHECK_ARGUMENTS -DSOL_PRINT_ERRORS=0)

target_compile_options(${PROJECT_NAME} PRIVATE -DGLM_FORCE_RADIANS)
target_compile_options(${PROJECT_NAME} PRIVATE -D_USE_MATH_DEFINES)
target_compile_options(${PROJECT_NAME} PRIVATE -DSOL_CHECK_ARGUMENTS -DSOL_PRINT_ERRORS=0)

if(MINGW)
	target_link_options(${PROJECT_NAME}_lib PRIVATE -static-libstdc++ -static-libgcc -mconsole)

	target_link_options(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc -mconsole)
endif()

# target_compile_options(${PROJECT_NAME}_lib PRIVATE -pg)
# target_compile_options(${PROJECT_NAME} PRIVATE -pg)
# target_link_options(${PROJECT_NAME}_lib PRIVATE -pg)
# target_link_options(${PROJECT_NAME} PRIVATE -pg)

#------------------------------------------------------------------------------
# Link options
#------------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}_lib PUBLIC ${CMAKE_PROJECT_NAME}_common)
target_link_libraries(${PROJECT_NAME}     PRIVATE ${PROJECT_NAME}_lib)

