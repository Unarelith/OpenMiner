/*
 * =====================================================================================
 *
 *       Filename:  TextButton.hpp
 *
 *    Description:
 *
 *        Created:  28/06/2018 10:16:52
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef TEXTBUTTON_HPP_
#define TEXTBUTTON_HPP_

#include <functional>

#include "Image.hpp"
#include "Text.hpp"
#include "Widget.hpp"

class TextButton : public Widget {
	public:
		using Callback = std::function<void(void)>;
		TextButton(const Callback &callback, Widget *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void setText(const std::string &text);

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Image m_background{"texture-widgets"};
		Image m_hoverBackground{"texture-widgets"};

		Text m_text;

		Callback m_callback;

		bool m_isHovered = false;
};

#endif // TEXTBUTTON_HPP_
