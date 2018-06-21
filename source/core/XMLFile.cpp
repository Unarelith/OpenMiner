/*
 * =====================================================================================
 *
 *       Filename:  XMLFile.cpp
 *
 *    Description:
 *
 *        Created:  17/01/2018 19:22:02
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Exception.hpp"
#include "XMLFile.hpp"

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
			case tinyxml2::XML_ERROR_FILE_READ_ERROR:
				errorString = "File read error.";
				break;
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

