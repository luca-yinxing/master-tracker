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

#ifndef AT_MONO_PROCESS_H
#define AT_MONO_PROCESS_H

//! \file

#include <utils/utils.hpp>

#include <string>

namespace mtk::msp
{
/**
 * \brief An OS indipendent interface for windows module informations.
 * \link
 * https://docs.microsoft.com/en-us/windows/win32/api/psapi/ns-psapi-moduleinfo
 * Win32 interface \endlink
 *
 */
struct moduleinfo {
	/**
	 * \brief The load address of the module
	 *
	 */
	uint32_t address = 0;
	/**
	 * \brief The size of the module in bytes
	 *
	 */
	uint32_t size = 0;
};

/**
 * \brief Search in the loaded modules of a process for the module with the
 * given name. Has multiple implementations.
 *
 * \sa moduleinfo
 * \param pid process id
 * \param m_info module pointer
 * \param m_name module name
 * \return execode EXE_INVALID_ARGS or EXE_ERROR if failed, EXE_OK on success
 */
execode find_module(int pid, moduleinfo* m_info, const char* m_name);
/**
 * \brief Get the process id from the OS. Has multiple implementations.
 *
 * \param name the process name
 * \return int -1 if failed else current process id
 */
int get_pid(const char* name);

}    // namespace mtk::msp

#endif