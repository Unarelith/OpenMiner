/*
 * =====================================================================================
 *
 *       Filename:  Texture.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/12/2014 01:15:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <string>

#include "OpenGL.hpp"
#include "Types.hpp"

class Texture {
	public:
		Texture();
		Texture(const std::string &filename);
		~Texture();
		
		void load(const std::string &filename);
		
		static void bind(const Texture *texture);
		
		u16 width() const { return m_width; }
		u16 height() const { return m_height; }
		
	private:
		std::string m_filename;
		
		u16 m_width;
		u16 m_height;
		
		GLuint m_texture;
};

#endif // TEXTURE_HPP_
