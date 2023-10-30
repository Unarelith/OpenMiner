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
#include <gk/core/Exception.hpp>

#include "Color.hpp"
#include "ShaderUniform.hpp"

ShaderUniform::ShaderUniform(ShaderUniform &&shaderUniform) {
	m_handle = shaderUniform.m_handle;
	shaderUniform.m_handle.idx = bgfx::kInvalidHandle;
}

ShaderUniform::~ShaderUniform() {
	free();
}

ShaderUniform &ShaderUniform::operator=(ShaderUniform &&shaderUniform) {
	m_handle = shaderUniform.m_handle;
	shaderUniform.m_handle.idx = bgfx::kInvalidHandle;

	return *this;
}

void ShaderUniform::init(const char *uniformName, bgfx::UniformType::Enum type, uint16_t num) {
	if (bgfx::isValid(m_handle))
		throw EXCEPTION("Trying to reuse an uniform named", (m_name ? m_name : "?"), "with name", (uniformName ? uniformName : "?"));

	m_name = uniformName;
	m_handle = bgfx::createUniform(m_name, type, num);
}

void ShaderUniform::free() {
	if (bgfx::isValid(m_handle))
		bgfx::destroy(m_handle);
}

void ShaderUniform::setValue(float x, float y, float z, float w) const {
	float value[4] = {x, y, z, w};
	bgfx::setUniform(m_handle, value);
}

void ShaderUniform::setValue(const Color &color, bool needsRounding) const {
	if (!needsRounding) {
		float value[4] = {color.r, color.g, color.b, color.a};
		bgfx::setUniform(m_handle, value);
	}
	else {
		float value[4] = {
			color.r255() / 255.f,
			color.g255() / 255.f,
			color.b255() / 255.f,
			color.a255() / 255.f
		};

		bgfx::setUniform(m_handle, value);
	}
}
