/*
 * =====================================================================================
 *
 *       Filename:  XMLFile.hpp
 *
 *    Description:
 *
 *        Created:  17/01/2018 19:21:52
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef XMLFILE_HPP_
#define XMLFILE_HPP_

#include <string>

#include <tinyxml2.h>

class XMLFile {
	public:
		XMLFile() = default;
		XMLFile(const std::string &filename);
		XMLFile(const XMLFile &) = default;
		XMLFile(XMLFile &&) = default;

		void load(const std::string &filename);

		tinyxml2::XMLHandle FirstChildElement(const char *element) { return m_doc.FirstChildElement(element); }

	private:
		tinyxml2::XMLDocument m_xml;
		tinyxml2::XMLHandle m_doc{m_xml};
};

#endif // XMLFILE_HPP_
