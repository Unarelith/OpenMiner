# From https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html
function(update_git_submodules)
	find_package(Git QUIET)

	if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
		if(GIT_SUBMODULE)
			message(STATUS "Git submodule update")
			execute_process(COMMAND ${GIT_EXECUTABLE} submodule sync -q --recursive WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
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
endfunction()
