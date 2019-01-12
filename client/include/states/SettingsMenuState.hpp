/*
 * =====================================================================================
 *
 *       Filename:  SettingsMenuState.hpp
 *
 *    Description:
 *
 *        Created:  29/06/2018 06:11:43
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SETTINGSMENUSTATE_HPP_
#define SETTINGSMENUSTATE_HPP_

#include <gk/core/ApplicationState.hpp>
#include <gk/gl/Shader.hpp>
#include <gk/graphics/RectangleShape.hpp>

#include "MenuWidget.hpp"

class SettingsMenuState : public gk::ApplicationState {
	public:
		SettingsMenuState(gk::ApplicationState *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void addMainButtons();
		void addGameplayButtons();
		void addGraphicsButtons();
		void addInputButtons();

		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		MenuWidget m_menuWidget{1, 8};

		gk::Shader m_shader;

		glm::mat4 m_projectionMatrix;

		gk::RectangleShape m_background;
};

#endif // SETTINGSMENUSTATE_HPP_
