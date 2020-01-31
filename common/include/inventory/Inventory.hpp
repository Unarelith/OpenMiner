/*
 * =====================================================================================
 *
 *       Filename:  Inventory.hpp
 *
 *    Description:
 *
 *        Created:  21/06/2018 00:59:26
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef INVENTORY_HPP_
#define INVENTORY_HPP_

#include <vector>

#include <gk/core/Vector3.hpp>

#include "ItemStack.hpp"
#include "ISerializable.hpp"

class Inventory : public ISerializable {
	public:
		Inventory(u16 width, u16 height)
			: m_width(width), m_height(height) { m_items.resize(width * height); }

		const ItemStack &getStack(u16 x, u16 y) const { return m_items.at(x + y * m_width); }
		ItemStack &getStackRef(u16 x, u16 y) { return m_items.at(x + y * m_width); }
		void setStack(u16 x, u16 y, const std::string &name, u16 amount = 1);
		void addStack(const std::string &name, u16 amount = 1);

		void serialize(sf::Packet &packet) const override;
		void deserialize(sf::Packet &packet) override;

		u16 width() const { return m_width; }
		u16 height() const { return m_height; }

		const std::vector<ItemStack> &items() const { return m_items; }

		bool inBlock() const { return m_inBlock; }
		void setInBlock(bool inBlock) { m_inBlock = inBlock; }

		const gk::Vector3i &blockPos() const { return m_blockPos; }
		void setBlockPos(const gk::Vector3i &blockPos) { m_blockPos = blockPos; }

		bool hasChanged() const { return m_hasChanged; }
		void setChanged(bool hasChanged) { m_hasChanged = hasChanged; }

	private:
		u16 m_width = 0;
		u16 m_height = 0;

		std::vector<ItemStack> m_items;

		bool m_inBlock = false;
		gk::Vector3i m_blockPos{0, 0, 0};

		bool m_hasChanged = false; // Used to send inventory update packets
};

#endif // INVENTORY_HPP_
