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
#include <bx/math.h>

#include <gk/core/Debug.hpp>

#include "BGFXTestState.hpp"

BGFXTestState::BGFXTestState()
{
	gk::Vector2i windowSize{1600, 1050}; // FIXME

	const bx::Vec3 at  = { 0.0f, 0.0f,   0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -35.0f };
	const bx::Vec3 up  = { 0.0f, 1.0f,   0.0f };
	bx::mtxLookAt(m_viewMtx, eye, at, up);
	bx::mtxProj(m_projMtx, 60.0f, float(windowSize.x) / float(windowSize.y), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

	m_camera.setPosition(0.f, 0.f, 0.f);
	m_camera.setDirection(0.f, 0.f, -35.f);
	m_camera.setUpVector(0.f, 1.f, 0.f);

	m_camera.setFieldOfView(60.0f);
	m_camera.setAspectRatio(float(windowSize.x) / float(windowSize.y));
	m_camera.setNearClippingPlane(0.1f);
	m_camera.setFarClippingPlane(100.0f);

	m_view.setSize((float)windowSize.x, (float)windowSize.y);
	m_view.setCenter((float)windowSize.x / 2.f, (float)windowSize.y / 2.f);

	// Set view 0 default viewport.
	bgfx::setViewRect(0, 0, 0, (uint16_t)windowSize.x, (uint16_t)windowSize.y);

	m_rect.setFillColor(gk::Color::White);
	m_rect.setSize(100, 100);
	m_rect.setPosition(0, 0);

	m_shader.loadFromFile("basic");
}

void BGFXTestState::draw(RenderTarget &target, RenderStates states) const
{
	// states.projectionMatrix = m_camera.getTransform();
	// states.viewMatrix = m_camera.getViewTransform();
	// states.projectionMatrix = glm::transpose(m_camera.getTransform().getMatrix());
	// states.viewMatrix = glm::transpose(m_camera.getViewTransform().getMatrix());
	// states.projectionMatrix = glm::make_mat4(m_projMtx);
	// states.viewMatrix = glm::make_mat4(m_viewMtx);
	states.projectionMatrix = m_view.getTransform();
	states.viewMatrix = m_view.getViewTransform();

	states.shader = &m_shader;

	target.draw(m_cube, states);

	target.draw(m_rect, states);
}

//==============================================================================

struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;
};

static PosColorVertex s_cubeVertices[] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t s_cubeTriList[] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};

Cube::Cube()
{
	m_vbo.layout()
	     .begin()
	     .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
	     .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
	     .end();

	m_vbo.init(s_cubeVertices, sizeof(s_cubeVertices));
	m_ibo.init(s_cubeTriList, sizeof(s_cubeTriList));
}

void Cube::draw(RenderTarget &target, RenderStates states) const
{
	static float tick = 0.f;
	states.transform.rotateX(glm::degrees(2.234f + tick * 0.001f));
	states.transform.rotateY(glm::degrees(0.934f + tick * 0.001f));
	states.transform = glm::transpose(states.transform.getMatrix());
	tick++;

	target.drawElements(m_vbo, m_ibo, 0, 0, states);
}

