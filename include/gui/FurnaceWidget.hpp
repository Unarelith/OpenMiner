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
		FurnaceWidget(Inventory &playerInventory, BlockData &blockData, Widget *parent = nullptr);

		void onEvent(const S_Event &event) override;

		void update() override;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Image m_background;

		Inventory &m_playerInventory;
		InventoryWidget m_playerInventoryWidget{this};
		InventoryWidget m_hotbarInventoryWidget{this};

		InventoryWidget m_inputInventoryWidget{this};
		InventoryWidget m_outputInventoryWidget{this};
		InventoryWidget m_fuelInventoryWidget{this};

		BlockData &m_blockData;

		Image m_burnImage{"texture-furnace"};
		Image m_progressImage{"texture-furnace"};

		MouseItemWidget m_mouseItemWidget{this};
};

#endif // FURNACEWIDGET_HPP_
