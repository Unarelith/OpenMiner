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

#include "RectangleShape.hpp"
#include "Shader.hpp"

class Crosshair : public IDrawable {
	public:
		Crosshair();

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		VertexBuffer m_vbo;

		Shader m_shader;

		RectangleShape m_hShape;
		RectangleShape m_vShape1;
		RectangleShape m_vShape2;
};

#endif // CROSSHAIR_HPP_
