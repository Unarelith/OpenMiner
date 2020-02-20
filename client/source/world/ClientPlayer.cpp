/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <glm/gtc/matrix_transform.hpp>

#include <gk/core/input/GamePad.hpp>
#include <gk/core/Mouse.hpp>

#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "GameKey.hpp"
#include "Registry.hpp"

ClientPlayer *ClientPlayer::s_instance = nullptr;

ClientPlayer::ClientPlayer(gk::Camera &camera) : m_camera(camera) {
	// FIXME: Warning: Duplicated in ServerCommandHandler.hpp
	m_x = 12;
	m_y = 12;
	m_z = 20;

	m_angleH = -90.0;
	m_angleV = 0.01;

	m_camera.setPosition(m_x, m_y, m_z - 0.1);
	m_camera.setTargetPosition(pointTargetedX(), pointTargetedY(), pointTargetedZ());
}

void ClientPlayer::turnH(float angle) {
	m_angleH += angle;

	while(m_angleH >= 180.0f) {
		m_angleH -= 360.0f;
	}
	while(m_angleH < -180.0f) {
		m_angleH += 360.0f;
	}

	m_camera.setTargetPosition(pointTargetedX(), pointTargetedY(), pointTargetedZ());
}

void ClientPlayer::turnV(float angle) {
	m_angleV += angle;

	if(89.9f < m_angleV) {
		m_angleV = 89.9f;
	}
	else if(-89.9f > m_angleV) {
		m_angleV = -89.9f;
	}

	m_camera.setTargetPosition(pointTargetedX(), pointTargetedY(), pointTargetedZ());
}

void ClientPlayer::move(float direction) {
	direction += m_angleH;

	m_velocity.x = 0.04f * cosf(direction * RADIANS_PER_DEGREES);
	m_velocity.y = 0.04f * sinf(direction * RADIANS_PER_DEGREES);
}

void ClientPlayer::processInputs() {
	if(gk::GamePad::isKeyPressed(GameKey::Jump) && !m_isJumping) {
		m_isJumping = true;
		m_velocity.z = m_jumpSpeed;
	}

	if(gk::GamePad::isKeyPressed(GameKey::Fly)) {
		m_velocity.z = 0.1;
	}

	if(gk::GamePad::isKeyPressed(GameKey::Sneak)) {
		m_velocity.z = -0.1;
	}

	if(gk::GamePad::isKeyPressed(GameKey::Up))        move(0.0f);
	else if(gk::GamePad::isKeyPressed(GameKey::Down)) move(180.0f);

	if(gk::GamePad::isKeyPressed(GameKey::Left))       move(90.0f);
	else if(gk::GamePad::isKeyPressed(GameKey::Right)) move(-90.0f);

	if (gk::GamePad::isKeyPressed(GameKey::Left)  && gk::GamePad::isKeyPressed(GameKey::Up))   move(45.0f);
	if (gk::GamePad::isKeyPressed(GameKey::Right) && gk::GamePad::isKeyPressed(GameKey::Up))   move(-45.0f);
	if (gk::GamePad::isKeyPressed(GameKey::Left)  && gk::GamePad::isKeyPressed(GameKey::Down)) move(135.0f);
	if (gk::GamePad::isKeyPressed(GameKey::Right) && gk::GamePad::isKeyPressed(GameKey::Down)) move(-135.0f);

	if (gk::GamePad::isKeyPressed(GameKey::Sprint)) {
		m_velocity.x *= 1.5;
		m_velocity.y *= 1.5;
	}
}

