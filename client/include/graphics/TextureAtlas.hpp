/*
 * =====================================================================================
 *
 *       Filename:  TextureAtlas.hpp
 *
 *    Description:
 *
 *        Created:  29/01/2020 16:34:15
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef TEXTUREATLAS_HPP_
#define TEXTUREATLAS_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include <gk/core/IntTypes.hpp>
#include <gk/core/SDLHeaders.hpp>

class TextureAtlas {
	public:
		TextureAtlas() = default;

		void addFile(const std::string &filename);
		void packTextures();
		void saveToFile(const std::string &filename);

		u16 getTextureID(const std::string &filename);

		const SDL_Surface *getSurface() const { return m_atlas.get(); }

	private:
		// Expected texture size
		u16 m_width = 0;
		u16 m_height = 0;

		// Mapping between filename and internal texture ID
		std::unordered_map<std::string, u16> m_textureMap;

		// Textures to pack together
		using SurfacePtr = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;
		std::unordered_map<std::string, SurfacePtr> m_textures;

		// Packed texture
		SurfacePtr m_atlas{nullptr, &SDL_FreeSurface};
};

#endif // TEXTUREATLAS_HPP_
