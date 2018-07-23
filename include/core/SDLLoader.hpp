/*
 * =====================================================================================
 *
 *       Filename:  SDLLoader.hpp
 *
 *    Description:
 *
 *        Created:  15/09/2014 00:06:50
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SDLLOADER_HPP_
#define SDLLOADER_HPP_

class SDLLoader {
	public:
		SDLLoader() = default;
		SDLLoader(const SDLLoader &) = delete;
		SDLLoader(SDLLoader &&) = delete;
		~SDLLoader();

		SDLLoader& operator=(const SDLLoader &) = delete;
		SDLLoader& operator=(SDLLoader &&) = delete;

		void load();

	private:
		bool m_sdlInitialized = false;
		bool m_imgInitialized = false;
		bool m_mixInitialized = false;
};

#endif // SDLLOADER_HPP_
