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

#include "ApplicationState.hpp"
#include "Config.hpp"
#include "RectangleShape.hpp"
#include "Shader.hpp"
#include "WorkbenchWidget.hpp"

class InventoryState : public ApplicationState {
	public:
		InventoryState(ApplicationState *parent = nullptr);

		template<typename T, typename... Args>
		void setupWidget(Args &&...args) {
			m_widget.reset(new T(std::forward<Args>(args)...));

			m_widget->setScale(GUI_SCALE, GUI_SCALE, 1);
			m_widget->setPosition(SCREEN_WIDTH  / 2.0 - m_widget->getGlobalBounds().width  / 2.0,
			                      SCREEN_HEIGHT / 2.0 - m_widget->getGlobalBounds().height / 2.0, 0);
		}

		void onEvent(const S_Event &event) override;

		void update() override;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Shader m_shader;

		std::unique_ptr<Widget> m_widget;

		RectangleShape m_background;
};

#endif // INVENTORYSTATE_HPP_
