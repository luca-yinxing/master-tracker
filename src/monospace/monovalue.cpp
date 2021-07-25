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

#include "monovalue.hpp"
#include <unistdio.h>

namespace mtk::msp
{
execode
get_array_value(const monovalue& array, int index, monovalue& value) noexcept
{
	if (array.type_code != SZARRAY)
		return EXE_INVALID_TYPE;

	return get_array_value(array.array, index, value);
}

execode
get_array_value(const monoarray& array, int index, monovalue& value) noexcept
{
	if (array.count < index || index < 0)
		return EXE_OUT_OF_RANGE;

	value = array.data[index];

	return EXE_OK;
}

execode
get_array_value_int(monovalue& value, int index, int& dest) noexcept
{
	if (value.type_code != SZARRAY)
		return EXE_INVALID_TYPE;

	if (value.array.count < index || index < 0)
		return EXE_OUT_OF_RANGE;

	monovalue* tmp = &value.array.data[index];

	if (tmp->type_code != I && tmp->type_code != I4) {
		return EXE_INVALID_TYPE;
	}

	dest = tmp->data.ui32;

	return EXE_OK;
}

execode
get_value(uint32_t offset_addr, monotype type, monovalue& value) noexcept
{
	value.type_code = type.code;
	value.init	= true;

	return get_value_data(offset_addr, type, value);
}

execode
value_managed_array(uint32_t addr, monovalue& value)
{
	if (addr == ADDR_ZERO) {
		return EXE_INVALID_ARGS;
	}

	value.klass_data = read_ui32(addr);

	if (value.klass_data == ADDR_ZERO) {
		return EXE_ERROR;
	}

	uint32_t vtable	   = read_ui32(value.klass_data);
	uint32_t array_ptr = read_ui32(vtable);
	// Is empty array?
	if (vtable == array_ptr) {
		value.array.data  = nullptr;
		value.array.count = 0;
		return EXE_ERROR;
	}
	monotype element_type;
	value.klass_def = read_ui32(array_ptr);
	get_class_type(value.klass_def, element_type);
	int32_t array_size = get_class_size(array_ptr);

	value.array.count = read_i32(value.klass_data + 0xc);
	if (value.array.count > MAX_ALLOCABLE_ITEMS) {
		return EXE_OVERFLOW;
	}
	uint32_t start = value.klass_data + 0x10;
	value.array.data =
	    (monovalue*)new_array(value.array.count, sizeof(monovalue));

	for (int i = 0; i < value.array.count; i++) {
		get_value_data(start + (i * array_size),
			       element_type,
			       value.array.data[i]);
	}

	return EXE_OK;
}

execode
value_managed_class(uint32_t addr, monovalue& value)
{
	if (addr == ADDR_ZERO) {
		return EXE_INVALID_ARGS;
	}

	value.klass_data = read_ui32(addr);
	if (value.klass_data == ADDR_ZERO)
		return EXE_ERROR;

	value.klass_def = get_class_managed_klass(value.klass_data);

	return EXE_OK;
}

execode
value_managed_generic(uint32_t addr, uint32_t type_data, monovalue& value)
{
	uint32_t definition_addr = read_ui32(type_data);
	if (get_class_is_value_type(get_class_bitfield(definition_addr))) {
		value.klass_def	 = definition_addr;
		value.klass_data = addr;
		return EXE_OK;
	}

	return value_managed_class(addr, value);
}

execode
value_managed_valuetype(uint32_t addr, uint32_t type_data, monovalue& value)
{
	if (addr == ADDR_ZERO || type_data == ADDR_ZERO) {
		return EXE_INVALID_ARGS;
	}

	value.klass_data = addr;
	value.klass_def	 = type_data;
	return EXE_OK;
}

execode
get_value_data(uint32_t addr, monotype type, monovalue& value) noexcept
{
	value.type_code = type.code;
	value.init	= true;

	switch (type.code) {
	case BOOLEAN:
		value.data.ui8 = read_ui8(addr);
		break;

	case CHAR:
		value.data.ui8 = read_ui8(addr);
		break;

	case I1:
		value.data.ui8 = read_ui8(addr);
		break;

	case U1:
		value.data.ui8 = read_ui8(addr);
		break;

	case I2:
		value.data.ui16 = read_ui16(addr);
		break;

	case U2:
		value.data.ui16 = read_ui16(addr);
		break;

	case I:
	case I4:
		value.data.ui32 = read_ui32(addr);
		break;

	case U:
	case U4:
		value.data.ui32 = read_ui32(addr);
		break;

	case I8:
		value.data.ui64 = read_ui64(addr);
		break;

	case U8:
		value.data.ui64 = read_ui64(addr);
		break;

	case R4:
		value.data.f32 = read_ui64(addr);
		break;

	case R8:
		value.data.f64 = read_ui64(addr);
		break;

	case STRING:
		return value_managed_string(addr, value);

	case SZARRAY:
		return value_managed_array(addr, value);

	case VALUETYPE:
		return value_managed_valuetype(addr, type.data, value);

	case CLASS:
		return value_managed_class(addr, value);

	case GENERICINST:
		return value_managed_generic(addr, type.data, value);

	case OBJECT:

	case VAR:
	case ARRAY:
	case ENUM:
	case MVAR:

	case END:
	case VOID:
	case PTR:
	case BYREF:
	case TYPEDBYREF:
	case FNPTR:
	case CMOD_REQD:
	case CMOD_OPT:
	case INTERNAL:
	case MODIFIER:
	case SENTINEL:
	case PINNED:
		return EXE_NOT_IMPL;
	default:
		return EXE_NOT_IMPL;
	}

	return EXE_OK;
}

execode
value_managed_string(uint32_t addr, monovalue& value) noexcept
{
	if (addr == ADDR_ZERO) {
		return EXE_INVALID_ARGS;
	}

	value.klass_data = read_ui32(addr);
	value.str.count	 = read_i32(value.klass_data + 0x8);
	if (value.str.count > MAX_ALLOCABLE_ITEMS) {
		return EXE_OVERFLOW;
	}
	if (value.str.count <= 0) {
		// TODO: is it fine?
		value.str.count = 0;
		value.str.data	= new_string16(0);
		return EXE_OK;
	}

	value.str.data = new_string16(value.str.count);

	read_string16(value.str.data, value.klass_data + 0xc, value.str.count);
	return EXE_OK;
}

execode
get_value_string(const monovalue& value, char* str, int maxlen) noexcept
{
	if (!value_valid(value))
		return EXE_INVALID_ARGS;

	if (str == nullptr)
		return EXE_INVALID_ARGS;

	switch (value.type_code) {
	case BOOLEAN:
		snprintf(str, maxlen, "Bool(%i)", value.data.ui8);
		break;

	case CHAR:
		snprintf(str, maxlen, "Char(%i)", value.data.ui8);
		break;

	case I1:
		snprintf(str, maxlen, "Int8(%i)", value.data.ui8);
		break;

	case U1:
		snprintf(str, maxlen, "UInt8(%i)", value.data.ui8);
		break;

	case I2:
		snprintf(str, maxlen, "Int16(%i)", value.data.ui16);
		break;

	case U2:
		snprintf(str, maxlen, "UInt16(%i)", value.data.ui16);
		break;

	case I:
	case I4:
		snprintf(str, maxlen, "Int32(%i)", value.data.ui32);
		break;

	case U:
	case U4:
		snprintf(str, maxlen, "Uint32(%i)", value.data.ui32);
		break;

	case I8:
		snprintf(str, maxlen, "Int64(%li)", value.data.ui64);
		break;

	case U8:
		snprintf(str, maxlen, "UInt64(%li)", value.data.ui64);
		break;

	case R4:
		snprintf(str, maxlen, "Float(%f)", value.data.f32);
		break;

	case R8:
		snprintf(str, maxlen, "Double(%f)", value.data.f64);
		break;

	case STRING:
	{
		if (value.str.count == 0 || value.str.data == nullptr) {
			snprintf(str, maxlen, "EmptyString()");
			break;
		}
		char buffer[value.str.count + 1];
		to_utf8(value.str.data, buffer, value.str.count);
		buffer[value.str.count] = '\0';
		snprintf(str, maxlen, "String(%s)", buffer);
		break;
	}
	case SZARRAY:
	{
		char tmp_name[DEF_BUFFER_STR];
		if (value.array.data == nullptr || value.array.count == 0) {
			snprintf(str, maxlen, "EmptyArray()");
			break;
		}
		if (get_class_name(value.klass_def, tmp_name) != EXE_OK) {
			snprintf(str, maxlen, "error_get_class_name(Array)");
			return EXE_ERROR;
		} else {
			snprintf(str, maxlen, "Array(%s)", tmp_name);
		}
		break;
	}
	case VALUETYPE:
	{
		char tmp_name[DEF_BUFFER_STR];
		if (get_class_name(value.klass_def, tmp_name) != EXE_OK) {
			snprintf(str,
				 maxlen,
				 "error_get_class_name(ValueType)");
			return EXE_ERROR;
		} else {
			snprintf(str, maxlen, "ValueType(%s)", tmp_name);
		}
		break;
	}
	case CLASS:
	{
		char tmp_name[DEF_BUFFER_STR];
		if (get_class_name(value.klass_def, tmp_name) != EXE_OK) {
			snprintf(str, maxlen, "error_get_class_name(Class)");
			return EXE_ERROR;
		} else {
			snprintf(str, maxlen, "Class(%s)", tmp_name);
		}
		break;
	}
	case GENERICINST:
	{
		char tmp_name[DEF_BUFFER_STR];
		if (get_class_name(value.klass_def, tmp_name) != EXE_OK) {
			snprintf(str, maxlen, "error_get_class_name(GInst)");
			return EXE_ERROR;
		} else {
			snprintf(str, maxlen, "GInst(%s)", tmp_name);
		}
		break;
	}
	case OBJECT:

	case VAR:
	case ARRAY:
	case ENUM:
	case MVAR:

	case END:
	case VOID:
	case PTR:
	case BYREF:
	case TYPEDBYREF:
	case FNPTR:
	case CMOD_REQD:
	case CMOD_OPT:
	case INTERNAL:
	case MODIFIER:
	case SENTINEL:
	case PINNED:
		snprintf(str, maxlen, "Unknown()");
		return EXE_NOT_IMPL;
	default:
		snprintf(str, maxlen, "Unknown()");
		return EXE_NOT_IMPL;
	}

	return EXE_OK;
}
}    // namespace mtk::msp