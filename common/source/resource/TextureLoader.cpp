/*
 * =====================================================================================
 *
 *  OpenMiner
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <gk/core/XMLFile.hpp>
#include <gk/gl/OpenGL.hpp>
#include <gk/gl/Texture.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "TextureLoader.hpp"

void TextureLoader::load(const char *xmlFilename, gk::ResourceHandler &handler) {
	gk::XMLFile doc(xmlFilename);

	tinyxml2::XMLElement *textureElement = doc.FirstChildElement("textures").FirstChildElement("texture").ToElement();
	while (textureElement) {
		std::string name = textureElement->Attribute("name");
		std::string path = textureElement->Attribute("path");

		auto &texture = handler.add<gk::Texture>("texture-" + name);
		texture.loadFromFile(path);

		// gk::Texture::bind(&texture);
		// glGenerateMipmap(GL_TEXTURE_2D);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
		// gk::Texture::bind(nullptr);

		textureElement = textureElement->NextSiblingElement("texture");
	}
}

