/*
 * =====================================================================================
 *
 *       Filename:  BlockInfoWidget.hpp
 *
 *    Description:
 *
 *        Created:  06/07/2018 14:32:50
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef BLOCKINFOWIDGET_HPP_
#define BLOCKINFOWIDGET_HPP_

#include "ItemWidget.hpp"

class BlockInfoWidget : public Widget {
	public:
		BlockInfoWidget(Widget *parent = nullptr);

		void update() override;

		const Block *currentBlock() const { return m_currentBlock; }
		void setCurrentBlock(const Block *block);

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		Inventory m_inventory{1, 1};
		ItemWidget m_itemWidget{m_inventory, 0, 0, this};

		Sprite m_background{"texture-toasts", 160, 32};
		Text m_text;

		bool m_isVisible = false;

		const Block *m_currentBlock = nullptr;
};

#endif // BLOCKINFOWIDGET_HPP_
