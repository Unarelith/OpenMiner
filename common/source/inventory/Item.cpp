/*
 * =====================================================================================
 *
 *       Filename:  Item.cpp
 *
 *    Description:
 *
 *        Created:  23/06/2018 22:30:25
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Item.hpp"

Item::Item(u32 id, const std::string &textureFilename, const std::string &name, const std::string &label) {
	m_id = id;
	m_textureFilename = textureFilename;

	m_name = name;
	m_label = label;
}

