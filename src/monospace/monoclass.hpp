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

#ifndef MT_MONO_MONOCLASS_H
#define MT_MONO_MONOCLASS_H

//! \file

#include "monotype.hpp"
#include "offset.hpp"
#include "vmread.hpp"

#include <utils/utils.hpp>

#include <stdint.h>
#include <string>

namespace mtk::msp
{
/**
 * \brief enum for mono class kind
 *
 */
enum monoclass_kind
{
	DEF	= 1,
	GTG	= 2,
	GINST	= 3,
	GPARAM	= 4,
	KARRAY	= 5,
	POINTER = 6,
};

/**
 * \brief Get the class managed vtable
 *
 * \param addr address of the managed class
 * \return uint32_t address
 */
inline uint32_t
get_class_managed_vtable(uint32_t addr) noexcept
{
	return read_ui32(addr);
}

/**
 * \brief Get the class managed klass
 *
 * \param addr address of the managed class
 * \return uint32_t address
 */
inline uint32_t
get_class_managed_klass(uint32_t addr) noexcept
{
	return read_ui32(read_ui32(addr));
}

/**
 * \brief data structure for mono classes
 *
 */
struct monoclass {
	/**
	 * \brief data for flags
	 *
	 */
	uint32_t bitfield;
	/**
	 * \brief field counter value
	 *
	 */
	uint32_t fieldcount;
	/**
	 * \brief parent class address
	 *
	 */
	uint32_t parent;
	/**
	 * \brief nestedin class address
	 *
	 */
	uint32_t nestedin;

	/**
	 * \brief class address
	 *
	 */
	uint32_t address;
	/**
	 * \brief string for the name of the class
	 *
	 */
	char* name;
	/**
	 * \brief string for the namespace of the class
	 *
	 */
	char* namespace_name;
	/**
	 * \brief size of the object
	 *
	 */
	int32_t size;
	/**
	 * \brief runtime info address
	 *
	 */
	uint32_t runtime_info;
	/**
	 * \brief vtable address
	 *
	 */
	uint32_t vtable_address;
	/**
	 * \brief vtable size
	 *
	 */
	int32_t vtable_size;
	/**
	 * \brief type structure of the monoclass
	 *
	 */
	monotype type;
	/**
	 * \brief class kind
	 *
	 */
	monoclass_kind kind;

