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

#ifndef MT_MONO_MONOFIELD_H
#define MT_MONO_MONOFIELD_H

//! \file

#include "monoclass.hpp"
#include "monotype.hpp"
#include "monovalue.hpp"
#include "vmread.hpp"

#include <utils/utils.hpp>

namespace mtk::msp
{
/**
 * \brief data structure for mono class fields
 *
 */
struct monofield {
	/**
	 * \brief address of the field
	 *
	 */
	uint32_t address;
	/**
	 * \brief parent class address of the field
	 *
	 */
	uint32_t klass;
	/**
	 * \brief type data structure of the field
	 *
	 */
	monotype type;

	/**
	 * \brief string with the field name
	 *
	 */
	char* name;
	/**
	 * \brief offset of this field in the vtable
	 *
	 */
	int32_t offset;
};

/**
 * \brief Get the field offset
 *
 * \param addr address of the field
 * \return int32_t with offset value
 */
inline int32_t
get_field_offset(uint32_t addr) noexcept
{
	return read_i32(addr + 0xc);
}

/**
 * \brief Get the field name
 *
 * \param addr address of the field
 * \param str where put the string name
 * \param maxlen max length of the string
 * \return EXE_INVALID_ARGS if addr is ADDR_ZERO
 * \return EXE_ERROR if can't fetch the string
 * \return EXE_OK if success
 */
inline execode
get_field_name(uint32_t addr, char* str, int maxlen = DEF_BUFFER_STR) noexcept
{
	if (addr == ADDR_ZERO)
		return EXE_INVALID_ARGS;
	return read_string(str, read_ui32(addr + 0x4), maxlen);
}

/**
 * \brief Get the field type
 *
 * \param addr address of the field
 * \param type where put the result monotype
 * \return EXE_INVALID_ARGS if address is ADDR_ZERO
 * \return EXE_OK if success
 */
inline execode
get_field_type(uint32_t addr, monotype& type) noexcept
{
	return get_type(read_ui32(addr), type);
}

/**
 * \brief Get the vtable field offset
 *
 * \param vtable_data_addr address of the vtable data
 * \param field_offset field offset value
 * \param is_valuetype is a valuetype field
 * \return uint32_t patched address
 */
inline uint32_t
get_vtable_field_offset(uint32_t vtable_data_addr,
			int32_t field_offset,
			bool is_valuetype) noexcept
{
	return field_offset - (is_valuetype ? 0x8 : 0x0) + vtable_data_addr;
}

/**
 * \brief Get the field object by the class address
 *
 * \param klass_addr address of the parent class
 * \param addr address of the field
 * \param mf where put the result monofield
 * \return EXE_INVALID_ARGS if addr or klass_addr is ADDR_ZERO
 * \return EXE_OK if success
 */
execode get_field(uint32_t klass_addr, uint32_t addr, monofield& mf) noexcept;

/**
 * \brief Get the field list by the class address
 *
 * \param klass_addr address of the class
 * \param field_list array where put the result field list
 * \param maxlen max length of the field list
 * \return int count of the element pushed into the field list
 */
int get_field_list(uint32_t klass_addr,
		   uint32_t* field_list,
		   int maxlen = DEF_BUFFER_FIELDLIST) noexcept;
/**
 * \brief Get the field list by the monoclass object
 *
 * \param klass monoclass object
 * \param field_list array where put the result field list
 * \param maxlen max length of the field list
 * \return int count of the element pushed into the field list
 */
int get_field_list(const monoclass& klass,
		   uint32_t* field_list,
		   int maxlen = DEF_BUFFER_FIELDLIST) noexcept;
/**
 * \brief Get the field list by the monovalue object
 *
 * \param value monovalue object
 * \param field_list array where put the result field list
 * \param maxlen max length of the field list
 * \return int count of the element pushed into the field list
 */
int get_field_list(const monovalue& value,
		   uint32_t* field_list,
		   int maxlen = DEF_BUFFER_FIELDLIST) noexcept;

/**
 * \brief field a field address by the class address and field name
 *
 * \param klass_addr address of the class
 * \param name name of the field to search
 * \return uint32_t 0 if not found
 * \return uint32_t address of the field
 */
uint32_t find_field(uint32_t klass_addr, const char* name) noexcept;
/**
 * \brief field a field address by the class object and field name
 *
 * \param klass class object
 * \param name name of the field to search
 * \return uint32_t 0 if not found
 * \return uint32_t address of the field
 */
uint32_t find_field(const monoclass& klass, const char* name) noexcept;

/**
 * \brief Get the value of the field by name
 *
 * \param klass_v mono class value
 * \param name string with the field name
 * \param value where put the result monovalue
 * \return EXE_ERROR if a generic error occurred
 * \return EXE_OK if success
 */
execode get_field_value(const monovalue& klass_v,
			const char* name,
			monovalue& value) noexcept;

/**
 * \brief Get the value of the field by address
 *
 * \param klass_v mono class value
 * \param field_addr field address
 * \param value where put the result monovalue
 * \return EXE_ERROR if a generic error occurred
 * \return EXE_OK if success
 */
execode get_field_value(const monovalue& klass_v,
			uint32_t field_addr,
			monovalue& value) noexcept;

execode get_field_value_string16(const monovalue& klass_v,
				 const char* name,
				 char16_t* str,
				 int length) noexcept;

execode get_field_value_string(const monovalue& klass_v,
			       const char* name,
			       char* str,
			       int length) noexcept;

execode get_field_value_int(const monovalue& klass_v,
			    const char* name,
			    int& dest) noexcept;

execode get_field_value_ui64(const monovalue& klass_v,
			     const char* name,
			     uint64_t& dest) noexcept;

execode get_field_value_i64(const monovalue& klass_v,
			    const char* name,
			    int64_t& dest) noexcept;
}    // namespace mtk::msp

#endif