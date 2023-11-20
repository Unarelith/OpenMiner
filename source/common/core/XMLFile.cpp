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
#include "common/core/Exception.hpp"
#include "common/core/XMLFile.hpp"

XMLFile::XMLFile(const std::string &filename) {
	load(filename);
}

void XMLFile::load(const std::string &filename) {
	int code = m_xml.LoadFile(filename.c_str());

	if(code != 0) {
		std::string errorString;

		switch(code) {
			case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
				errorString = "File not found.";
				break;
			case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
				errorString = "File couldn't be opened.";
				break;
			// case tinyxml2::XML_ERROR_FILE_READ_ERROR:
			// 	errorString = "File read error.";
			// 	break;
			case tinyxml2::XML_ERROR_PARSING_ELEMENT:
				errorString = "Error while parsing element.";
				break;
			case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:
				errorString = "Error while parsing attribute.";
				break;
			case tinyxml2::XML_ERROR_PARSING_TEXT:
				errorString = "Error while parsing text.";
				break;
			case tinyxml2::XML_ERROR_PARSING_CDATA:
				errorString = "Error while parsing cdata.";
				break;
			case tinyxml2::XML_ERROR_PARSING_COMMENT:
				errorString = "Error while parsing comment.";
				break;
			case tinyxml2::XML_ERROR_PARSING_DECLARATION:
				errorString = "Error while parsing declaration.";
				break;
			case tinyxml2::XML_ERROR_PARSING_UNKNOWN:
				errorString = "Parsing error: Unknown object.";
				break;
			case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:
				errorString = "Empty document.";
				break;
			case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:
				errorString = "Element mismatched.";
				break;
			case tinyxml2::XML_ERROR_PARSING:
				errorString = "Parsing error.";
				break;
			default:
				errorString = "Unknown error.";
				break;
		}

		throw EXCEPTION("Failed to load", filename, "\nError", code, ":", errorString);
	}
}
