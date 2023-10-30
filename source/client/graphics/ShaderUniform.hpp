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
#ifndef SHADERUNIFORM_HPP_
#define SHADERUNIFORM_HPP_

#include <bgfx/bgfx.h>

#include "NonCopyable.hpp"

class Color;

class ShaderUniform : public NonCopyable {
	public:
		ShaderUniform() = default;
		ShaderUniform(ShaderUniform &&);
		~ShaderUniform();

		ShaderUniform &operator=(ShaderUniform &&);

		void init(const char *uniformName, bgfx::UniformType::Enum type, uint16_t num = 1);
		void free();

		void setValue(float x, float y = 0.f, float z = 0.f, float w = 0.f) const;
		void setValue(const Color &color, bool needsRounding = false) const;

		const bgfx::UniformHandle &handle() const { return m_handle; }

	private:
		const char *m_name = nullptr;

		bgfx::UniformHandle m_handle = BGFX_INVALID_HANDLE;
};

#endif // SHADERUNIFORM_HPP_
