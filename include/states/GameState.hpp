/*
 * =====================================================================================
 *
 *       Filename:  GameState.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15/12/2014 03:51:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef GAMESTATE_HPP_
#define GAMESTATE_HPP_

#include <glm/glm.hpp>

#include "ApplicationState.hpp"
#include "Camera.hpp"
#include "World.hpp"

class GameState : public ApplicationState {
	public:
		GameState();
		~GameState();
		
		void update();
		
		void draw();
		
	private:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		
		Camera m_camera;
		
		Shader m_shader;
		
		World m_world;
};

#endif // GAMESTATE_HPP_
