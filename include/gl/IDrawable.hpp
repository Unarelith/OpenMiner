/*
 * =====================================================================================
 *
 *       Filename:  IDrawable.hpp
 *
 *    Description:
 *
 *        Created:  14/06/2018 19:53:57
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef IDRAWABLE_HPP_
#define IDRAWABLE_HPP_

#include "RenderTarget.hpp"

class IDrawable {
	public:
		virtual ~IDrawable() = default;

	protected:
		friend class RenderTarget;

		virtual void draw(RenderTarget &target, RenderStates states) const = 0;
};

#endif // IDRAWABLE_HPP_
