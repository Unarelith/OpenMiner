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

#include "ApplicationState.hpp"
#include "Shader.hpp"
#include "WorkbenchWidget.hpp"

class InventoryState : public ApplicationState {
	public:
		InventoryState(ApplicationState *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Shader m_shader;

		WorkbenchWidget m_widget;
};

#endif // INVENTORYSTATE_HPP_
