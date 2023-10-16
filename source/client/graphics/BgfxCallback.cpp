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
#include <bx/debug.h>
#include <bx/file.h>
#include <bx/string.h>

#include <bimg/bimg.h>

#include "BgfxCallback.hpp"

void BgfxCallback::fatal(const char* _filePath, uint16_t _line, bgfx::Fatal::Enum _code, const char* _str) {
	trace(_filePath, _line, "BGFX FATAL 0x%08x: %s\n", _code, _str);

	if (bgfx::Fatal::DebugCheck == _code) {
		bx::debugBreak();
	}
	else {
		abort();
	}
}

void BgfxCallback::traceVargs(const char* _filePath, uint16_t _line, const char* _format, va_list _argList) {
	char temp[2048];
	char* out = temp;
	va_list argListCopy;
	va_copy(argListCopy, _argList);
	int32_t len   = bx::snprintf(out, sizeof(temp), "%s (%d): ", _filePath, _line);
	int32_t total = len + bx::vsnprintf(out + len, sizeof(temp)-len, _format, argListCopy);
	va_end(argListCopy);
	if ((int32_t)sizeof(temp) < total) {
		out = (char*)alloca(total+1);
		bx::memCopy(out, temp, len);
		bx::vsnprintf(out + len, total-len, _format, _argList);
	}
	out[total] = '\0';
	bx::debugOutput(out);
}

void BgfxCallback::profilerBegin(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/) {
}

void BgfxCallback::profilerBeginLiteral(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/) {
}

void BgfxCallback::profilerEnd() {
}

uint32_t BgfxCallback::cacheReadSize(uint64_t /*_id*/) {
	return 0;
}

bool BgfxCallback::cacheRead(uint64_t /*_id*/, void* /*_data*/, uint32_t /*_size*/) {
	return false;
}

void BgfxCallback::cacheWrite(uint64_t /*_id*/, const void* /*_data*/, uint32_t /*_size*/) {
}

void BgfxCallback::screenShot(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void* _data, uint32_t _size, bool _yflip) {
	BX_UNUSED(_filePath, _width, _height, _pitch, _data, _size, _yflip);

	const int32_t len = bx::strLen(_filePath)+5;
	char* filePath = (char*)alloca(len);
	bx::strCopy(filePath, len, _filePath);
	bx::strCat(filePath, len, ".tga");

	bx::FileWriter writer;
	if (bx::open(&writer, filePath)) {
		bimg::imageWritePng(&writer, _width, _height, _pitch, _data, bimg::TextureFormat::RGBA8, _yflip);
		bx::close(&writer);
	}
}

void BgfxCallback::captureBegin(uint32_t /*_width*/, uint32_t /*_height*/, uint32_t /*_pitch*/, bgfx::TextureFormat::Enum /*_format*/, bool /*_yflip*/) {
	BX_TRACE("Warning: using capture without callback (a.k.a. pointless).");
}

void BgfxCallback::captureEnd() {
}

void BgfxCallback::captureFrame(const void* /*_data*/, uint32_t /*_size*/) {
}

void BgfxCallback::trace(const char* _filePath, uint16_t _line, const char* _format, ...) {
	va_list argList;
	va_start(argList, _format);

	traceVargs(_filePath, _line, _format, argList);

	va_end(argList);
}
