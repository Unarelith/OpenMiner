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
#include <vector>

#include <gk/core/IntTypes.hpp>
#include <gk/core/SDLHeaders.hpp>
#include <gk/core/Rect.hpp>
#include <gk/gl/Texture.hpp>

class TextureAtlas {
	public:
		TextureAtlas() = default;

		void addFile(const std::string &path, const std::string &filename);
		void packTextures();

		void loadFromRegistry();

		u16 getTextureID(const std::string &filename) const;
		gk::FloatRect getTexCoords(const std::string &filename, bool normalized = true) const;

		const gk::Texture &texture() const { return m_texture; }

		bool isReady() const { return m_isReady; }

	private:
		// Expected texture size
		u16 m_tileSize = 0;

		// Mapping between filename and internal texture ID
		std::unordered_map<std::string, u16> m_textureMap;

		// Textures to pack together
		using SurfacePtr = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;
		std::vector<SurfacePtr> m_textures;

		// Packed texture
		gk::Texture m_texture;

		// Is the texture atlas ready to use?
		bool m_isReady = false;
};

#endif // TEXTUREATLAS_HPP_
