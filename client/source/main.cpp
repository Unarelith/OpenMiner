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
#include "ClientApplication.hpp"

// Use discrete GPU by default in Windows
// from https://gist.github.com/statico/6809850727c708f08458
// Thanks to Hopson for pointing this out
#ifdef _WIN32
extern "C" {
	#include <Windows.h>

	// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

	// https://gpuopen.com/amdpowerxpressrequesthighperformance/
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

// Fix the scaling problem encountered with Windows
// From: https://discourse.libsdl.org/t/sdl-getdesktopdisplaymode-resolution-reported-in-windows-10-when-using-app-scaling/22389/4
#ifdef _WIN32
typedef enum PROCESS_DPI_AWARENESS {
	PROCESS_DPI_UNAWARE = 0,
	PROCESS_SYSTEM_DPI_AWARE = 1,
	PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

void setDpiAwareness() {
	BOOL(WINAPI *SetProcessDPIAware)(void); // Vista and later
	HRESULT(WINAPI *SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS dpiAwareness); // Windows 8.1 and later

	void *userDLL = SDL_LoadObject("USER32.DLL");
	if (userDLL) {
		SetProcessDPIAware = (BOOL(WINAPI *)(void))SDL_LoadFunction(userDLL, "SetProcessDPIAware");
	}

	void *shcoreDLL = SDL_LoadObject("SHCORE.DLL");
	if (shcoreDLL) {
		SetProcessDpiAwareness = (HRESULT(WINAPI *)(PROCESS_DPI_AWARENESS))SDL_LoadFunction(shcoreDLL, "SetProcessDpiAwareness");
	}

	if (SetProcessDpiAwareness) {
		// Try Windows 8.1+ version
		HRESULT result = SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
	}
	else if (SetProcessDPIAware) {
		// Try Vista - Windows 8 version.
		// This has a constant scale factor for all monitors.
		BOOL success = SetProcessDPIAware();
	}
}
#endif

int main(int argc, char **argv) {
#ifdef _WIN32
	setDpiAwareness();
#endif

	ClientApplication app(argc, argv);
	app.run();

	return 0;
}

