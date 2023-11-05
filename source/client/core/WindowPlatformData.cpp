#include <SDL.h>
#include <SDL_syswm.h>

#include <bgfx/bgfx.h>
#include <bx/platform.h>

bool BGFX_setupPlatformData(SDL_Window *window, bgfx::PlatformData &pd)
{
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	if (!SDL_GetWindowWMInfo(window, &wmi)) {
		return false;
	}

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	pd.ndt = wmi.info.x11.display;
	pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_OSX
	pd.ndt = NULL;
	pd.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
	pd.ndt = NULL;
	pd.nwh = wmi.info.win.window;
#elif BX_PLATFORM_STEAMLINK
	pd.ndt = wmi.info.vivante.display;
	pd.nwh = wmi.info.vivante.window;
#endif // BX_PLATFORM_
	pd.context = NULL;
	pd.backBuffer = NULL;
	pd.backBufferDS = NULL;

	return true;
}
