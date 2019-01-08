/*
 * =====================================================================================
 *
 *       Filename:  InventoryState.hpp
 *
 *    Description:
 *
 *        Created:  20/06/2018 23:11:44
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef INVENTORYSTATE_HPP_
#define INVENTORYSTATE_HPP_

#include <memory>

#include <gk/core/ApplicationState.hpp>
#include <gk/graphics/RectangleShape.hpp>
#include <gk/gl/Shader.hpp>

#include "Config.hpp"
#include "Widget.hpp"

// FIXME: Old class used when widgets were defined from C++
class InventoryState : public gk::ApplicationState {
	public:
		InventoryState(gk::ApplicationState *parent = nullptr);

		template<typename T, typename... Args>
		void setupWidget(Args &&...args) {
			m_widget.reset(new T(std::forward<Args>(args)...));

			m_widget->setScale(GUI_SCALE, GUI_SCALE, 1);
			m_widget->setPosition(SCREEN_WIDTH  / 2.0 - m_widget->getGlobalBounds().width  / 2.0,
			                      SCREEN_HEIGHT / 2.0 - m_widget->getGlobalBounds().height / 2.0, 0);
		}

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		gk::Shader m_shader;
		glm::mat4 m_projectionMatrix;

		std::unique_ptr<Widget> m_widget;

		gk::RectangleShape m_background;
};

#endif // INVENTORYSTATE_HPP_
