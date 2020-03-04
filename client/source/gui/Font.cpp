/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <fstream>

#include <gk/gl/Texture.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "Font.hpp"

Font::Font(const std::string &textureName, const std::string &configPath)
	: m_texture(gk::ResourceHandler::getInstance().get<gk::Texture>(textureName))
{
	m_textureName = textureName;

	std::memset(m_charWidth, 0, sizeof(u8) * 256);

	parseConfig(configPath);
}

gk::Vector2f Font::getTexCoords(u8 c, u8 x, u8 y) const {
	u8 tileX = c % (m_texture.getSize().x / m_width);
	u8 tileY = c / (m_texture.getSize().x / m_width);

	gk::Vector2f texCoords{
		(tileX + x) * m_width  / (float)m_texture.getSize().x,
		(tileY + y) * m_height / (float)m_texture.getSize().y
	};

	return texCoords;
}

void Font::parseConfig(const std::string &configPath) {
	std::ifstream file(configPath);

	std::string line;
	while (std::getline(file, line, '\n')) {
		if (line.empty()) continue;

		u16 dot = line.find_first_of('.');
		u16 equal = line.find_first_of('=');

		std::string propertyName = line.substr(0, dot);
		if (propertyName != "width")
			throw EXCEPTION("Unexpected property for font:", propertyName);

		int propertyKey = std::stoi(line.substr(dot + 1, equal - dot - 1));
		u8 propertyValue = std::stoi(line.substr(equal + 1));

		m_charWidth[propertyKey] = propertyValue;
	}
}

