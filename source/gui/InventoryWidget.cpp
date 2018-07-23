/*
 * =====================================================================================
 *
 *       Filename:  InventoryWidget.cpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 01:09:20
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "InventoryWidget.hpp"

void InventoryWidget::init(Inventory &inventory, unsigned int offset, unsigned int size) {
	m_itemWidgets.clear();

	// for (u16 y = 0 ; y < inventory.height() ; ++y) {
	// 	for (u16 x = 0 ; x < inventory.width() ; ++x) {
	for (u16 i = 0 ; i < (size > 0 ? size : inventory.width() * inventory.height()) ; ++i) {
		ItemWidget &widget = m_itemWidgets.emplace_back(inventory, (i + offset) % inventory.width(), (i + offset) / inventory.width(), this);
		widget.update();
		widget.setPosition((i % inventory.width()) * 18, (i / inventory.width()) * 18, 0);
	}

	m_width = inventory.width() * 18;
	m_height = inventory.height() * 18;

	m_inventoryWidth = inventory.width();
	m_inventoryHeight = inventory.height();
}

void InventoryWidget::onMouseEvent(const S_Event &event, MouseItemWidget &mouseItemWidget, bool isReadOnly) {
#ifdef USE_SDL
	if (event.type == SDL_MOUSEMOTION) {
#elif defined USE_SFML
	if (event.type == sf::Event::MouseMoved) {
#endif // USE_SDL, USE_SFML
		m_currentItemWidget = nullptr;
		for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
#ifdef USE_SDL
			if (m_itemWidgets[i].isPointInWidget(event.motion.x, event.motion.y)) {
#elif defined USE_SFML
			if (m_itemWidgets[i].isPointInWidget(event.mouseMove.x, event.mouseMove.y)) {
#endif // USE_SDL, USE_SFML
				m_currentItemWidget = &m_itemWidgets[i];

				m_selectedItemBackground.setPosition(1 + (i % m_inventoryWidth) * 18, 1 + (i / m_inventoryWidth) * 18, 0);
			}
		}
	}
#ifdef USE_SDL
	else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && m_currentItemWidget) {
#elif defined USE_SFML
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && m_currentItemWidget) {
#endif // USE_SDL, USE_SFML
		mouseItemWidget.swapItems(*m_currentItemWidget, isReadOnly);
	}
#ifdef USE_SDL
	else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT && m_currentItemWidget) {
#elif defined USE_SFML
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && m_currentItemWidget) {
#endif // USE_SDL, USE_SFML
		if (!isReadOnly) {
			mouseItemWidget.putItem(*m_currentItemWidget);
		}
	}
}

void InventoryWidget::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();

	for (std::size_t i = 0 ; i < m_itemWidgets.size() ; ++i) {
		target.draw(m_itemWidgets[i], states);
	}

	if (m_currentItemWidget)
		target.draw(m_selectedItemBackground, states);
}

