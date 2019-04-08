/*
 * =====================================================================================
 *
 *       Filename:  FurnaceWidget.hpp
 *
 *    Description:
 *
 *        Created:  28/06/2018 10:02:21
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef FURNACEWIDGET_HPP_
#define FURNACEWIDGET_HPP_

#include "BlockData.hpp"
#include "InventoryWidget.hpp"

class FurnaceWidget : public Widget {
	public:
		FurnaceWidget(ClientCommandHandler &client, MouseItemWidget &mouseItemWidget, Inventory &playerInventory, BlockData &blockData, Widget *parent = nullptr);

		void onEvent(const SDL_Event &event) override;

		void update() override;

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		ClientCommandHandler &m_client;

		gk::Image m_background;

		Inventory &m_playerInventory;
		InventoryWidget m_playerInventoryWidget{m_client, this};
		InventoryWidget m_hotbarInventoryWidget{m_client, this};

		InventoryWidget m_inputInventoryWidget{m_client, this};
		InventoryWidget m_outputInventoryWidget{m_client, this};
		InventoryWidget m_fuelInventoryWidget{m_client, this};

		BlockData &m_blockData;

		gk::Image m_burnImage{"texture-furnace"};
		gk::Image m_progressImage{"texture-furnace"};

		MouseItemWidget &m_mouseItemWidget;
};

#endif // FURNACEWIDGET_HPP_
