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

#ifndef AT_MONO_MONOTYPE_H
#define AT_MONO_MONOTYPE_H

//! \file

#include "vmread.hpp"

#include <stdint.h>

namespace mtk::msp
{
/**
 * \brief enum list of the mono types code
 *
 */
enum monotype_code
{
	END = 0x00, /* End of List */

	VOID = 0x01,

	BOOLEAN = 0x02,

	CHAR = 0x03,

	I1 = 0x04,

	U1 = 0x05,

	I2 = 0x06,

	U2 = 0x07,

	I4 = 0x08,

	U4 = 0x09,

	I8 = 0x0a,

	U8 = 0x0b,

	R4 = 0x0c,

	R8 = 0x0d,

	STRING	  = 0x0e,
	PTR	  = 0x0f, /* arg: <type> token */
	BYREF	  = 0x10, /* arg: <type> token */
	VALUETYPE = 0x11, /* arg: <type> token */
	CLASS	  = 0x12, /* arg: <type> token */
	VAR	  = 0x13, /* number */
	ARRAY	  = 0x14, /* type, rank, boundsCount, bound1, loCount, lo1 */
	GENERICINST =
	    0x15, /* <type> <type-arg-count> <type-1> \x{2026} <type-n> */
	TYPEDBYREF = 0x16,

	I = 0x18,

	U	  = 0x19,
	FNPTR	  = 0x1b, /* arg: full method signature */
	OBJECT	  = 0x1c,
	SZARRAY	  = 0x1d, /* 0-based one-dim-array */
	MVAR	  = 0x1e, /* number */
	CMOD_REQD = 0x1f, /* arg: typedef or typeref token */
	CMOD_OPT  = 0x20, /* optional arg: typedef or typref token */
	INTERNAL  = 0x21, /* CLR internal type */
	MODIFIER  = 0x40, /* Or with the following types */
	SENTINEL  = 0x41, /* Sentinel for varargs method signature */
	PINNED	  = 0x45, /* Local var that points to pinned object */
	ENUM	  = 0x55  /* an enumeration */
};

/**
 * \brief data structure for mono type information and data
 *
 */
struct monotype {
	/**
	 * \brief mono type structure address
	 *
	 */
	uint32_t address;
	/**
	 * \brief runtime data
	 *
	 */
	uint32_t data;
	/**
	 * \brief attributes for flags as static and constant type
	 *
	 */
	uint32_t attrs;
	/**
	 * \brief mono type code
	 *
	 */
	monotype_code code;
};

/**
 * \brief Get the type runtime data
 *
 * \param addr address of the monotype
 * \return uint32_t with runtime data value
 */
inline uint32_t
get_type_data(uint32_t addr) noexcept
{
	return read_ui32(addr);
}

/**
 * \brief Get the type attributes
 *
 * \param addr address of the monotype
 * \return uint32_t with attributes value
 */
inline uint32_t
get_type_attr(uint32_t addr) noexcept
{
	return read_ui32(addr + 0x4);
}

/**
 * \brief Get the type code
 *
 * \param attrs monotype attributes value
 * \return monotype_code value
 */
inline monotype_code
get_type_code(uint32_t attrs) noexcept
{
	return static_cast<monotype_code>(0xff & (attrs >> 16));
}

/**
 * \brief Get the type static value
 *
 * \param attrs monotype attributes value
 * \return true is a static type
 * \return false is not a static type
 */
inline bool
get_type_static(uint32_t attrs) noexcept
{
	return (attrs & 0x10) == 0x10;
}

/**
 * \brief Get the type constant value
 *
 * \param attrs monotype attributes value
 * \return true type is constant
 * \return false type is not constant
 */
inline bool
get_type_constant(uint32_t attrs) noexcept
{
	return (attrs & 0x40) == 0x40;
}

/**
 * \brief Get the type object
 *
 * \param addr address of the monotype
 * \param mt where put the monotype result
 * \return EXE_INVALID_ARGS if address is ADDR_ZERO
 * \return EXE_OK if success
 */
inline execode
get_type(uint32_t addr, monotype& mt) noexcept
{
	if (addr == ADDR_ZERO)
		return EXE_INVALID_ARGS;

	mt.address = addr;
	mt.data	   = get_type_data(addr);
	mt.attrs   = get_type_attr(addr);
	mt.code	   = get_type_code(mt.attrs);

	return EXE_OK;
}

}    // namespace mtk::msp

#endif