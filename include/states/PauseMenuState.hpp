/*
 * =====================================================================================
 *
 *       Filename:  PauseMenuState.hpp
 *
 *    Description:
 *
 *        Created:  28/06/2018 10:14:02
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef PAUSEMENUSTATE_HPP_
#define PAUSEMENUSTATE_HPP_

#include "ApplicationState.hpp"
#include "MenuWidget.hpp"
#include "Shader.hpp"
#include "RectangleShape.hpp"

class PauseMenuState : public ApplicationState {
	public:
		PauseMenuState(ApplicationState *parent = nullptr);

		void onEvent(const S_Event &event) override;

		void update() override;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		MenuWidget m_menuWidget{1, 3};

		Shader m_shader;

		RectangleShape m_background;
};

#endif // PAUSEMENUSTATE_HPP_
