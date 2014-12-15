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

#include "ApplicationState.hpp"
#include "OpenGL.hpp"
#include "Shader.hpp"

class GameState : public ApplicationState {
	public:
		GameState();
		~GameState();
		
		void update();
		
		void draw();
		
	private:
		GLuint m_vbo;
		
		Shader m_shader;
};

#endif // GAMESTATE_HPP_
