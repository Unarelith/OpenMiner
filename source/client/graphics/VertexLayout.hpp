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
#ifndef VERTEXLAYOUT_HPP_
#define VERTEXLAYOUT_HPP_

#include <string>
#include <vector>

#include <gk/core/IntTypes.hpp>
#include <gk/gl/OpenGL.hpp>

struct VertexAttributeDef {
	VertexAttributeDef(u16 _id, const std::string &_name, GLint _size, GLenum _type,
			GLboolean _normalized, GLsizei _stride, const void *_offset)
		: id(_id), name(_name), size(_size), type(_type),
		normalized(_normalized), stride(_stride), offset(_offset) {}

	u16 id;
	std::string name;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const void *offset;
};

class VertexBufferLayout {
	using AttributeVector = std::vector<VertexAttributeDef>;

	using iterator = AttributeVector::iterator;
	using const_iterator = AttributeVector::const_iterator;

	public:
		template<typename... Args>
		void addAttribute(Args &&...args) {
			m_attributes.emplace_back(std::forward<Args>(args)...);
		}

		void setupDefaultLayout();

		void enableLayout() const;
		void disableLayout() const;

		iterator begin() { return m_attributes.begin(); }
		iterator end() { return m_attributes.end(); }
		const_iterator begin() const { return m_attributes.begin(); }
		const_iterator end() const { return m_attributes.end(); }

	private:
		AttributeVector m_attributes;
};

#endif // VERTEXLAYOUT_HPP_
