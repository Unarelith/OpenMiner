/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:
 *
 *        Created:  13/12/2014 20:49:00
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
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

int main(int argc, char **argv) {
	ClientApplication app(argc, argv);
	app.run();

	return 0;
}

