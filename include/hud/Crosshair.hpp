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

#include "IDrawable.hpp"
#include "VertexBuffer.hpp"

class Crosshair : public IDrawable {
	public:
		Crosshair();

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		VertexBuffer m_vbo{GL_LINES, 0, 4};
};

#endif // CROSSHAIR_HPP_
