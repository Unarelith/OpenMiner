/*
 * =====================================================================================
 *
 *       Filename:  Skybox.hpp
 *
 *    Description:
 *
 *        Created:  25/12/2014 23:43:07
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SKYBOX_HPP_
#define SKYBOX_HPP_

#include <gk/gl/Shader.hpp>
#include <gk/gl/VertexBuffer.hpp>

class Skybox {
	public:
		Skybox();

		void draw(gk::Shader &shader);

	private:
		gk::VertexBuffer m_vbo;
};

#endif // SKYBOX_HPP_
