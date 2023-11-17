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
#ifndef PROGRESSBARWIDGET_HPP_
#define PROGRESSBARWIDGET_HPP_

#include "common/world/BlockData.hpp"

#include "client/graphics/Image.hpp"
#include "client/gui/Widget.hpp"

// Duplicated in ProgressBarWidgetDef
enum class ProgressBarType : u8 {
	ItemProcess = 0,
	BurnProcess = 1
};

class ProgressBarWidget : public Widget {
	public:
		ProgressBarWidget(const Texture &texture, BlockData &blockData, ProgressBarType type, Widget *parent = nullptr);

		void init(const FloatRect &clipRect, const Vector2i &position, const std::string &meta, unsigned int maxMetaValue);
		void init(const FloatRect &clipRect, const Vector2i &position, const std::string &meta, const std::string &maxMeta);

		void update() override;

	private:
		void draw(RenderTarget &target, RenderStates states) const override;

		BlockData &m_blockData;

		FloatRect m_clipRect;
		Vector2i m_position;

		std::string m_meta;
		std::string m_maxMeta;
		unsigned int m_maxMetaValue = 0;

		Image m_image;

		ProgressBarType m_type;
};

#endif // PROGRESSBARWIDGET_HPP_
