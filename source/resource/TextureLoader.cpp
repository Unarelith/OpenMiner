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

		textureElement = textureElement->NextSiblingElement("texture");
	}
}

