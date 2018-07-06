
# - Find JSONCPP
#
# Find JSONCPP includes and library
#
# This module defines the following variables:
#  JSONCPP_FOUND - True if libjsoncpp has been found.
#  JSONCPP_INCLUDE_DIRS - location to look for headers,
#  JSONCPP_LIBRARIES - the libraries to link against libjsoncpp

find_path(JSONCPP_INCLUDE_DIR json/features.h
	PATH_SUFFIXES include)
find_library(JSONCPP_LIBRARY jsoncpp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JSONCPP DEFAULT_MSG
	JSONCPP_INCLUDE_DIR JSONCPP_LIBRARY)

if(NOT JSONCPP_FOUND)
	message(FATAL_ERROR "libjsoncpp missing" )
endif(NOT JSONCPP_FOUND)

set(JSONCPP_LIBRARIES ${JSONCPP_LIBRARY})
mark_as_advanced(
	JSONCPP_INCLUDE_DIRS
	JSONCPP_LIBRARIES
	)
