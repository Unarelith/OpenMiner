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
#include <gk/gl/GLCheck.hpp>
#include <gk/gl/Vertex.hpp>
#include <gk/core/GameClock.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "BlockCursor.hpp"
#include "BlockCursorRaycast.hpp"
#include "ClientCommandHandler.hpp"
#include "ClientPlayer.hpp"
#include "Config.hpp"
#include "GameKey.hpp"
#include "Hotbar.hpp"
#include "Registry.hpp"

static float cubeCoords[6 * 4 * 3] = {
	// Right
	1, 1, 1,
	1, 1, 0,
	1, 0, 0,
	1, 0, 1,

	// Top
	1, 1, 1,
	0, 1, 1,
	0, 1, 0,
	1, 1, 0,

	// Back
	1, 1, 1,
	1, 0, 1,
	0, 0, 1,
	0, 1, 1,

	// Left
	0, 1, 1,
	0, 1, 0,
	0, 0, 0,
	0, 0, 1,

	// Bottom
	1, 0, 1,
	0, 0, 1,
	0, 0, 0,
	1, 0, 0,

	// Front
	0, 0, 0,
	0, 1, 0,
	1, 1, 0,
	1, 0, 0,
};

void BlockCursor::onEvent(const SDL_Event &event, const Hotbar &hotbar) {
	if (event.type == SDL_MOUSEBUTTONDOWN && m_selectedBlock.w != -1) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			m_animationStart = gk::GameClock::getTicks();
			m_currentTool = &m_player.inventory().getStack(hotbar.cursorPos(), 0);
		}
		else if (event.button.button == SDL_BUTTON_RIGHT) {
			if (m_animationStart != 0)
				m_animationStart = 0;

			u32 blockId = m_world.getBlock(m_selectedBlock.x, m_selectedBlock.y, m_selectedBlock.z);
			const Block &block = Registry::getInstance().getBlock(blockId);
			const Item &item = Registry::getInstance().getItem(hotbar.currentItem());

			bool blockActivationSent = false;
			if (block.id() && block.canBeActivated() && !gk::GamePad::isKeyPressed(GameKey::Sneak)) {
				m_client.sendBlockActivated(m_selectedBlock);
				blockActivationSent = true;
			}

			if (block.id() && !blockActivationSent && hotbar.currentItem() && item.isBlock()) {
				s8 face = m_selectedBlock.w;

				s32 x = m_selectedBlock.x;
				s32 y = m_selectedBlock.y;
				s32 z = m_selectedBlock.z;

				if(face == 0) x++;
				if(face == 3) x--;
				if(face == 1) y++;
				if(face == 4) y--;
				if(face == 2) z++;
				if(face == 5) z--;

				u32 blockId = m_world.getBlock(x, y, z);
				const Block &block = Registry::getInstance().getBlock(blockId);
				if (!blockId || block.drawType() == BlockDrawType::Liquid) {
					m_world.setBlock(x, y, z, hotbar.currentItem());

					m_client.sendPlayerPlaceBlock(x, y, z, hotbar.currentItem());

					const ItemStack &currentStack = m_player.inventory().getStack(hotbar.cursorPos(), 0);
					m_player.inventory().setStack(hotbar.cursorPos(), 0, currentStack.amount() > 1 ? currentStack.item().name() : "", currentStack.amount() - 1);

					m_client.sendPlayerInvUpdate();
				}
			}
		}
	}
	else if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			m_animationStart = 0;
		}
	}
}

void BlockCursor::update(const Hotbar &hotbar) {
	bool selectedBlockChanged = false;
	glm::vec4 selectedBlock = findSelectedBlock();
	if (selectedBlock.x != m_selectedBlock.x || selectedBlock.y != m_selectedBlock.y || selectedBlock.z != m_selectedBlock.z)
		selectedBlockChanged = true;

	m_selectedBlock = selectedBlock;

	m_currentBlock = &Registry::getInstance().getBlock(m_world.getBlock(m_selectedBlock.x, m_selectedBlock.y, m_selectedBlock.z));
	// if (block.boundingBox().intersects(FloatBox{m_selectedBlock.x, m_selectedBlock.y, m_selectedBlock.z, 1, 1, 1})) {
	// 	selectedBlockChanged = false;
	// 	m_selectedBlock.w = -1;
	// }

	if (selectedBlockChanged)
		m_animationStart = (m_animationStart) ? gk::GameClock::getTicks() : 0;

	const ItemStack &currentStack = m_player.inventory().getStack(hotbar.cursorPos(), 0);
	float timeToBreak = 0;
	if (m_animationStart) {
		if (m_currentTool->item().id() != currentStack.item().id()) {
			m_animationStart = gk::GameClock::getTicks();
			m_currentTool = &currentStack;
		}
		else {
			timeToBreak = m_currentBlock->timeToBreak(currentStack.item().harvestCapability(), currentStack.item().miningSpeed());

			if (gk::GameClock::getTicks() > m_animationStart + timeToBreak * 1000) {
				ItemStack itemDrop = m_currentBlock->getItemDrop();
				m_player.inventory().addStack(itemDrop.item().name(), itemDrop.amount());
				m_world.setBlock(m_selectedBlock.x, m_selectedBlock.y, m_selectedBlock.z, 0);
				m_animationStart = gk::GameClock::getTicks();

				m_client.sendPlayerDigBlock(m_selectedBlock);
				m_client.sendPlayerInvUpdate();
			}
		}
	}

	if (m_selectedBlock.w != -1)
		updateVertexBuffer(*m_currentBlock);
	else
		m_currentBlock = nullptr;

	if (m_animationStart && m_currentBlock)
		updateAnimationVertexBuffer(*m_currentBlock, (gk::GameClock::getTicks() - m_animationStart) / (timeToBreak * 100));
}

