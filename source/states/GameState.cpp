/*
 * =====================================================================================
 *
 *       Filename:  GameState.cpp
 *
 *    Description:
 *
 *        Created:  15/12/2014 03:51:55
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGL.hpp"

#include "Config.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "GameState.hpp"

GameState::GameState() : m_camera(Camera::getInstance()) {
	m_shader.createProgram();

	m_shader.addShader(GL_VERTEX_SHADER, "shaders/game.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "shaders/color.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "shaders/light.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "shaders/fog.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "shaders/game.f.glsl");

	m_shader.linkProgram();

	Shader::bind(&m_shader);

	m_projectionMatrix = glm::perspective(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, DIST_NEAR, DIST_FAR);

	m_shader.setUniform("u_projectionMatrix", m_projectionMatrix);

	m_shader.setUniform("u_tex", 0);

	Shader::bind(nullptr);
}

void GameState::onEvent(const SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION) {
		if(SCREEN_WIDTH / 2 != event.motion.x || SCREEN_HEIGHT / 2 != event.motion.y) {
			m_camera.turnH(event.motion.xrel * 0.06);
			m_camera.turnV(-event.motion.yrel * 0.06);

			Mouse::resetToWindowCenter();

			m_camera.update();
		}
	}
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			m_world.setBlock(m_selectedBlock.x, m_selectedBlock.y, m_selectedBlock.z, 0);
		}
		else if (event.button.button == SDL_BUTTON_RIGHT) {
			int face = m_selectedBlock.w;

			int x = m_selectedBlock.x;
			int y = m_selectedBlock.y;
			int z = m_selectedBlock.z;

			if(face == 0) x++;
			if(face == 3) x--;
			if(face == 1) y++;
			if(face == 4) y--;
			if(face == 2) z++;
			if(face == 5) z--;

			m_world.setBlock(x, y, z, 1);
		}
	}
}

void GameState::update() {
	m_viewMatrix = m_camera.processInputs();

	m_selectedBlock = findSelectedBlock(false);
}

void GameState::draw() {
	Shader::bind(&m_shader);

	m_shader.setUniform("u_viewMatrix", m_viewMatrix);
	m_shader.setUniform("u_projectionMatrix", m_projectionMatrix);

	// m_skybox.draw(m_shader);

	m_world.draw(m_shader, m_projectionMatrix, m_viewMatrix);

	drawSelectedBlock();

	RenderStates states;
	states.shader = &m_shader;
	m_renderTarget.draw(m_crosshair, states);

	Shader::bind(nullptr);
}

void GameState::drawSelectedBlock() {
	float bx = m_selectedBlock.x;
	float by = m_selectedBlock.y;
	float bz = m_selectedBlock.z;

	/* Render a box around the block we are pointing at */
	float box[24][3] = {
		{bx + 0, by + 0, bz + 0},//, 14},
		{bx + 1, by + 0, bz + 0},//, 14},
		{bx + 0, by + 1, bz + 0},//, 14},
		{bx + 1, by + 1, bz + 0},//, 14},
		{bx + 0, by + 0, bz + 1},//, 14},
		{bx + 1, by + 0, bz + 1},//, 14},
		{bx + 0, by + 1, bz + 1},//, 14},
		{bx + 1, by + 1, bz + 1},//, 14},

		{bx + 0, by + 0, bz + 0},//, 14},
		{bx + 0, by + 1, bz + 0},//, 14},
		{bx + 1, by + 0, bz + 0},//, 14},
		{bx + 1, by + 1, bz + 0},//, 14},
		{bx + 0, by + 0, bz + 1},//, 14},
		{bx + 0, by + 1, bz + 1},//, 14},
		{bx + 1, by + 0, bz + 1},//, 14},
		{bx + 1, by + 1, bz + 1},//, 14},

		{bx + 0, by + 0, bz + 0},//, 14},
		{bx + 0, by + 0, bz + 1},//, 14},
		{bx + 1, by + 0, bz + 0},//, 14},
		{bx + 1, by + 0, bz + 1},//, 14},
		{bx + 0, by + 1, bz + 0},//, 14},
		{bx + 0, by + 1, bz + 1},//, 14},
		{bx + 1, by + 1, bz + 0},//, 14},
		{bx + 1, by + 1, bz + 1},//, 14},
	};

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_CULL_FACE);

	VertexBuffer::bind(&m_cursorVBO);

	m_cursorVBO.setData(sizeof(box), box, GL_DYNAMIC_DRAW);

	m_shader.setUniform("u_modelMatrix", glm::mat4(1));

	m_shader.enableVertexAttribArray("coord3d");
	glVertexAttribPointer(m_shader.attrib("coord3d"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, 24);
	m_shader.disableVertexAttribArray("coord3d");

	VertexBuffer::bind(nullptr);

	glEnable(GL_CULL_FACE);
	glEnable(GL_POLYGON_OFFSET_FILL);
}

