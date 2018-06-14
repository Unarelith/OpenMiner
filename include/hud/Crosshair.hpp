/*
 * =====================================================================================
 *
 *       Filename:  Crosshair.hpp
 *
 *    Description:
 *
 *        Created:  14/06/2018 18:09:45
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CROSSHAIR_HPP_
#define CROSSHAIR_HPP_

#include "Shader.hpp"
#include "VertexBuffer.hpp"

class Crosshair {
	public:
		Crosshair();

		void draw(Shader &shader);

	private:
		VertexBuffer m_vbo;
};

#endif // CROSSHAIR_HPP_
