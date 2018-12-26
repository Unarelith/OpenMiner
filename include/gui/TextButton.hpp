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
		using Callback = std::function<void(TextButton &)>;
		TextButton(Widget *parent = nullptr);
		TextButton(const Callback &callback, Widget *parent = nullptr);

		void onEvent(const sf::Event &event) override;

		const std::string &text() const { return m_text.text(); }
		void setText(const std::string &text);
		void setCallback(const Callback &callback) { m_callback = callback; }
		void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Image m_background{"texture-widgets"};
		Image m_hoverBackground{"texture-widgets"};
		Image m_disabledBackground{"texture-widgets"};

		Text m_text;

		Callback m_callback;

		bool m_isHovered = false;
		bool m_isEnabled = true;
};

#endif // TEXTBUTTON_HPP_
