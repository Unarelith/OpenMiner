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

#include <gk/graphics/Image.hpp>
// #include <gk/graphics/Text.hpp>

#include "Text.hpp"
#include "Widget.hpp"

class TextButton : public Widget {
	public:
		using Callback = std::function<void(TextButton &)>;
		TextButton(Widget *parent = nullptr);
		TextButton(const Callback &callback, Widget *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		const std::string &text() const { return m_text.text(); }
		void setText(const std::string &text);
		void setCallback(const Callback &callback) { m_callback = callback; }
		void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::Image m_background{"texture-widgets"};
		gk::Image m_hoverBackground{"texture-widgets"};
		gk::Image m_disabledBackground{"texture-widgets"};

		Text m_text;
		// gk::Text m_text;
		// gk::Text m_textShadow;

		Callback m_callback;

		// FIXME: Replace these by an enum State
		bool m_isHovered = false;
		bool m_isEnabled = true;
};

#endif // TEXTBUTTON_HPP_
