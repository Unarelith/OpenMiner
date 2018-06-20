/*
 * =====================================================================================
 *
 *       Filename:  WorkbenchWidget.hpp
 *
 *    Description:
 *
 *        Created:  20/06/2018 23:22:08
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef WORKBENCHWIDGET_HPP_
#define WORKBENCHWIDGET_HPP_

#include "InventoryWidget.hpp"
#include "SDLHeaders.hpp"

#include "Inventory.hpp"

class WorkbenchWidget : public IDrawable {
	public:
		WorkbenchWidget();

		void onEvent(const SDL_Event &event);

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Texture m_backgroundTexture;
		Image m_background;

		Inventory m_inventory{9, 3};
		InventoryWidget m_inventoryWidget;
};

#endif // WORKBENCHWIDGET_HPP_
