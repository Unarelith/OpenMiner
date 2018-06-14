/*
 * =====================================================================================
 *
 *       Filename:  RenderTarget.hpp
 *
 *    Description:
 *
 *        Created:  14/06/2018 19:54:56
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef RENDERTARGET_HPP_
#define RENDERTARGET_HPP_

#include "RenderStates.hpp"

class IDrawable;
class VertexBuffer;

class RenderTarget {
	public:
		void draw(const IDrawable &drawable, const RenderStates &states = RenderStates::Default);
		void draw(const VertexBuffer &vertexBuffer, const RenderStates &states = RenderStates::Default);
};

#endif // RENDERTARGET_HPP_
