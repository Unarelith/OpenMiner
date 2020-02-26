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
#include <gk/core/Mouse.hpp>

#include "ClientPlayer.hpp"
#include "ClientWorld.hpp"
#include "GameKey.hpp"
#include "Registry.hpp"

ClientPlayer *ClientPlayer::s_instance = nullptr;

ClientPlayer::ClientPlayer(gk::Camera &camera) : m_camera(camera) {
	// FIXME: Warning: Duplicated in ServerCommandHandler.hpp
	m_x = 14;
	m_y = 14;
	m_z = 18;

	m_viewAngleH = -90.;
	m_viewAngleV = 0.01;
	m_viewAngleRoll = 0.;

	updateDir();

	m_camera.setDPosition(m_x, m_y, m_z - 0.1);
}

void ClientPlayer::turnH(double angle) {
	m_viewAngleH = (m_viewAngleH + angle >= -180. && m_viewAngleH + angle < 0.) ?
		m_viewAngleH + angle : fmod(m_viewAngleH + angle, 360.);

	if (m_viewAngleH >= 180.) m_viewAngleH -= 360.;

	updateDir();
}

void ClientPlayer::turnViewV(double angle) {
	m_viewAngleV = std::max(std::min(m_viewAngleV + angle, 90.), -90.);

	updateDir();
}

void ClientPlayer::updateDir() {
	double ch = cos(m_viewAngleH * RADIANS_PER_DEGREES);
	double sh = sin(m_viewAngleH * RADIANS_PER_DEGREES);
	double cv = cos(m_viewAngleV * RADIANS_PER_DEGREES);
	double sv = sin(m_viewAngleV * RADIANS_PER_DEGREES);
	double cr = cos(m_viewAngleRoll * RADIANS_PER_DEGREES);
	double sr = sin(m_viewAngleRoll * RADIANS_PER_DEGREES);

	m_forwardDir = gk::Vector3d{ch * cv, sh * cv, sv};
	m_camera.setDirection(m_forwardDir);
	m_camera.setUpVector(gk::Vector3d{sh * sr - ch * sv * cr, -ch * sr - sh * sv * cr, cv * cr});
}

void ClientPlayer::move(double direction) {
	direction += m_viewAngleH;

	m_velocity.x = 0.04 * cos(direction * RADIANS_PER_DEGREES);
	m_velocity.y = 0.04 * sin(direction * RADIANS_PER_DEGREES);
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

	if(gk::GamePad::isKeyPressed(GameKey::Up))        move(0.0);
	else if(gk::GamePad::isKeyPressed(GameKey::Down)) move(180.0);

	if(gk::GamePad::isKeyPressed(GameKey::Left))       move(90.0);
	else if(gk::GamePad::isKeyPressed(GameKey::Right)) move(-90.0);

	if (gk::GamePad::isKeyPressed(GameKey::Left)  && gk::GamePad::isKeyPressed(GameKey::Up))   move(45.0);
	if (gk::GamePad::isKeyPressed(GameKey::Right) && gk::GamePad::isKeyPressed(GameKey::Up))   move(-45.0);
	if (gk::GamePad::isKeyPressed(GameKey::Left)  && gk::GamePad::isKeyPressed(GameKey::Down)) move(135.0);
	if (gk::GamePad::isKeyPressed(GameKey::Right) && gk::GamePad::isKeyPressed(GameKey::Down)) move(-135.0);

	if (gk::GamePad::isKeyPressed(GameKey::Sprint)) {
		m_velocity.x *= 1.5;
		m_velocity.y *= 1.5;
	}
}

