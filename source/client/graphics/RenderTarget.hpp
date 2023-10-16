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
#ifndef RENDERTARGET_HPP_
#define RENDERTARGET_HPP_

#include <gk/core/Rect.hpp>

#include "IndexBuffer.hpp"
#include "RenderStates.hpp"
#include "View.hpp"

class Drawable;
class IndexBuffer;
class VertexBuffer;

class RenderTarget {
	public:
		virtual ~RenderTarget() = default;

		void init();
		void free();

		void draw(const Drawable &drawable, const RenderStates &states = RenderStates::Default);

		void draw(const VertexBuffer &vertexBuffer, u32 firstVertex, u32 vertexCount, const RenderStates &states = RenderStates::Default);
		void drawElements(const VertexBuffer &vertexBuffer, const IndexBuffer &indexBuffer, u32 count, const RenderStates &states = RenderStates::Default);

		void beginDrawing(const RenderStates &states);
		void setBgfxState(const RenderStates &states);

		virtual gk::Vector2u getSize() const = 0;

		virtual const View &getDefaultView() const = 0;

		View *getView() { return m_view; }
		const View *getView() const { return m_view; }
		void setView(const View &view) { m_view = const_cast<View*>(&view); m_viewChanged = true; }
		void disableView() { m_view = nullptr; }

		const IndexBuffer &defaultIndexBuffer() const { return m_defaultIndexBuffer; }

	private:
		void initDefaultIndexBuffer();

		gk::IntRect getViewport(const View &view) const;

		void applyCurrentView(const RenderStates &states);

		bool m_viewChanged = false;
		View *m_view = nullptr;

		bgfx::UniformHandle m_samplerUniform = BGFX_INVALID_HANDLE;

		IndexBuffer m_defaultIndexBuffer;
};

#endif // RENDERTARGET_HPP_
