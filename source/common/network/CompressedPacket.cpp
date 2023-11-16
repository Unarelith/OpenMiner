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
#include <cassert>

#include "core/Debug.hpp"
#include "network/CompressedPacket.hpp"

// Note: This class was implemented thanks to this SFML forum topic:
// https://en.sfml-dev.org/forums/index.php?topic=14344.0

const void* CompressedPacket::onSend(std::size_t& size) {
	// We only support data with a maximum size of
	// an unsigned short (so the size can be sent
	// in the first two bytes of the packet)
	assert(size <= 65535);

	// Cast the data to a bytef pointer
	const Bytef* srcData = static_cast<const Bytef*>(getData());

	// Get the size of the packet to send
	uLong srcSize = (uLong)getDataSize();

	// Compute the size of the compressed data
	uLong dstSize = compressBound(srcSize);

	// Resize the vector to accomodate the compressed data,
	// plus two bytes for our uncompressed size
	m_compressionBuffer.resize(dstSize + 2);

	// Take the first 8 bytes of srcSize
	m_compressionBuffer[0] = srcSize & 0xFF;

	// And the second 8 bytes
	m_compressionBuffer[1] = (srcSize >> 8) & 0xFF;

	// Compress the data into the rest of the buffer
	int result = compress(m_compressionBuffer.data() + 2, &dstSize, srcData, srcSize);
	if (result != Z_OK)
		logError() << "Failed to compress packet";

	// Set the size to the compressed size plus
	// two bytes for the size marker
	size = (dstSize + 2);

	// Return data to send
	return m_compressionBuffer.data();
}

void CompressedPacket::onReceive(const void* data, std::size_t size) {
	// Cast the data to Bytef*, the format zlib deals with
	const Bytef* srcData = static_cast<const Bytef*>(data);

	// Extract the uncompressed data size from the first two
	// bytes in the packet so we can use it for the buffer
	sf::Uint16 uncompressedSize = sf::Uint16(srcData[1] << 8 | srcData[0]);

	// Resize the vector to accomodate the uncompressed data
	m_compressionBuffer.resize(uncompressedSize);

	// Declare a variable for the destination size
	uLong dstSize = uncompressedSize;

	// Uncompress the data (remove the first two bytes)
	int result = uncompress(m_compressionBuffer.data(), &dstSize, (srcData + 2), (uLong)(size - 2));
	if (result != Z_OK)
		logError() << "Failed to uncompress packet";

	// Assert that the uncompressed size is the same as the
	// size we were sent for the buffer
	assert(dstSize == uncompressedSize);

	// Append it to the packet
	append(m_compressionBuffer.data(), dstSize);
}