	/**
	 * \brief offset of the class
	 *
	 */
	uint32_t offset;
};

/**
 * \brief Get the class offset
 *
 * \param addr address of the class
 * \return uint32_t class offset
 */
inline uint32_t
get_class_offset(uint32_t addr) noexcept
{
	return read_ui32(addr + CLASS_OFFSET);
}

/**
 * \brief Get the class name
 *
 * \param addr address of the class
 * \param str where put the string name
 * \param maxlen max length of the string
 * \return EXE_INVALID_ARGS if addr is ADDR_ZERO
 * \return EXE_ERROR if can't fetch the string
 * \return EXE_OK if success
 */
inline execode
get_class_name(uint32_t addr, char* str, int maxlen = DEF_BUFFER_STR) noexcept
{
	if (addr == ADDR_ZERO)
		return EXE_INVALID_ARGS;

	return read_string(str, read_ui32(addr + CLASS_NAME), maxlen);
}

/**
 * \brief Get the class namespace
 *
 * \param addr address of the class
 * \param str where put the string name
 * \param maxlen max length of the string
 * \return EXE_INVALID_ARGS if addr is ADDR_ZERO
 * \return EXE_ERROR if can't fetch the string
 * \return EXE_OK if success
 */
inline execode
get_class_namespace(uint32_t addr,
		    char* str,
		    int maxlen = DEF_BUFFER_STR) noexcept
{
	if (addr == ADDR_ZERO)
		return EXE_INVALID_ARGS;

	return read_string(str, read_ui32(addr + CLASS_NAMESPACE), maxlen);
}

/**
 * \brief Get the class size
 *
 * \param addr address of the class
 * \return int32_t class size
 */
inline int32_t
get_class_size(uint32_t addr) noexcept
{
	return read_i32(addr + CLASS_SIZE);
}

/**
 * \brief Get the class runtime info
 *
 * \param addr address of the class
 * \return uint32_t runtime info address
 */
inline uint32_t
get_class_runtimeinfo(uint32_t addr) noexcept
{
	return read_ui32(addr + CLASS_RUNTIME_INFO);
}

/**
 * \brief Get the class vtable address
 *
 * \param runtime_info runtime info address of the class
 * \return uint32_t vtable address
 */
inline uint32_t
get_class_vtable_addr(uint32_t runtime_info) noexcept
{
	return read_ui32(runtime_info + CLASS_RUNTIME_INFO_DOMAIN_VTABLE);
}

/**
 * \brief Get the class vtable size
 *
 * \param addr address of the class
 * \return int32_t vtable size
 */
inline int32_t
get_class_vtable_size(uint32_t addr) noexcept
{
	return read_i32(addr + CLASS_VTABLE_SIZE);
}

/**
 * \brief Get the class bitfield
 *
 * \param addr address of the class
 * \return uint32_t bitfield flags
 */
inline uint32_t
get_class_bitfield(uint32_t addr) noexcept
{
	return read_ui32(addr + CLASS_BITFIELD);
}

/**
 * \brief Get the class field count
 *
 * \param addr address of the class
 * \return uint32_t field count
 */
inline uint32_t
get_class_fieldcount(uint32_t addr) noexcept
{
	return read_ui32(addr + CLASS_FIELDCOUNT);
}

/**
 * \brief Get the class parent address
 *
 * \param addr address of the class
 * \return uint32_t parent address
 */
inline uint32_t
get_class_parent(uint32_t addr) noexcept
{
	return read_ui32(addr + CLASS_PARENT);
}

/**
 * \brief Get the class kind
 *
 * \param addr address of the class
 * \return monoclass_kind
 */
inline monoclass_kind
get_class_kind(uint32_t addr) noexcept
{
	return (monoclass_kind)(read_ui8(addr + CLASS_KIND) & 0x7);
}

/**
 * \brief Get the class is value type
 *
 * \param bitfield bitfield for flags of the class
 * \return true is a value type
 * \return false is not a value type
 */
inline bool
get_class_is_value_type(uint32_t bitfield) noexcept
{
	return (bitfield & 0x4) == 0x4;
}

/**
 * \brief Get the class is enum
 *
 * \param bitfield bitfield for flags of the class
 * \return true is a enum
 * \return false is not a enum
 */
inline bool
get_class_is_enum(uint32_t bitfield) noexcept
{
	return (bitfield & 0x8) == 0x8;
}

/**
 * \brief is a managed class
 *
 * \param code type code of the class
 * \return true is a mono managed class
 * \return false is not a mono managed class
 */
inline bool
is_class_managed(monotype_code code) noexcept
{
	return code == CLASS || code == GENERICINST || code == VALUETYPE;
}

/**
 * \brief Get the class type
 *
 * \param addr address of the class
 * \param type monotype object where put the result
 * \return EXE_INVALID_ARGS if address is ADDR_ZERO
 * \return EXE_OK if success
 */
inline execode
get_class_type(uint32_t addr, monotype& type) noexcept
{
	return get_type(addr + CLASS_BYVAL_ARG, type);
}

/**
 * \brief Get the class data addr
 *
 * \param vtable_addr vtable address of the class
 * \param vtable_size vtable size of class
 * \return uint32_t address of the data
 */
inline uint32_t
get_class_data_addr(uint32_t vtable_addr, int32_t vtable_size) noexcept
{
	return read_ui32(vtable_addr + VTABLE + (vtable_size * 0x4));
}

/**
 * \brief Get the class object
 *
 * \param addr address of the class
 * \param mc where put the class object result
 * \return EXE_INVALID_ARGS if addr is ADDR_ZERO
 * \return EXE_OK if success
 */
execode get_class(uint32_t addr, monoclass& mc) noexcept;

/**
 * \brief Get into class_list vector all mono class address on class cache
 *
 * \sa class_list
 * \param assembly_addr assembly address
 * \param class_list array where put the addresses
 * \param maxlen max element number of class_list array
 * \return int number of item insert in the class_list
 */
int get_class_list(uint32_t assembly_addr,
		   uint32_t* class_list,
		   int maxlen = DEF_BUFFER_CLASSLIST);

/**
 * \brief find a class definition from the assembly address
 *
 * \param assembly_addr assembly address
 * \param name string with the name of the class
 * \return uint32_t ADDR_ZERO if the class is not found
 * \return uint32_t address of the class definition
 */
uint32_t find_class(uint32_t assembly_addr, const char* name) noexcept;

}    // namespace mtk::msp
#endif