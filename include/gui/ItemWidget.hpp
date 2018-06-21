/*
 * =====================================================================================
 *
 *       Filename:  ItemWidget.hpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 01:10:13
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef ITEMWIDGET_HPP_
#define ITEMWIDGET_HPP_

#include "Image.hpp"

class ItemWidget : public IDrawable, public Transformable {
	public:
		ItemWidget(u16 id);

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Texture m_texture;
		Image m_image;
};

#endif // ITEMWIDGET_HPP_
