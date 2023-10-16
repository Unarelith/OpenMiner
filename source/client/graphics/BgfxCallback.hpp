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
#ifndef BGFXCALLBACK_HPP_
#define BGFXCALLBACK_HPP_

#include <bgfx/bgfx.h>

class BgfxCallback : public bgfx::CallbackI {
	public:
		virtual ~BgfxCallback() = default;

		void fatal(const char* _filePath, uint16_t _line, bgfx::Fatal::Enum _code, const char* _str) override;

		void traceVargs(const char* _filePath, uint16_t _line, const char* _format, va_list _argList) override;

		void profilerBegin(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/) override;
		void profilerBeginLiteral(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/) override;
		void profilerEnd() override;

		uint32_t cacheReadSize(uint64_t /*_id*/) override;
		bool cacheRead(uint64_t /*_id*/, void* /*_data*/, uint32_t /*_size*/) override;
		void cacheWrite(uint64_t /*_id*/, const void* /*_data*/, uint32_t /*_size*/) override;

		void screenShot(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void* _data, uint32_t _size, bool _yflip) override;

		void captureBegin(uint32_t /*_width*/, uint32_t /*_height*/, uint32_t /*_pitch*/, bgfx::TextureFormat::Enum /*_format*/, bool /*_yflip*/) override;
		void captureEnd() override;
		void captureFrame(const void* /*_data*/, uint32_t /*_size*/) override;

	private:
		void trace(const char* _filePath, uint16_t _line, const char* _format, ...);
};

#endif // BGFXCALLBACK_HPP_
