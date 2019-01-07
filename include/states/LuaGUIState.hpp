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

#include <gk/core/ApplicationState.hpp>
#include <gk/gl/Shader.hpp>
#include <gk/graphics/RectangleShape.hpp>

#include "LuaGUI.hpp"
#include "Widget.hpp"

// FIXME: This class is almost a duplicate of InventoryState
class LuaGUIState : public gk::ApplicationState {
	public:
		LuaGUIState(LuaGUI &gui, gk::ApplicationState *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		void loadGUI(LuaGUI &gui);

		gk::Shader m_shader;
		glm::mat4 m_orthoMatrix;

		std::vector<std::unique_ptr<Widget>> m_widgets;

		gk::RectangleShape m_background;

		Widget m_mainWidget;
};

#endif // LUAGUISTATE_HPP_
