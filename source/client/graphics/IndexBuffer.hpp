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

#include <gk/utils/NonCopyable.hpp>

#include <bgfx/bgfx.h>

class IndexBuffer : public gk::NonCopyable {
	public:
		IndexBuffer();
		IndexBuffer(IndexBuffer &&);
		~IndexBuffer();

		IndexBuffer &operator=(IndexBuffer &&);

		void init(const void *data, uint32_t size, bool isDynamic = false);
		void update(const void *data, uint32_t size, uint32_t offset = 0) const;
		void free();

		void enable() const;
		void enable(uint32_t startIndex, uint32_t numIndices) const;

		bool isValid() const;

	private:
		bgfx::IndexBufferHandle m_staticHandle = BGFX_INVALID_HANDLE;
		bgfx::DynamicIndexBufferHandle m_dynamicHandle = BGFX_INVALID_HANDLE;

		bool m_isDynamic;

		const bgfx::Memory *m_data = nullptr;
};

#endif // INDEXBUFFER_HPP_
