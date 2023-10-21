file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/shaders")

function(compile_shaders)
	if(UNIX AND NOT APPLE)
		set(platform linux)
		set(PROFILES 120 spirv)
	else()
		set(platform windows)
		set(PROFILES 120 spirv s_5_0)
	endif()

	foreach(profile IN LISTS PROFILES)
		foreach(shader_name IN LISTS ARGN)
			set(IN_VERTEX    "${CMAKE_SOURCE_DIR}/resources/shaders/${shader_name}.vs.sc")
			set(IN_FRAGMENT  "${CMAKE_SOURCE_DIR}/resources/shaders/${shader_name}.fs.sc")
			set(IN_DEF       "${CMAKE_SOURCE_DIR}/resources/shaders/${shader_name}.def.sc")

			set(OUT_VERTEX   "${CMAKE_SOURCE_DIR}/resources/shaders/${shader_name}.${profile}.vs.bin")
			set(OUT_FRAGMENT "${CMAKE_SOURCE_DIR}/resources/shaders/${shader_name}.${profile}.fs.bin")

			set(BGFX_SHADER_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/external/bgfx.cmake/bgfx/src")

			set(BGFX_SHADERC bgfx::shaderc)

			add_custom_command(
				OUTPUT ${OUT_VERTEX}
				DEPENDS ${IN_VERTEX} ${IN_DEF}
				COMMAND ${BGFX_SHADERC} --type vertex --platform ${platform} --profile ${profile} -f ${IN_VERTEX} --varyingdef ${IN_DEF} -o ${OUT_VERTEX} -i "${BGFX_SHADER_INCLUDE_PATH}"
			)

			add_custom_command(
				OUTPUT ${OUT_FRAGMENT}
				DEPENDS ${IN_FRAGMENT} ${IN_DEF}
				COMMAND ${BGFX_SHADERC} --type fragment --platform ${platform} --profile ${profile} -f ${IN_FRAGMENT} --varyingdef ${IN_DEF} -o ${OUT_FRAGMENT} -i "${BGFX_SHADER_INCLUDE_PATH}"
			)

			list(APPEND SHADER_SOURCE_LIST ${IN_VERTEX} ${IN_FRAGMENT} ${IN_DEF})
			list(APPEND SHADER_OUTPUT_LIST ${OUT_VERTEX} ${OUT_FRAGMENT})
		endforeach()
	endforeach()

	add_custom_target(shaders DEPENDS ${SHADER_OUTPUT_LIST})

	source_group(TREE "${CMAKE_SOURCE_DIR}/resources/shaders/" PREFIX "Shader Files" FILES ${SHADER_SOURCE_LIST})
	target_sources(shaders PRIVATE ${SHADER_SOURCE_LIST})
endfunction()

compile_shaders(
	basic
	game
	screen
	skybox
)
