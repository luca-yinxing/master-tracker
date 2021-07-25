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

#include "monofield.hpp"

namespace mtk::msp
{
execode
get_field(uint32_t klass_addr, uint32_t addr, monofield& mf) noexcept
{
	if (klass_addr == ADDR_ZERO || addr == ADDR_ZERO) {
		return EXE_INVALID_ARGS;
	}
	mf.address = addr;
	mf.klass   = klass_addr;
	get_type(read_ui32(addr), mf.type);
	mf.name = new_string();
	get_field_name(addr, mf.name);
	mf.offset = get_field_offset(addr);

	return EXE_OK;
}

static int
s_find_field(uint32_t klass_addr,
	     uint32_t klass_parent,
	     monoclass_kind klass_kind,
	     uint32_t klass_fieldcount,
	     const char* name)
{
	uint32_t field_addr = ADDR_ZERO;

	// If class kind is a GINST search on GINST class
	if (klass_kind == GINST) {
		uint32_t ginst_addr =
		    read_ui32(read_ui32(klass_addr + CLASS_SIZEOF));
		if (get_class_fieldcount(ginst_addr) > 0) {
			field_addr = find_field(ginst_addr, name);
			if (field_addr != ADDR_ZERO)
				return field_addr;
		}
	}

	uint32_t first_field = read_ui32(klass_addr + CLASS_FIELDS);
	if (first_field == ADDR_ZERO) {
		return field_addr;    // 0
	}
	// Search in the class field list
	for (uint32_t field_index = 0; field_index < klass_fieldcount;
	     field_index++) {
		field_addr = first_field + (field_index * CLASS_FIELDSIZE);
		if (read_ui32(field_addr) == ADDR_ZERO) {
			break;
		}
		char buffer[DEF_BUFFER_STR];
		get_field_name(field_addr, buffer);
		if (strcmp(buffer, name) == 0)
			return field_addr;
	}

	// If not found, search in the parent classes
	if (klass_parent != ADDR_ZERO) {
		if (get_class_fieldcount(klass_parent) > 0) {
			field_addr = find_field(klass_parent, name);
			if (field_addr != ADDR_ZERO)
				return field_addr;
		}
	}

	return field_addr;    // 0
}

static int
s_field_list(uint32_t klass_addr,
	     uint32_t klass_parent,
	     monoclass_kind klass_kind,
	     uint32_t klass_fieldcount,
	     uint32_t* field_list,
	     int maxlen,
	     int start_index = 0)
{
	int pushed_item = 0;

	if (klass_parent != ADDR_ZERO) {
		if (get_class_fieldcount(klass_parent) > 0) {
			uint32_t parent_parent = get_class_parent(klass_parent);
			monoclass_kind parent_kind =
			    get_class_kind(klass_parent);
			uint32_t parent_fieldcount =
			    get_class_fieldcount(klass_parent);

			pushed_item += s_field_list(klass_parent,
						    parent_parent,
						    parent_kind,
						    parent_fieldcount,
						    field_list,
						    maxlen - pushed_item,
						    start_index + pushed_item);
		}
	}

	if (klass_kind == GINST) {
		uint32_t ginst_addr =
		    read_ui32(read_ui32(klass_addr + CLASS_SIZEOF));
		if (get_class_fieldcount(ginst_addr) > 0) {
			uint32_t ginst_parent = get_class_parent(ginst_addr);
			monoclass_kind ginst_kind = get_class_kind(ginst_addr);
			uint32_t ginst_fieldcount =
			    get_class_fieldcount(ginst_addr);

			pushed_item += s_field_list(ginst_addr,
						    ginst_parent,
						    ginst_kind,
						    ginst_fieldcount,
						    field_list,
						    maxlen - pushed_item,
						    start_index + pushed_item);
		}
	}

	uint32_t first_field = read_ui32(klass_addr + CLASS_FIELDS);
	if (first_field == ADDR_ZERO) {
		return pushed_item;    // 0
	}

	for (uint32_t field_index = 0; field_index < klass_fieldcount;
	     field_index++) {
		uint32_t field_ptr =
		    first_field + (field_index * CLASS_FIELDSIZE);
		if (read_ui32(field_ptr) == ADDR_ZERO) {
			break;
		}
		if (pushed_item < maxlen)
			field_list[pushed_item++] = field_ptr;
	}

	return pushed_item;
}

int
get_field_list(uint32_t klass_addr, uint32_t* field_list, int maxlen) noexcept
{
	uint32_t klass_parent	  = get_class_parent(klass_addr);
	monoclass_kind klass_kind = get_class_kind(klass_addr);
	uint32_t klass_fieldcount = get_class_fieldcount(klass_addr);

	return s_field_list(klass_addr,
			    klass_parent,
			    klass_kind,
			    klass_fieldcount,
			    field_list,
			    maxlen);
}

int
get_field_list(const monoclass& klass,
	       uint32_t* field_list,
	       int maxlen) noexcept
{
	return s_field_list(klass.address,
			    klass.parent,
			    klass.kind,
			    klass.fieldcount,
			    field_list,
			    maxlen);
}

int
get_field_list(const monovalue& value,
	       uint32_t* field_list,
	       int maxlen) noexcept
{
	return get_field_list(value.klass_def, field_list, maxlen);
}

uint32_t
find_field(uint32_t klass_addr, const char* name) noexcept
{
	uint32_t klass_parent	  = get_class_parent(klass_addr);
	monoclass_kind klass_kind = get_class_kind(klass_addr);
	uint32_t klass_fieldcount = get_class_fieldcount(klass_addr);

	return s_find_field(klass_addr,
			    klass_parent,
			    klass_kind,
			    klass_fieldcount,
			    name);
}

uint32_t
find_field(const monoclass& klass, const char* name) noexcept
{
	return s_find_field(klass.address,
			    klass.parent,
			    klass.kind,
			    klass.fieldcount,
			    name);
}

static execode
s_get_field_value(const monovalue& klass_v,
		  uint32_t field_addr,
		  monovalue& value) noexcept
{
	if (field_addr == ADDR_ZERO) {
		return EXE_ERROR;
	}

	uint32_t field_offset = get_field_offset(field_addr);
	bool is_value_type =
	    get_class_is_value_type(get_class_bitfield(klass_v.klass_def));
	monotype field_type;
	get_field_type(field_addr, field_type);

	if (is_value_class_def(klass_v)) {
		uint32_t klass_runtime_info =
		    get_class_runtimeinfo(klass_v.klass_def);
		uint32_t klass_vtable_addr =
		    get_class_vtable_addr(klass_runtime_info);
		uint32_t klass_vtable_size =
		    get_class_vtable_size(klass_v.klass_def);

		uint32_t vtable_data_addr =
		    get_class_data_addr(klass_vtable_addr, klass_vtable_size);

		uint32_t offset_addr = get_vtable_field_offset(vtable_data_addr,
							       field_offset,
							       is_value_type);

		return get_value(offset_addr, field_type, value);
	}

	if (!is_class_managed(klass_v.type_code))
		return EXE_ERROR;

	uint32_t offset_addr = get_vtable_field_offset(klass_v.klass_data,
						       field_offset,
						       is_value_type);

	return get_value(offset_addr, field_type, value);
}

execode
get_field_value(const monovalue& klass_v,
		const char* name,
		monovalue& value) noexcept
{
	uint32_t field_addr = find_field(klass_v.klass_def, name);

	return s_get_field_value(klass_v, field_addr, value);
}

execode
get_field_value(const monovalue& klass_v,
		uint32_t field_addr,
		monovalue& value) noexcept
{
	return s_get_field_value(klass_v, field_addr, value);
}

execode
get_field_value_string16(const monovalue& klass_v,
			 const char* name,
			 char16_t* str,
			 int length) noexcept
{
	if (str == nullptr || length < 1) {
		return EXE_INVALID_ARGS;
	}
	execode retval = EXE_OK;
	monovalue tmp;
	uint32_t field_addr = find_field(klass_v.klass_def, name);

	retval = s_get_field_value(klass_v, field_addr, tmp);

	if (tmp.type_code != STRING) {
		return EXE_INVALID_TYPE;
	}

	int val_size = str16len(tmp.str.data);

	if (val_size > length) {
		return EXE_OVERFLOW;
	}

	str16cpy(str, tmp.str.data);

	clear_value(tmp);

	return retval;
}

execode
get_field_value_string(const monovalue& klass_v,
		       const char* name,
		       char* str,
		       int length) noexcept
{
	if (str == nullptr || length < 1) {
		return EXE_INVALID_ARGS;
	}
	execode retval = EXE_OK;
	monovalue tmp;
	uint32_t field_addr = find_field(klass_v.klass_def, name);

	retval = s_get_field_value(klass_v, field_addr, tmp);

	if (tmp.type_code != STRING) {
		return EXE_INVALID_TYPE;
	}

	int val_size = str16len(tmp.str.data);

	if (val_size > length) {
		return EXE_OVERFLOW;
	}

	to_utf8(tmp.str.data, str, length);

	clear_value(tmp);

	return retval;
}

execode
get_field_value_int(const monovalue& klass_v,
		    const char* name,
		    int& dest) noexcept
{
	execode retval = EXE_OK;
	monovalue tmp;
	uint32_t field_addr = find_field(klass_v.klass_def, name);

	retval = s_get_field_value(klass_v, field_addr, tmp);

	if (tmp.type_code != I && tmp.type_code != I4) {
		return EXE_INVALID_TYPE;
	}

	dest = tmp.data.ui32;

	clear_value(tmp);

	return retval;
}

execode
get_field_value_ui64(const monovalue& klass_v,
		     const char* name,
		     uint64_t& dest) noexcept
{
	execode retval = EXE_OK;
	monovalue tmp;
	uint32_t field_addr = find_field(klass_v.klass_def, name);

	retval = s_get_field_value(klass_v, field_addr, tmp);

	if (tmp.type_code != U8) {
		return EXE_INVALID_TYPE;
	}

	dest = tmp.data.ui64;

	clear_value(tmp);

	return retval;
}

execode
get_field_value_i64(const monovalue& klass_v,
		    const char* name,
		    int64_t& dest) noexcept
{
	execode retval = EXE_OK;
	monovalue tmp;
	uint32_t field_addr = find_field(klass_v.klass_def, name);

	retval = s_get_field_value(klass_v, field_addr, tmp);

	if (tmp.type_code != I8) {
		return EXE_INVALID_TYPE;
	}

	dest = tmp.data.ui64;

	clear_value(tmp);

	return retval;
}

}    // namespace mtk::msp