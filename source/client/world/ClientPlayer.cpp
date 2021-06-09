/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <glm/gtc/matrix_transform.hpp>

#include <gk/core/input/GamePad.hpp>
#include <gk/core/GameClock.hpp>
#include <gk/core/Mouse.hpp>

#include "ClientCommandHandler.hpp"
#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "GameConfig.hpp"
#include "GameKey.hpp"
#include "Registry.hpp"

ClientPlayer *ClientPlayer::s_instance = nullptr;

ClientPlayer::ClientPlayer(gk::Camera &camera) : m_camera(camera) {
	m_cameraLocalPos = gk::Vector3f{0.f, 0.f, 1.625f};

	updateCamera();

	m_camera.setDPosition(m_x + m_cameraLocalPos.x, m_y + m_cameraLocalPos.y, m_z + m_cameraLocalPos.z);
}

void ClientPlayer::turnH(float angle) {
	m_viewAngleH = fmodf(m_viewAngleH + angle, 360.f);
	if (m_viewAngleH >= 180.f) m_viewAngleH -= 360.f;
	if (m_viewAngleH < -180.f) m_viewAngleH += 360.f;

	// updateCamera();
}

void ClientPlayer::turnViewV(float angle) {
	m_viewAngleV = std::max(std::min(m_viewAngleV + angle, 90.f), -90.f);

	// updateCamera();
}

void ClientPlayer::updateCamera() {
	float viewAngleH = m_viewAngleH;
	float viewAngleV = m_viewAngleV;
	float viewAngleRoll = m_viewAngleRoll;

	if (Config::isViewBobbingEnabled)
		applyViewBobbing(viewAngleH, viewAngleV, viewAngleRoll);

	float ch = cosf(viewAngleH * gk::DEG_TO_RADf);
	float sh = sinf(viewAngleH * gk::DEG_TO_RADf);
	float cv = cosf(viewAngleV * gk::DEG_TO_RADf);
	float sv = sinf(viewAngleV * gk::DEG_TO_RADf);
	float cr = cosf(viewAngleRoll * gk::DEG_TO_RADf);
	float sr = sinf(viewAngleRoll * gk::DEG_TO_RADf);

	m_forwardDir = gk::Vector3f{ch * cv, sh * cv, sv};
	m_camera.setDirection(m_forwardDir);
	m_camera.setUpVector(gk::Vector3f{sh * sr - ch * sv * cr, -ch * sr - sh * sv * cr, cv * cr});
}

void ClientPlayer::move(float direction) {
	direction += m_viewAngleH;

	constexpr float playerSpeed = 0.0275f;

	m_velocity.x = playerSpeed * cosf(direction * gk::DEG_TO_RADf);
	m_velocity.y = playerSpeed * sinf(direction * gk::DEG_TO_RADf);
}

void ClientPlayer::processInputs() {
	if(gk::GamePad::isKeyPressed(GameKey::Jump) && !m_isJumping) {
		m_isJumping = true;
		m_velocity.z = m_jumpSpeed;
	}

	if(gk::GamePad::isKeyPressed(GameKey::Fly)) {
		m_velocity.z = 0.1f;
	}

	if(gk::GamePad::isKeyPressed(GameKey::Sneak)) {
		m_velocity.z = -0.1f;
	}

	if(gk::GamePad::isKeyPressed(GameKey::Forward))    move(0.0f);
	else if(gk::GamePad::isKeyPressed(GameKey::Back))  move(180.0f);

	if(gk::GamePad::isKeyPressed(GameKey::Left))       move(90.0f);
	else if(gk::GamePad::isKeyPressed(GameKey::Right)) move(-90.0f);

	if (gk::GamePad::isKeyPressed(GameKey::Left)  && gk::GamePad::isKeyPressed(GameKey::Forward)) move(45.0f);
	if (gk::GamePad::isKeyPressed(GameKey::Right) && gk::GamePad::isKeyPressed(GameKey::Forward)) move(-45.0f);
	if (gk::GamePad::isKeyPressed(GameKey::Left)  && gk::GamePad::isKeyPressed(GameKey::Back))    move(135.0f);
	if (gk::GamePad::isKeyPressed(GameKey::Right) && gk::GamePad::isKeyPressed(GameKey::Back))    move(-135.0f);

	if (gk::GamePad::isKeyPressed(GameKey::Sprint)) {
		m_velocity.x *= 1.5f;
		m_velocity.y *= 1.5f;
	}
}

