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
#ifndef INDEXBUFFER_HPP_
#define INDEXBUFFER_HPP_

#include <bgfx/bgfx.h>

#include "IntTypes.hpp"
#include "NonCopyable.hpp"

class IndexBuffer : public NonCopyable {
	public:
		IndexBuffer();
		IndexBuffer(IndexBuffer &&);
		~IndexBuffer();

		IndexBuffer &operator=(IndexBuffer &&);

		void init(const void *data, u32 size, bool isDynamic = false);
		void update(const void *data, u32 size, u32 offset = 0) const;
		void free();

		void enable() const;
		void enable(u32 startIndex, u32 numIndices) const;

		bool isValid() const;

		u16 handle() const { return (!m_isDynamic) ? m_staticHandle.idx : m_dynamicHandle.idx; }

	private:
		bgfx::IndexBufferHandle m_staticHandle = BGFX_INVALID_HANDLE;
		bgfx::DynamicIndexBufferHandle m_dynamicHandle = BGFX_INVALID_HANDLE;

		bool m_isDynamic = false;
};

#endif // INDEXBUFFER_HPP_