void ClientPlayer::updatePosition(const ClientWorld &world) {
	ClientChunk *chunk = (ClientChunk *)world.getChunkAtBlockPos(m_x, m_y, m_z);
	if (!Config::isFlyModeEnabled && chunk && chunk->isInitialized()) {
		m_velocity.z -= m_gravity; // Gravity

		if (m_velocity.z < -m_jumpSpeed) // Jump max accel
			m_velocity.z = -m_jumpSpeed;
	}

	if (!Config::isNoClipEnabled)
		checkCollisions(world);

	if (!Config::isFlyModeEnabled && m_velocity.z != 0) {
		m_velocity.x *= 0.75;
		m_velocity.y *= 0.75;
	}

	setPosition(m_x + m_velocity.x, m_y + m_velocity.y, m_z + m_velocity.z);

	m_velocity.x = 0;
	m_velocity.y = 0;

	if (Config::isFlyModeEnabled)
		m_velocity.z = 0;
}

void ClientPlayer::setPosition(double x, double y, double z) {
	m_x = x;
	m_y = y;
	m_z = z;

	Player::setPosition(x, y, z);
	m_camera.setDPosition(x, y, z - 0.1);
}

void ClientPlayer::checkCollisions(const ClientWorld &world) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(__ANDROID__)
	const double PLAYER_HEIGHT = 1.8;
	double eyeheight = m_z + PLAYER_HEIGHT - 1.4;
	// testPoint(world, glm::dvec3(m_x, m_y, m_z), m_velocity);
	testPoint(world, m_x - 0.2, m_y - 0.2, eyeheight - PLAYER_HEIGHT - 0.4, m_velocity);
	testPoint(world, m_x + 0.2, m_y - 0.2, eyeheight - PLAYER_HEIGHT - 0.4, m_velocity);
	testPoint(world, m_x - 0.2, m_y + 0.2, eyeheight - PLAYER_HEIGHT - 0.4, m_velocity);
	testPoint(world, m_x + 0.2, m_y + 0.2, eyeheight - PLAYER_HEIGHT - 0.4, m_velocity);
	testPoint(world, m_x - 0.2, m_y - 0.2, eyeheight - 0.4, m_velocity);
	testPoint(world, m_x + 0.2, m_y - 0.2, eyeheight - 0.4, m_velocity);
	testPoint(world, m_x - 0.2, m_y + 0.2, eyeheight - 0.4, m_velocity);
	testPoint(world, m_x + 0.2, m_y + 0.2, eyeheight - 0.4, m_velocity);
#else
	for (double z = m_z + m_hitbox.z ; z <= m_z + (m_hitbox.z + m_hitbox.sizeZ) + 0.1 ; z += 0.9) {
		for (double y = m_y + m_hitbox.y ; y <= m_y + (m_hitbox.y + m_hitbox.sizeY) + 0.1 ; y += 0.2) {
			for (double x = m_x + m_hitbox.x ; x <= m_x + (m_hitbox.x + m_hitbox.sizeX) + 0.1 ; x += 0.2) {
				if (x == m_x + m_hitbox.x || x == m_x + (m_hitbox.x + m_hitbox.sizeX)
				 || y == m_y + m_hitbox.y || y == m_y + (m_hitbox.y + m_hitbox.sizeY)
				 || z == m_z + m_hitbox.z || z == m_z + (m_hitbox.z + m_hitbox.sizeZ))
					testPoint(world, x, y, z, m_velocity);
			}
		}
	}
#endif
}

bool passable(const ClientWorld &world, double x, double y, double z) {
	u32 blockID = world.getBlock(x, y, z);
	const Block &block = Registry::getInstance().getBlock(blockID);
	return !blockID || block.drawType() == BlockDrawType::Liquid || block.drawType() == BlockDrawType::XShape;
}

void ClientPlayer::testPoint(const ClientWorld &world, double x, double y, double z, glm::dvec3 &vel) {
	// FIXME: Temporary fix, find the real problem!!!
	if (x < 1) --x;
	if (y < 1) --y;
	if (z < 1) --z;

	if(!passable(world, x + vel.x, y, z)) vel.x = 0;
	if(!passable(world, x, y + vel.y, z)) vel.y = 0;
	if(!passable(world, x, y, z + vel.z)) {
		if(vel.z < 0 && m_isJumping) m_isJumping = false;
		vel.z = 0;
	}
}

