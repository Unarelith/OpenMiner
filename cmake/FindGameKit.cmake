# Copyright (c) 2019 Quentin Bazin
# This file is licensed under LGPL 2.1
# See LICENSE file

# GAMEKIT_FOUND
# GAMEKIT_INCLUDE_DIRS
# GAMEKIT_LIBARIES

find_path(GAMEKIT_INCLUDE_DIRS NAMES gk)
find_library(GAMEKIT_LIBRARIES NAMES gamekit)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(gamekit DEFAULT_MSG GAMEKIT_LIBRARIES GAMEKIT_INCLUDE_DIRS)

mark_as_advanced(GAMEKIT_INCLUDE_DIRS GAMEKIT_LIBRARIES)

