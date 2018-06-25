/*
 * =====================================================================================
 *
 *       Filename:  Text.hpp
 *
 *    Description:
 *
 *        Created:  24/06/2018 01:48:24
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef TEXT_HPP_
#define TEXT_HPP_

#include <string>

#include "IDrawable.hpp"
#include "Transformable.hpp"
#include "VertexBuffer.hpp"

class Text : public IDrawable, public Transformable {
	public:
		Text();

		void setText(const std::string &text) { m_text = text; }

	private:
		void updateCharWidth();

		void draw(RenderTarget &target, RenderStates states) const override;

		std::string m_text;

		int m_charWidth[256];

		Texture &m_texture;
		VertexBuffer m_vbo;
};

#endif // TEXT_HPP_
