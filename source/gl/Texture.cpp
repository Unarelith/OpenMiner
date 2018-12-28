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
	SDL_Surface *surface = IMG_Load(filename.c_str());
	if(!surface) {
		throw EXCEPTION("Failed to load texture:", filename);
	}

	m_filename = filename;

	m_width = surface->w;
	m_height = surface->h;

	glGenTextures(1, &m_texture);

	bind(this);

	GLenum format = (surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

	glGenerateMipmap(GL_TEXTURE_2D);

	// FIXME: GL_NEAREST_MIPMAP_LINEAR causes blue shadows on trees, probably due to blending..
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

	bind(nullptr);

	SDL_FreeSurface(surface);
}

void Texture::bind(const Texture *texture) {
	if(texture) {
		glBindTexture(GL_TEXTURE_2D, texture->m_texture);
	} else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

