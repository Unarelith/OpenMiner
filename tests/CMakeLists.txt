#------------------------------------------------------------------------------
# Unit testing
#------------------------------------------------------------------------------
if(OM_BUILD_TESTS)
	find_package(CxxTest QUIET)

	if(CXXTEST_FOUND)
		include_directories(${CXXTEST_INCLUDE_DIRS})
		enable_testing()

		file(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/source/*.hpp")
		foreach(HEADER_FILE ${HEADER_FILES})
			get_filename_component(HEADER_DIRECTORY ${HEADER_FILE} DIRECTORY)
			include_directories(${HEADER_DIRECTORY})
		endforeach(HEADER_FILE)

		file(GLOB_RECURSE TEST_FILES "${PROJECT_SOURCE_DIR}/tests/*.hpp")
		CXXTEST_ADD_TEST(${CMAKE_PROJECT_NAME}_tests unit-test.cpp ${TEST_FILES})
		target_link_libraries(${CMAKE_PROJECT_NAME}_tests ${CMAKE_PROJECT_NAME}_common ${CMAKE_PROJECT_NAME}_server_lib)
	endif()
endif()
