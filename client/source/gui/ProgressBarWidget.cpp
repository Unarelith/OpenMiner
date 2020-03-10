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
#include "ProgressBarWidget.hpp"

ProgressBarWidget::ProgressBarWidget(const gk::Texture &texture, BlockData &blockData, ProgressBarType type, Widget *parent)
	: Widget(parent), m_blockData(blockData), m_image(texture)
{
	m_type = type;
}

void ProgressBarWidget::init(const gk::FloatRect &clipRect, const gk::Vector2i &position, const std::string &meta, unsigned int maxMetaValue) {
	m_clipRect = clipRect;
	m_position = position;

	m_image.setClipRect(clipRect.x, clipRect.y, clipRect.sizeX, clipRect.sizeY);
	m_image.setPosition(position.x, position.y);

	m_meta = meta;
	m_maxMetaValue = maxMetaValue;
}

void ProgressBarWidget::init(const gk::FloatRect &clipRect, const gk::Vector2i &position, const std::string &meta, const std::string &maxMeta) {
	m_clipRect = clipRect;
	m_position = position;

	m_image.setClipRect(clipRect.x, clipRect.y, clipRect.sizeX, clipRect.sizeY);
	m_image.setPosition(position.x, position.y);

	m_meta = meta;
	m_maxMeta = maxMeta;
}

void ProgressBarWidget::update() {
	int metaValue = m_blockData.meta.get<int>(m_meta);
	if (!m_maxMeta.empty())
		m_maxMetaValue = m_blockData.meta.get<int>(m_maxMeta);

	if (m_maxMetaValue == 0) {
		m_image.setClipRect(0, 0, 0, 0);
	}
	else if (m_type == ProgressBarType::ItemProcess) {
		m_image.setClipRect(m_clipRect.x, m_clipRect.y, (float)metaValue / m_maxMetaValue * m_clipRect.sizeX, m_clipRect.sizeY);
	}
	else if (m_type == ProgressBarType::BurnProcess) {
		float height = ceil((float)metaValue / m_maxMetaValue * m_clipRect.sizeY);
		m_image.setPosition(m_position.x, m_position.y + m_clipRect.sizeY - height);
		m_image.setClipRect(m_clipRect.x, m_clipRect.y + m_clipRect.sizeY - height, m_clipRect.sizeX, height);
	}
}

void ProgressBarWidget::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_image, states);
}

