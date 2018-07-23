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

#include "ApplicationState.hpp"
#include "MenuWidget.hpp"
#include "Shader.hpp"
#include "RectangleShape.hpp"

class SettingsMenuState : public ApplicationState {
	public:
		SettingsMenuState(ApplicationState *parent = nullptr);

		void onEvent(const S_Event &event) override;

		void update() override;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		MenuWidget m_menuWidget{1, 8};

		Shader m_shader;

		RectangleShape m_background;
};

#endif // SETTINGSMENUSTATE_HPP_