// Not really GLSL fract(), but the absolute distance to the nearest integer value
// FIXME: fract also exists in glm, check if its the same thing
float GameState::fract(float value) {
	float f = value - floorf(value);
	if(f > 0.5) return 1 - f;
	else return f;
}

// FIXME
glm::vec4 GameState::findSelectedBlock(bool useDepthBuffer) {
	int mx, my, mz;
	int face = -1;

	glm::vec3 lookAt{m_camera.pointTargetedX() - m_camera.x(),
	                 m_camera.pointTargetedY() - m_camera.y(),
	                 m_camera.pointTargetedZ() - m_camera.z()};

	glm::vec3 position{m_camera.x(),
	                   m_camera.y(),
	                   m_camera.z()};

	if(useDepthBuffer) {
		// At which voxel are we looking? First, find out coords of the center pixel
		float depth;
		glReadPixels(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

		glm::vec4 viewport = glm::vec4(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glm::vec3 winCoord = glm::vec3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, depth);
		glm::vec3 objCoord = glm::unProject(winCoord, m_viewMatrix, m_projectionMatrix, viewport);

		// Find out which block it belongs to
		mx = objCoord.x;
		my = objCoord.y;
		mz = objCoord.z;

		if(objCoord.x < 0) mx--;
		if(objCoord.y < 0) my--;
		if(objCoord.z < 0) mz--;

		// Find out which face of the block we are looking at
		if(fract(objCoord.x) < fract(objCoord.y)) {
			if(fract(objCoord.x) < fract(objCoord.z)) {
				face = 0; // X
			} else {
				face = 2; // Z
			}
		} else {
			if(fract(objCoord.y) < fract(objCoord.z)) {
				face = 1; // Y
			} else {
				face = 2; // Z
			}
		}

		if(face == 0 && lookAt.x > 0) face += 3;
		if(face == 1 && lookAt.y > 0) face += 3;
		if(face == 2 && lookAt.z > 0) face += 3;
	} else {
		// Very naive ray casting algorithm to find out which block we are looking at
		glm::vec3 testPos = position;
		glm::vec3 prevPos = position;

		for(int i = 0 ; i < 100 ; i++) {
			// Advance from our current position to the direction we are looking at, in small steps
			prevPos = testPos;
			testPos += lookAt * 0.1f;

			mx = floorf(testPos.x);
			my = floorf(testPos.y);
			mz = floorf(testPos.z);

			// If we find a block that is not air, we are done
			Block *block = m_world.getBlock(mx, my, mz);
			if(block && block->id()) break;
		}

		// Find out which face of the block we are looking at

		int px = floorf(prevPos.x);
		int py = floorf(prevPos.y);
		int pz = floorf(prevPos.z);

		if(px > mx) face = 0;
		else if(px < mx) face = 3;
		else if(py > my) face = 1;
		else if(py < my) face = 4;
		else if(pz > mz) face = 2;
		else if(pz < mz) face = 5;

		// If we are looking at air, move the cursor out of sight
		Block *block = m_world.getBlock(mx, my, mz);
		if(!block || !block->id()) {
			mx = my = mz = 99999;
		}
	}

	return {mx, my, mz, face};
}

