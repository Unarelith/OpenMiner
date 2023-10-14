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

	// Set view and projection matrix for view 0.
	float view[16];
	bx::mtxLookAt(view, eye, at);

	float proj[16];
	bx::mtxProj(proj, 60.0f, float(windowSize.x) / float(windowSize.y), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
	bgfx::setViewTransform(0, view, proj);

	// Set view 0 default viewport.
	bgfx::setViewRect(0, 0, 0, (uint16_t)windowSize.x, (uint16_t)windowSize.y);
}

void BGFXTestState::draw(gk::RenderTarget &target, gk::RenderStates states) const
{
	m_cube.draw();
}

//==============================================================================

struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;

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

#include <fstream>

Cube::Cube()
{
	// Vertices
	PosColorVertex::init();
	m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), PosColorVertex::ms_layout);
	m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

	// Shader
	auto loadShader = [](const char *path) {
		std::ifstream file(path, std::ios::binary);
		file.seekg(0, std::ios::end);
		u32 fileSize = (u32)file.tellg();
		file.seekg(0, std::ios::beg);

		std::string str;
		str.reserve(fileSize);
		str.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		return bgfx::createShader(bgfx::copy(str.data(), fileSize));
	};

	bgfx::ShaderHandle shader_vs = loadShader("resources/shaders/bgfx_test.vs.bin");
	bgfx::ShaderHandle shader_fs = loadShader("resources/shaders/bgfx_test.fs.bin");
	m_program = bgfx::createProgram(shader_vs, shader_fs, true);
}

Cube::~Cube()
{
	bgfx::destroy(m_vbh);
	bgfx::destroy(m_ibh);
	bgfx::destroy(m_program);
}

void Cube::draw() const
{
	static float tick = 0.f;

	float mtx[16];
	bx::mtxIdentity(mtx);
	bx::mtxRotateXY(mtx, 2.234f + tick * 0.001f, 0.934f + tick * 0.001f);
	bgfx::setTransform(mtx);

	bgfx::setVertexBuffer(0, m_vbh);
	bgfx::setIndexBuffer(m_ibh);
	bgfx::submit(0, m_program);

	tick++;
}

