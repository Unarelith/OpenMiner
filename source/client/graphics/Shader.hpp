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
#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <string>

#include <bgfx/bgfx.h>

class Transform;

class Shader {
	public:
		Shader() = default;
		Shader(const std::string &name);
		~Shader();

		void loadFromFile(const std::string &name);

		bgfx::ShaderHandle loadShader(const std::string &path);

		bool isValid() const;

		const bgfx::ProgramHandle &program() const { return m_program; }

	private:
		bgfx::ShaderHandle m_vertexShader = BGFX_INVALID_HANDLE;
		bgfx::ShaderHandle m_fragmentShader = BGFX_INVALID_HANDLE;

		bgfx::ProgramHandle m_program = BGFX_INVALID_HANDLE;
};

#endif // SHADER_HPP_
