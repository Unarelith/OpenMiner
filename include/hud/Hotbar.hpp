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

#include <vector>

#include "Image.hpp"
#include "SDLHeaders.hpp"

class Hotbar : public IDrawable {
	public:
		Hotbar();

		void onEvent(const SDL_Event &event);

		void addItem(u16 id);

		int cursorPos() const { return m_cursorPos; }

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Texture m_texture;
		Image m_background;

		Image m_cursor;
		int m_cursorPos = 0;

		Texture m_blocksTexture;
		// std::vector<Image> m_items;
};

#endif // HOTBAR_HPP_
