/*
 * =====================================================================================
 *
 *       Filename:  GameState.hpp
 *
 *    Description:
 *
 *        Created:  15/12/2014 03:51:32
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef GAMESTATE_HPP_
#define GAMESTATE_HPP_

#include <glm/glm.hpp>

#include "ApplicationState.hpp"
#include "BlockCursor.hpp"
#include "Camera.hpp"
#include "Crosshair.hpp"
#include "Hotbar.hpp"
#include "Skybox.hpp"
#include "World.hpp"

#include "RenderTarget.hpp"

class GameState : public ApplicationState {
	public:
		GameState();

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void initShaders();

		void draw(RenderTarget &target, RenderStates states) const override;

		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;

		Camera &m_camera{Camera::getInstance()};

		Shader m_shader;

		Skybox m_skybox;
		World m_world;

		glm::vec4 m_selectedBlock{0, 0, 0, -1};
		BlockCursor m_blockCursor{m_camera, m_world, m_viewMatrix, m_projectionMatrix};
		Crosshair m_crosshair;

		Texture m_widgetTexture;
		Inventory m_playerInventory{9, 3};
		Inventory m_hotbarInventory{9, 1};
		Hotbar m_hotbar{m_hotbarInventory};
};

#endif // GAMESTATE_HPP_
