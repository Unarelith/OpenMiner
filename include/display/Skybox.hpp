/*
 * =====================================================================================
 *
 *       Filename:  Skybox.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25/12/2014 23:43:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef SKYBOX_HPP_
#define SKYBOX_HPP_

#include "Shader.hpp"
#include "VertexBuffer.hpp"

class Skybox {
	public:
		Skybox();
		~Skybox();
		
		void draw(Shader &shader);
		
	private:
		VertexBuffer m_vbo;
};

#endif // SKYBOX_HPP_
