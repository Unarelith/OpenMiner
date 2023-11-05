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
#include <fstream>

#include "Exception.hpp"
#include "Shader.hpp"

Shader::Shader(const std::string &name) {
	loadFromFile(name);
}

Shader::~Shader() {
	if (bgfx::isValid(m_program)) {
		bgfx::destroy(m_program);
		m_program.idx = bgfx::kInvalidHandle;
	}
}

void Shader::loadFromFile(const std::string &name) {
	std::string suffix;
	if (bgfx::getRendererType() == bgfx::RendererType::OpenGL)
		suffix = "120";
	else if (bgfx::getRendererType() == bgfx::RendererType::Vulkan)
		suffix = "spirv";
	else if (bgfx::getRendererType() == bgfx::RendererType::Direct3D11
		  || bgfx::getRendererType() == bgfx::RendererType::Direct3D12)
		suffix = "s_5_0";

	m_vertexShader = loadShader("resources/shaders/" + name + "." + suffix + ".vs.bin");
	m_fragmentShader = loadShader("resources/shaders/" + name + "." + suffix + ".fs.bin");

	m_program = bgfx::createProgram(m_vertexShader, m_fragmentShader, true);
}

bgfx::ShaderHandle Shader::loadShader(const std::string &path) {
	std::ifstream file(path, std::ios::binary);
	file.seekg(0, std::ios::end);
	u32 fileSize = (u32)file.tellg();
	file.seekg(0, std::ios::beg);

	std::string str;
	str.reserve(fileSize);
	str.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return bgfx::createShader(bgfx::copy(str.data(), fileSize));
}

bool Shader::isValid() const {
	return bgfx::isValid(m_program);
}

