# Copyright (c) 2019 Quentin Bazin
# This file is licensed under LGPL 2.1
# See LICENSE file

# GAMEKIT_FOUND
# GAMEKIT_INCLUDE_DIR
# GAMEKIT_LIBARIES

find_path(GAMEKIT_INCLUDE_DIR NAMES gk)
find_library(GAMEKIT_LIBRARIES NAMES gamekit)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(gamekit DEFAULT_MSG GAMEKIT_LIBRARIES GAMEKIT_INCLUDE_DIR)

mark_as_advanced(GAMEKIT_INCLUDE_DIR GAMEKIT_LIBRARIES)

