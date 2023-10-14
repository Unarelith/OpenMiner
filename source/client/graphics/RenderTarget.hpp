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
#include <gk/gl/OpenGL.hpp>
#include <gk/gl/View.hpp>

#include "RenderStates.hpp"

class Drawable;
class VertexBuffer;

class RenderTarget {
	public:
		virtual ~RenderTarget() = default;

		void draw(const Drawable &drawable, const RenderStates &states = RenderStates::Default);
		void draw(const VertexBuffer &vertexBuffer, GLenum mode, GLint firstVertex, GLsizei vertexCount, const RenderStates &states = RenderStates::Default);
		void drawElements(const VertexBuffer &vertexBuffer, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, const RenderStates &states = RenderStates::Default);

		void drawArrays(GLenum mode, GLint firstVertex, GLsizei vertexCount);

		void beginDrawing(const RenderStates &states);

		virtual gk::Vector2u getSize() const = 0;

		virtual const gk::View &getDefaultView() const = 0;

		gk::View *getView() { return m_view; }
		const gk::View *getView() const { return m_view; }
		// FIXME: const_cast shouldn't be used here but it's required for OpenMiner
		void setView(const gk::View &view) { m_view = const_cast<gk::View*>(&view); m_viewChanged = true; }
		void disableView() { m_view = nullptr; }

	private:
		gk::IntRect getViewport(const gk::View &view) const;

		void applyCurrentView(const RenderStates &states);

		bool m_viewChanged = false;
		gk::View *m_view = nullptr;

		gk::IntRect m_previousViewport;
};

#endif // RENDERTARGET_HPP_