void ClientPlayer::updatePosition(const ClientWorld &world) {
	ClientChunk *chunk = (ClientChunk *)world.getChunkAtBlockPos(m_x, m_y, m_z - 0.2);
	if (!Config::isFlyModeEnabled && chunk && chunk->isInitialized()) {
		m_velocity.z -= m_gravity; // Gravity

		if (m_velocity.z < -m_jumpSpeed) // Jump max accel
			m_velocity.z = -m_jumpSpeed;
	}

	if (!Config::isNoClipEnabled)
		checkCollisions(world);

	if (!Config::isFlyModeEnabled && m_velocity.z != 0) {
		m_velocity.x *= 0.75f;
		m_velocity.y *= 0.75f;
	}

	m_x += m_velocity.x;
	m_y += m_velocity.y;
	m_z += m_velocity.z;

	m_camera.setPosition(m_x, m_y, m_z - 0.1);
	Player::setPosition(m_x, m_y, m_z);

	if (m_velocity.x != 0 || m_velocity.y != 0 || m_velocity.z != 0)
		m_camera.setTargetPosition(pointTargetedX(), pointTargetedY(), pointTargetedZ());

	m_velocity.x = 0;
	m_velocity.y = 0;

	if (Config::isFlyModeEnabled)
		m_velocity.z = 0;
}

void ClientPlayer::checkCollisions(const ClientWorld &world) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(__ANDROID__)
	const float PLAYER_HEIGHT = 1.8;
	float eyeheight = m_z + PLAYER_HEIGHT - 1.4;
	// testPoint(world, glm::vec3(m_x, m_y, m_z), m_velocity);
	testPoint(world, m_x - 0.2, m_y - 0.2, eyeheight - PLAYER_HEIGHT - 0.4, m_velocity);
	testPoint(world, m_x + 0.2, m_y - 0.2, eyeheight - PLAYER_HEIGHT - 0.4, m_velocity);
	testPoint(world, m_x - 0.2, m_y + 0.2, eyeheight - PLAYER_HEIGHT - 0.4, m_velocity);
	testPoint(world, m_x + 0.2, m_y + 0.2, eyeheight - PLAYER_HEIGHT - 0.4, m_velocity);
	testPoint(world, m_x - 0.2, m_y - 0.2, eyeheight - 0.4, m_velocity);
	testPoint(world, m_x + 0.2, m_y - 0.2, eyeheight - 0.4, m_velocity);
	testPoint(world, m_x - 0.2, m_y + 0.2, eyeheight - 0.4, m_velocity);
	testPoint(world, m_x + 0.2, m_y + 0.2, eyeheight - 0.4, m_velocity);
#else
	for (float z = m_z + m_hitbox.z ; z <= m_z + m_hitbox.z + m_hitbox.sizeZ + 0.1f ; z += 0.9f) {
		for (float y = m_y + m_hitbox.y ; y <= m_y + m_hitbox.y + m_hitbox.sizeY + 0.1f ; y += 0.2f) {
			for (float x = m_x + m_hitbox.x ; x <= m_x + m_hitbox.x + m_hitbox.sizeX + 0.1f ; x += 0.2f) {
				if (x == m_x + m_hitbox.x || x == m_x + m_hitbox.x + m_hitbox.sizeX
				 || y == m_y + m_hitbox.y || y == m_y + m_hitbox.y + m_hitbox.sizeY
				 || z == m_z + m_hitbox.z || z == m_z + m_hitbox.z + m_hitbox.sizeZ)
					testPoint(world, x, y, z, m_velocity);
			}
		}
	}
#endif
}

bool passable(const ClientWorld &world, float x, float y, float z) {
	u32 blockID = world.getBlock(x, y, z);
	const Block &block = Registry::getInstance().getBlock(blockID);
	return !blockID || block.drawType() == BlockDrawType::Liquid || block.drawType() == BlockDrawType::XShape;
}

void ClientPlayer::testPoint(const ClientWorld &world, float x, float y, float z, glm::vec3 &speed) {
	// FIXME: Temporary fix, find the real problem!!!
	if (x < 1) --x;
	if (y < 1) --y;
	if (z < 1) --z;

	if(!passable(world, x + speed.x, y, z)) speed.x = 0;
	if(!passable(world, x, y + speed.y, z)) speed.y = 0;
	if(!passable(world, x, y, z + speed.z)) {
		if(speed.z < 0 && m_isJumping) m_isJumping = false;
		speed.z = 0;
	}
}

