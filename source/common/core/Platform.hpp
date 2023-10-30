/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef PLATFORM_HPP_
#define PLATFORM_HPP_

////////////////////////////////////////////////////////////
// Identify the operating system
// see http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
// and SFML/Config.hpp
////////////////////////////////////////////////////////////
#if defined(_WIN32)
	// Windows
	#define OM_PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
	// Apple platform, see which one it is
	#include "TargetConditionals.h"

	#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
		// iOS
		#define OM_PLATFORM_IOS
	#elif TARGET_OS_MAC
		// MacOS
		#define OM_PLATFORM_MACOS
	#else
		// Unsupported Apple system
		#error This Apple operating system is not supported by OpenMiner
	#endif
#elif defined(__unix__)
	// UNIX system, see which one it is
	#if defined(__ANDROID__)
		// Android
		#define OM_PLATFORM_ANDROID
	#elif defined(__linux__)
		// Linux
		#define OM_PLATFORM_LINUX
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		// FreeBSD
		#define OM_PLATFORM_FREEBSD
	#elif defined(__OpenBSD__)
		// OpenBSD
		#define OM_PLATFORM_OPENBSD
	#else
		// Unsupported UNIX system
		#error This UNIX operating system is not supported by OpenMiner
	#endif
#else
	// Unsupported system
	#error This operating system is not supported by OpenMiner
#endif

#endif // PLATFORM_HPP_
