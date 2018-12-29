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
#include <gk/core/XMLFile.hpp>
#include <gk/gl/OpenGL.hpp>
#include <gk/gl/Texture.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "TextureLoader.hpp"

void TextureLoader::load(const char *xmlFilename, gk::ResourceHandler &handler) {
	XMLFile doc(xmlFilename);

	tinyxml2::XMLElement *textureElement = doc.FirstChildElement("textures").FirstChildElement("texture").ToElement();
	while (textureElement) {
		std::string name = textureElement->Attribute("name");
		std::string path = textureElement->Attribute("path");

		auto &texture = handler.add<gk::Texture>("texture-" + name);
		texture.loadFromFile(path);

		textureElement = textureElement->NextSiblingElement("texture");
	}
}