void ClientPlayer::updatePosition(const ClientWorld &world) {
	ClientChunk *chunk = (ClientChunk *)world.getChunkAtBlockPos((int)m_x, (int)m_y, (int)m_z);
	if (chunk && chunk->isInitialized()) {
		if (!Config::isFlyModeEnabled) {
			m_velocity.z -= chunk->dimension().gravity() * 0.001f;

			m_isJumping = true;

			// Limit max vertical speed to jump speed
			if (m_velocity.z < -m_jumpSpeed)
				m_velocity.z = -m_jumpSpeed;
		}
	}
	// Block player until the chunk loads, unless "no clip" mode is enabled
	else if (!Config::isNoClipEnabled) {
		m_velocity = gk::Vector3f::Zero;
	}

	if (!Config::isNoClipEnabled)
		checkCollisions(world);

	// Reduce velocity while in the air, unless "fly mode" is enabled
	if (!Config::isFlyModeEnabled && m_velocity.z != 0.f) {
		m_velocity.x *= 0.65f;
		m_velocity.y *= 0.65f;
	}

	// Increase velocity in fly mode
	if (Config::isFlyModeEnabled) {
		m_velocity.x *= 2.f;
		m_velocity.y *= 2.f;
	}

	updateCamera();

	setPosition(m_x + m_velocity.x, m_y + m_velocity.y, m_z + m_velocity.z);

	m_velocity.x = 0.f;
	m_velocity.y = 0.f;

	if (Config::isFlyModeEnabled)
		m_velocity.z = 0.f;

	// Checking to block at camera position to enable specific effects
	const BlockState *blockState = world.getBlockState((int)m_camera.getDPosition().x, (int)m_camera.getDPosition().y, (int)m_camera.getDPosition().z);
	if (blockState && blockState->fogDepth() != 0) {
		GameConfig::currentScreenEffect = 1;
		GameConfig::fogDepth = blockState->fogDepth();
		GameConfig::fogColor = blockState->fogColor();
	}
	else {
		GameConfig::currentScreenEffect = 0;
	}

	// Sending PlayerChunkPosUpdate if needed
	if (!m_lastChunkPos.has_value() || m_lastChunkPos.value() != getCurrentChunk()) {
		world.client().sendPlayerChunkPosUpdate();
		m_lastChunkPos = getCurrentChunk();
	}
}

void ClientPlayer::setPosition(double x, double y, double z) {
	Player::setPosition(x, y, z);

	gk::Vector3f camPos = m_cameraLocalPos;
	m_camera.setDPosition(m_x + camPos.x, m_y + camPos.y, m_z + camPos.z);
}

void ClientPlayer::checkCollisions(const ClientWorld &world) {
	gk::Vector3d corner{m_x + m_hitbox.x, m_y + m_hitbox.y, m_z + m_hitbox.z};

	constexpr int numPointsPerEdge = 3;
	constexpr int lastPoint = numPointsPerEdge - 1;

	for (int z = 0; z <= lastPoint; z++) {
		for (int y = 0; y <= lastPoint; y++) {
			for (int x = 0; x <= lastPoint; x++) {
				if (x == 0 || x == lastPoint || y == 0 || y == lastPoint || z == 0 || z == lastPoint)
					testPoint(world, corner.x + m_hitbox.sizeX * ((double)x / lastPoint),
					          corner.y + m_hitbox.sizeY * ((double)y / lastPoint),
					          corner.z + m_hitbox.sizeZ * ((double)z / lastPoint), m_velocity);
			}
		}
	}
}

bool passable(const ClientWorld &world, double x, double y, double z) {
	const BlockState *blockState = world.getBlockState((int)floor(x), (int)floor(y), (int)floor(z));
	return !blockState || !blockState->block().id() || !blockState->isCollidable();
}

void ClientPlayer::testPoint(const ClientWorld &world, double x, double y, double z, gk::Vector3f &vel) {
	if(!passable(world, x + vel.x, y, z)) vel.x = 0.f;
	if(!passable(world, x, y + vel.y, z)) vel.y = 0.f;
	if(!passable(world, x, y, z + vel.z)) {
		if(vel.z < 0.f && m_isJumping) m_isJumping = false;
		vel.z = 0.f;
	}
}

void ClientPlayer::applyViewBobbing(float &viewAngleH, float &viewAngleV, float &viewAngleRoll) {
	if (m_velocity != gk::Vector3f::Zero && m_velocity.z == 0 && !Config::isFlyModeEnabled) {
		if (!m_isMoving) {
			m_movementStartTime = gk::GameClock::getInstance().getTicks();
			m_isMoving = true;
		}
	}
	else {
		m_isMoving = false;
	}

	if (m_isMoving) {
		// float t = (float)(gk::GameClock::getInstance().getTicks());
		float t = (float)(gk::GameClock::getInstance().getTicks() - m_movementStartTime);
		viewAngleH += 0.4f * sinf(t / 150.f);
		viewAngleV += 0.4f * sinf(t / 75.f);
		viewAngleRoll += 0.3f * -sinf(t / 150.f);
	}
}

