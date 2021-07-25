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

#ifndef AT_MONO_MONOSPACE_H
#define AT_MONO_MONOSPACE_H

//! \file

#include "monoclass.hpp"
#include "monofield.hpp"
#include "monovalue.hpp"
#include "process.hpp"

#include <stdint.h>
#include <vector>

namespace mtk::msp
{
/**
 * \brief Look for the address of the mono_get_root_domain function in the mono
 * module.
 *
 * \param mi the mono module information
 * \return uint32_t 0 if failed else address of the mono function
 */
uint32_t find_root_domain(const moduleinfo& mi);
/**
 * \brief Look up the address of a given assembly in the mono root domain
 *
 * \param root_addr mono root domain address
 * \param assembly name of assembly
 * \return uint32_t 0 if failed else address of the assembly
 */
uint32_t find_assembly(uint32_t root_addr, const char* assembly);

/**
 * \brief find a class definition from the assembly address
 *
 * \param assembly_addr assembly address
 * \param name string with the name of the class
 * \param value where put the class definition value
 * \return EXE_INVALID_ARGS if assembly_addr is ADDR_ZERO or name is nullptr
 * \return EXE_ERROR if can't find the class by name
 * \return EXE_OK if success
 */
inline execode
find_class(uint32_t assembly_addr, const char* name, monovalue& value) noexcept
{
	if (name == nullptr || assembly_addr == ADDR_ZERO) {
		return EXE_INVALID_ARGS;
	}

	value.klass_def = find_class(assembly_addr, name);
	if (value.klass_def == ADDR_ZERO)
		return EXE_ERROR;

	value.type_code = CLASS;
	value.init	= true;
	return EXE_OK;
}
}    // namespace mtk::msp

#endif