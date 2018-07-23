/*
 * =====================================================================================
 *
 *       Filename:  Texture.cpp
 *
 *    Description:
 *
 *        Created:  20/12/2014 01:15:43
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Exception.hpp"
#include "SDLHeaders.hpp"
#include "Texture.hpp"

Texture::Texture(const std::string &filename) {
	loadFromFile(filename);
}

Texture::Texture(Texture &&texture) {
	m_filename = texture.m_filename;

	m_width = texture.m_width;
	m_height = texture.m_height;

	m_texture = texture.m_texture;
	texture.m_texture = 0;
}

Texture::~Texture() noexcept {
	glDeleteTextures(1, &m_texture);
}

void Texture::loadFromFile(const std::string &filename) {
#ifdef USE_SDL
	SDL_Surface *surface = IMG_Load(filename.c_str());
	if(!surface) {
		throw EXCEPTION("Failed to load texture:", filename);
	}
#elif defined USE_SFML
	sf::Image image;
	if(!image.loadFromFile(filename)) {
		throw EXCEPTION("Failed to load texture:", filename);
	}
#endif // USE_SDL, USE_SFML

	m_filename = filename;

#ifdef USE_SDL
	m_width = surface->w;
	m_height = surface->h;
#elif defined USE_SFML
    m_width = image.getSize().x;
    m_height = image.getSize().y;
#endif // USE_SDL, USE_SFML

	glGenTextures(1, &m_texture);

	bind(this);

#ifdef USE_SDL
	GLenum format = (surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
#elif defined USE_SFML
	GLenum format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, image.getPixelsPtr());
#endif // USE_SDL, USE_SFML

	glGenerateMipmap(GL_TEXTURE_2D);

	// FIXME: GL_NEAREST_MIPMAP_LINEAR causes blue shadows on trees, probably due to blending..
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

	bind(nullptr);

#ifdef USE_SDL
	SDL_FreeSurface(surface);
#endif // USE_SDL
}

void Texture::bind(const Texture *texture) {
	if(texture) {
		glBindTexture(GL_TEXTURE_2D, texture->m_texture);
	} else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

