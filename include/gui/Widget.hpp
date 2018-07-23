/*
 * =====================================================================================
 *
 *       Filename:  Widget.hpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 07:19:27
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef WIDGET_HPP_
#define WIDGET_HPP_

#include "IDrawable.hpp"
#include "Rect.hpp"
#include "SDLHeaders.hpp"
#include "Transformable.hpp"

class Widget : public IDrawable, public Transformable {
	public:
		Widget(Widget *parent = nullptr) : m_parent(parent) {}
		Widget(unsigned int width, unsigned int height, Widget *parent = nullptr)
			: m_parent(parent), m_width(width), m_height(height) {}

		virtual void onEvent(const S_Event &) {}
		virtual void update() {}

		bool isPointInWidget(float x, float y);

		FloatRect getGlobalBounds() const;

		const Widget *parent() { return m_parent; }
		void setParent(Widget *parent) { m_parent = parent; }

		unsigned int width() const { return m_width; }
		unsigned int height() const { return m_height; }

	protected:
		Widget *m_parent = nullptr;

		unsigned int m_width = 0;
		unsigned int m_height = 0;
};

#endif // WIDGET_HPP_
