// Copyright (C) 2021 Luca Gasperini
//
// This file is part of Master Tracker.
//
// Master Tracker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Master Tracker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Master Tracker.  If not, see <http://www.gnu.org/licenses/>.

#ifndef AT_MONO_VMREAD_H
#define AT_MONO_VMREAD_H

//! \file

#include <utils/utils.hpp>

#include <cstring>
#include <stdint.h>
#include <string>

namespace mtk::msp
{
extern int process_id;

/**
 * \brief Read data from another process. This function has multiple
 * implementations based on the operating system.
 *
 * \param buffer a preallocated buffer memory where load the data
 * \param addr address where the data is located
 * \param size data size
 * \param pid process id (process handler for win32)
 * \return execode EXE_INVALID_ARGS or EXE_ERROR if failed, EXE_OK on success
 */
execode read_data(void* buffer,
		  uint32_t addr,
		  uint32_t size = 1024,
		  int pid	= -1) noexcept;

/**
 * \brief Read an unsigned int with 64 bits (8 bytes) from another process
 *
 * \param addr address where the data is located
 * \param pid process id
 * \return uint64_t
 */
inline uint64_t
read_ui64(uint32_t addr, int pid = -1) noexcept
{
	uint64_t buffer;
	if (read_data(&buffer, addr, sizeof(uint64_t), pid) != EXE_OK)
		return 0;
	return buffer;
}

/**
 * \brief Read an unsigned int with 32 bits (4 bytes) from another process
 *
 * \param addr address where the data is located
 * \param pid process id
 * \return uint32_t
 */
inline uint32_t
read_ui32(uint32_t addr, int pid = -1) noexcept
{
	uint32_t buffer;
	if (read_data(&buffer, addr, sizeof(uint32_t), pid) != EXE_OK)
		return 0;

	return buffer;
}

/**
 * \brief Read an int with 32 bits (4 bytes) from another process
 *
 * \param addr address where the data is located
 * \param pid process id
 * \return int32_t
 */
inline int32_t
read_i32(uint32_t addr, int pid = -1) noexcept
{
	int32_t buffer;
	if (read_data(&buffer, addr, sizeof(int32_t), pid) != EXE_OK)
		return 0;
	return buffer;
}

/**
 * \brief Read a string from another process
 *
 * \param str where place the string
 * \param addr address where the data is located
 * \param maxsize max size of parameter str string
 * \param pid process id
 * \return EXE_ERROR if cant fetch data
 * \return EXE_OK if success
 */
inline execode
read_string(char* str,
	    uint32_t addr,
	    uint32_t maxsize = 1024,
	    int pid	     = -1) noexcept
{
	char buffer[maxsize];
	if (read_data(buffer, addr, maxsize, pid) != EXE_OK) {
		return EXE_ERROR;
	}
	strcpy(str, buffer);

	return EXE_OK;
}

/**
 * \brief Read an unsigned int with 16 bits (2 bytes) from another process
 *
 * \param addr address where the data is located
 * \param pid process id
 * \return uint16_t
 */
inline uint16_t
read_ui16(uint32_t addr, int pid = -1) noexcept
{
	uint16_t buffer;
	if (read_data(&buffer, addr, sizeof(uint16_t), pid) != EXE_OK)
		return 0;
	return buffer;
}

/**
 * \brief Read an unsigned int with 8 bits (1 bytes) from another process
 *
 * \param addr address where the data is located
 * \param pid process id
 * \return uint8_t
 */
inline uint8_t
read_ui8(uint32_t addr, int pid = -1) noexcept
{
	uint8_t buffer;
	if (read_data(&buffer, addr, sizeof(uint8_t), pid) != EXE_OK)
		return 0;
	return buffer;
}

/**
 * \brief Read a string of char16_t from another process
 *
 * \param str where place the string of char16_t
 * \param addr address where the data is located
 * \param maxsize max size of parameter str string
 * \param pid process id
 * \return execode
 */
inline execode
read_string16(char16_t* str,
	      uint32_t addr,
	      uint32_t maxsize = 1024,
	      int pid	       = -1) noexcept
{
	char16_t buffer[maxsize];
	if (read_data(buffer, addr, sizeof(char16_t) * maxsize, pid) !=
	    EXE_OK) {
		return EXE_ERROR;
	}

	if (str16ncpy(str, buffer, maxsize) != EXE_OK) {
		return EXE_ERROR;
	}

	return EXE_OK;
}

}    // namespace mtk::msp

#endif