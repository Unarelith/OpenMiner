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

#include <gk/graphics/RectangleShape.hpp>

class Crosshair : public gk::Drawable {
	public:
		Crosshair();

		void setup();

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::VertexBuffer m_vbo;

		gk::RectangleShape m_hShape;
		gk::RectangleShape m_vShape1;
		gk::RectangleShape m_vShape2;
};

#endif // CROSSHAIR_HPP_
