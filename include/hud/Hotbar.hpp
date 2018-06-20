/*
 * =====================================================================================
 *
 *       Filename:  Hotbar.hpp
 *
 *    Description:
 *
 *        Created:  20/06/2018 05:40:05
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef HOTBAR_HPP_
#define HOTBAR_HPP_

#include "Image.hpp"

class Hotbar : public IDrawable {
	public:
		Hotbar();

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Texture m_texture;
		Image m_background;
};

#endif // HOTBAR_HPP_
