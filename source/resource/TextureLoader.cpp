/*
 * =====================================================================================
 *
 *       Filename:  TextureLoader.cpp
 *
 *    Description:
 *
 *        Created:  09/04/2018 01:38:03
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <SFML/Graphics/Texture.hpp>

#include "OpenGL.hpp"
#include "ResourceHandler.hpp"
#include "TextureLoader.hpp"
#include "XMLFile.hpp"

void TextureLoader::load(const char *xmlFilename, ResourceHandler &handler) {
	XMLFile doc(xmlFilename);

	tinyxml2::XMLElement *textureElement = doc.FirstChildElement("textures").FirstChildElement("texture").ToElement();
	while (textureElement) {
		std::string name = textureElement->Attribute("name");
		std::string path = textureElement->Attribute("path");

		auto &texture = handler.add<sf::Texture>("texture-" + name);
		texture.loadFromFile(path);

		sf::Texture::bind(&texture);
		glGenerateMipmap(GL_TEXTURE_2D);
		// FIXME: GL_NEAREST_MIPMAP_LINEAR causes blue shadows on trees, probably due to blending..
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
		sf::Texture::bind(nullptr);

		textureElement = textureElement->NextSiblingElement("texture");
	}
}

