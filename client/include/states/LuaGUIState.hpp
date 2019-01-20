/*
 * =====================================================================================
 *
 *       Filename:  LuaGUIState.hpp
 *
 *    Description:
 *
 *        Created:  07/01/2019 00:47:36
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef LUAGUISTATE_HPP_
#define LUAGUISTATE_HPP_

#include <memory>

#include <gk/core/ApplicationState.hpp>
#include <gk/gl/Shader.hpp>
#include <gk/graphics/RectangleShape.hpp>

#include "CraftingWidget.hpp"
#include "InventoryWidget.hpp"
#include "MouseItemWidget.hpp"

// FIXME: This class is almost a duplicate of InventoryState
class LuaGUIState : public gk::ApplicationState {
	public:
		LuaGUIState(/* LuaGUI &gui,  */gk::ApplicationState *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		// void loadGUI(LuaGUI &gui);

		gk::Shader m_shader;
		glm::mat4 m_orthoMatrix;

		Widget m_mainWidget;

		MouseItemWidget m_mouseItemWidget{&m_mainWidget};

		std::vector<CraftingWidget> m_craftingWidgets;
		std::vector<InventoryWidget> m_inventoryWidgets;
		std::vector<std::unique_ptr<Widget>> m_widgets;
		std::vector<std::unique_ptr<gk::IDrawable>> m_drawables;

		gk::RectangleShape m_background;
};

#endif // LUAGUISTATE_HPP_
