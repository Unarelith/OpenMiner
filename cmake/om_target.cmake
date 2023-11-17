function(om_setup_target PROJECT_NAME)
	target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

	# FIXME: Use target_include_directories(PUBLIC ...) in zlib/lua
	target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/external/entt/src)
	target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/external/zlib ${CMAKE_CURRENT_BINARY_DIR}/external/zlib)
	target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/external/lua/src)
	target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/external/FastNoiseLite)

	if (NOT MSVC)
		target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_GCC_FLAGS}>")
		target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_GCC_FLAGS}>")
		target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:RELWITHDEBINFO>:${RELWITHDEB_GCC_FLAGS}>")
		target_compile_options(${PROJECT_NAME} PUBLIC ${CLANG_FLAGS})
	endif ()

	target_compile_options(${PROJECT_NAME} PUBLIC -D_USE_MATH_DEFINES)
	target_compile_options(${PROJECT_NAME} PUBLIC -DSOL_CHECK_ARGUMENTS -DSOL_PRINT_ERRORS=0)

	# target_compile_options(${PROJECT_NAME} PRIVATE -pg)
endfunction()

function(om_add_library PROJECT_NAME HEADER_FILES SOURCE_FILES)
	message(${SOURCE_FILES})
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header Files" FILES ${HEADER_FILES})
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${SOURCE_FILES})

	add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

	om_setup_target(${PROJECT_NAME})
endfunction()

function(om_add_executable PROJECT_NAME HEADER_FILES SOURCE_FILES)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header Files" FILES ${HEADER_FILES})
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${SOURCE_FILES})

	add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

	om_setup_target(${PROJECT_NAME})

	if(MINGW)
		target_link_options(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc -mconsole)
	endif()
endfunction()
