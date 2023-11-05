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
#ifndef VERTEXBUFFER_HPP_
#define VERTEXBUFFER_HPP_

#include <bgfx/bgfx.h>

#include "IntTypes.hpp"
#include "NonCopyable.hpp"

class VertexBuffer : public NonCopyable {
	public:
		VertexBuffer();
		VertexBuffer(VertexBuffer &&);
		~VertexBuffer();

		VertexBuffer &operator=(VertexBuffer &&);

		void init(const void *data, u32 size, bool isDynamic = false);
		void update(const void *data, u32 size, u32 offset = 0) const;
		void free();

		void enable() const;
		void enable(u32 startVertex, u32 numVertices) const;

		bool isValid() const;

		u16 handle() const { return (!m_isDynamic) ? m_staticHandle.idx : m_dynamicHandle.idx; }

		bgfx::VertexLayout &layout() { return m_layout; }

		void setupDefaultLayout();

	private:
		bgfx::VertexBufferHandle m_staticHandle = BGFX_INVALID_HANDLE;
		bgfx::DynamicVertexBufferHandle m_dynamicHandle = BGFX_INVALID_HANDLE;

		bgfx::VertexLayout m_layout;

		bool m_isDynamic = false;

		u32 m_size = 0;
};

#endif // VERTEXBUFFER_HPP_