void BlockCursor::updateVertexBuffer(const Block &block) {
	gk::Vertex vertices[24];
	for (u8 i = 0 ; i < 24 ; ++i) {
		vertices[i].coord3d[0] = cubeCoords[i * 3]     * block.boundingBox().width  + block.boundingBox().x;
		vertices[i].coord3d[1] = cubeCoords[i * 3 + 1] * block.boundingBox().height + block.boundingBox().y;
		vertices[i].coord3d[2] = cubeCoords[i * 3 + 2] * block.boundingBox().depth  + block.boundingBox().z;
		vertices[i].coord3d[3] = -1;
	}

	gk::VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	gk::VertexBuffer::bind(nullptr);
}

void BlockCursor::updateAnimationVertexBuffer(const Block &block, int animationPos) {
	gk::Vertex vertices[24];
	for (u8 i = 0 ; i < 24 ; ++i) {
		vertices[i].coord3d[0] = cubeCoords[i * 3]     * block.boundingBox().width  + block.boundingBox().x;
		vertices[i].coord3d[1] = cubeCoords[i * 3 + 1] * block.boundingBox().height + block.boundingBox().y;
		vertices[i].coord3d[2] = cubeCoords[i * 3 + 2] * block.boundingBox().depth  + block.boundingBox().z;
		vertices[i].coord3d[3] = -1;
	}

	GLfloat color[4] = {1, 1, 1, 0.5};
	for (int i = 0 ; i < 24 ; ++i)
		memcpy(vertices[i].color, color, 4 * sizeof(GLfloat));

	if (animationPos != -1) {
		glm::vec4 blockTexCoords{0.1f * animationPos, 0.0, 0.1f + 0.1f * animationPos, 1.0};
		float faceTexCoords[2 * 4] = {
			blockTexCoords.x, blockTexCoords.w,
			blockTexCoords.z, blockTexCoords.w,
			blockTexCoords.z, blockTexCoords.y,
			blockTexCoords.x, blockTexCoords.y
		};

		for (u8 i = 0 ; i < 6 ; ++i) {
			for(u8 j = 0 ; j < 4 ; j++) {
				vertices[j + i * 4].texCoord[0] = faceTexCoords[j * 2];
				vertices[j + i * 4].texCoord[1] = faceTexCoords[j * 2 + 1];
			}
		}
	}

	gk::VertexBuffer::bind(&m_animationVBO);
	m_animationVBO.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	gk::VertexBuffer::bind(nullptr);
}

void BlockCursor::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	if (m_selectedBlock.w == -1) return;

	glCheck(glDisable(GL_POLYGON_OFFSET_FILL));
	glCheck(glDisable(GL_CULL_FACE));

	states.transform.translate({m_selectedBlock.x, m_selectedBlock.y, m_selectedBlock.z});

	target.draw(m_vbo, GL_LINES, 0, 24, states);

	if (m_animationStart > 0) {
		glCheck(glBlendFunc(GL_DST_COLOR, GL_ZERO));
		states.texture = &gk::ResourceHandler::getInstance().get<gk::Texture>("texture-block_destroy"); // FIXME
		target.draw(m_animationVBO, GL_QUADS, 0, 24, states);
		glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glEnable(GL_POLYGON_OFFSET_FILL));
}

glm::vec4 BlockCursor::findSelectedBlock() const {
	glm::dvec3 lookAt{m_player.pointTargetedX() - m_player.camera().getPosition().x,
	                  m_player.pointTargetedY() - m_player.camera().getPosition().y,
	                  m_player.pointTargetedZ() - m_player.camera().getPosition().z};

	glm::dvec3 position{m_player.camera().getPosition().x,
	                    m_player.camera().getPosition().y,
	                    m_player.camera().getPosition().z};

	// Ray casting algorithm to find out which block we are looking at
	const double maxReach = 10.;
	double bestDepth;
	int_fast8_t bestFace = -1;
	int_fast32_t bestX = 0, bestY = 0, bestZ = 0;

	glm::dvec3 lookAtN = glm::normalize(lookAt);

	BlockCursorRaycast::rayCastToAxis(AXIS_X, position, lookAtN, maxReach, bestX, bestY, bestZ, bestFace, bestDepth, m_world);
	BlockCursorRaycast::rayCastToAxis(AXIS_Y, position, lookAtN, maxReach, bestX, bestY, bestZ, bestFace, bestDepth, m_world);
	BlockCursorRaycast::rayCastToAxis(AXIS_Z, position, lookAtN, maxReach, bestX, bestY, bestZ, bestFace, bestDepth, m_world);

	return {bestX, bestY, bestZ, bestFace};
}

