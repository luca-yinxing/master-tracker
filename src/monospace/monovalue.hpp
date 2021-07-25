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

#ifndef AT_MONO_MONOVALUE_H
#define AT_MONO_MONOVALUE_H

//! \file

#include "monoclass.hpp"
#include "monotype.hpp"

namespace mtk::msp
{
constexpr int MAX_ALLOCABLE_ITEMS = 32768;

struct monovalue;

/**
 * \brief data structure for mono managed strings with UTF-16 encoding
 *
 */
struct monostring {
	/**
	 * \brief how many char in this string
	 *
	 */
	int count = 0;
	/**
	 * \brief array with UTF-16 char
	 *
	 */
	char16_t* data = nullptr;
};

/**
 * \brief data structure for mono managed arrays
 *
 */
struct monoarray {
	/**
	 * \brief how many element in this array
	 *
	 */
	int count = 0;
	/**
	 * \brief array with monovalues
	 *
	 */
	monovalue* data = nullptr;
};

/**
 * \brief data structure of a generic mono object allocated in memory with its
 * value
 *
 */
struct monovalue {
	/**
	 * \brief address of the object class definition
	 *
	 */
	uint32_t klass_def = ADDR_ZERO;
	/**
	 * \brief address of the object class data
	 *
	 */
	uint32_t klass_data = ADDR_ZERO;
	/**
	 * \brief flag for initialization in monospace memory
	 *
	 */
	bool init = false;
	/**
	 * \brief type of this object class
	 *
	 */
	monotype_code type_code = END;

	/**
	 * \brief container for raw data
	 *
	 */
	union {
		uint64_t ui64 = 0;
		uint32_t ui32;
		uint16_t ui16;
		uint8_t ui8;
		float f32;
		double f64;
	} data;

	/**
	 * \brief container for string data
	 *
	 */
	monostring str;
	/**
	 * \brief container for array data
	 *
	 */
	monoarray array;
};

/**
 * \brief error safety function for monovalue class
 *
 * \param value monovalue class
 * \return true the value is valid
 * \return false the value is not valid
 */
inline bool
value_valid(const monovalue& value) noexcept
{
	if (value.init == false)
		return false;
	if (value.type_code == CLASS || value.type_code == VALUETYPE ||
	    value.type_code == GENERICINST || value.type_code == SZARRAY ||
	    value.type_code == STRING) {
		if (value.klass_def == ADDR_ZERO &&
		    value.klass_data == ADDR_ZERO)
			return false;
	}
	return true;
}

/**
 * \brief check if the monovalue is just a class definition or not
 *
 * \param value monovalue class
 * \return true value is a class definition
 * \return false value is not a class definition
 */
inline bool
is_value_class_def(const monovalue& value) noexcept
{
	return value.klass_data == ADDR_ZERO && value.klass_def != ADDR_ZERO;
}

/**
 * \brief Get the array count object by monovalue
 *
 * \param array monovalue class
 * \return -1 if array is not a array
 * \return number of object in array
 */
inline int
get_array_count(const monovalue& array) noexcept
{
	if (array.type_code != SZARRAY)
		return -1;

	return array.array.count;
}

/**
 * \brief Get the array value object by monovalue
 *
 * \param array monovalue class
 * \param index element number to get
 * \param value where put the result value
 * \return EXE_INVALID_TYPE if array is not an array
 * \return EXE_OUT_OF_RANGE if index is out of range
 * \return EXE_OK if success
 */
execode
get_array_value(const monovalue& array, int index, monovalue& value) noexcept;
/**
 * \brief Get the array value object by monoarray
 *
 * \param array monoarray class
 * \param index element number to get
 * \param value where put the result value
 * \return EXE_OUT_OF_RANGE if index is out of range
 * \return EXE_OK if success
 */
execode
get_array_value(const monoarray& array, int index, monovalue& value) noexcept;

/**
 * \brief Get the array value to an int
 *
 * \param value monovalue class
 * \param index element number to get
 * \param dest where put the result value
 * \return EXE_INVALID_TYPE if array is not an array or is not an int
 * \return EXE_OUT_OF_RANGE if index is out of range
 * \return EXE_OK if success
 */
execode get_array_value_int(monovalue& value, int index, int& dest) noexcept;

/**
 * \brief Get the value object by value offset address
 *
 * \param offset_addr value offset address
 * \param type type of the value
 * \param value where put the result value
 * \return EXE_ERROR if type is invalid
 * \return EXE_NOT_IMPL if cant handle this type
 * \return EXE_OK if success
 */
execode
get_value(uint32_t offset_addr, monotype type, monovalue& value) noexcept;
/**
 * \brief Get the value data object by address
 *
 * \param addr address of the value
 * \param type type of the value
 * \param value where put the result value
 * \return EXE_ERROR if handler type function failed
 * \return EXE_NOT_IMPL if cant handle this type
 * \return EXE_OK if success
 */
execode get_value_data(uint32_t addr, monotype type, monovalue& value) noexcept;

/**
 * \brief handler type function for mono managed array
 *
 * \param addr address of the array in mono memory
 * \param value where put the result value
 * \return EXE_INVALID_ARGS if address is ADDR_ZERO
 * \return EXE_ERROR if can't read the array
 * \return EXE_OVERFLOW if request count of char is bigger than
 * MAX_ALLOCABLE_ITEMS \return EXE_OK if success
 */
execode value_managed_array(uint32_t addr, monovalue& value);

/**
 * \brief handler type function for mono managed class
 *
 * \param addr address of the class in mono memory
 * \param value where put the result value
 * \return EXE_INVALID_ARGS if address is ADDR_ZERO
 * \return EXE_ERROR if can't read the class
 * \return EXE_OK if success
 */
execode value_managed_class(uint32_t addr, monovalue& value);

/**
 * \brief handler type function for mono managed generic instance
 *
 * \param addr address of the generic instance in mono memory
 * \param type_data type data
 * \param value where put the result value
 * \return EXE_INVALID_ARGS if address is ADDR_ZERO
 * \return EXE_ERROR if can't read the generic instance
 * \return EXE_OK if success
 */
execode
value_managed_generic(uint32_t addr, uint32_t type_data, monovalue& value);

/**
 * \brief handler type function for mono managed generic instance
 *
 * \param addr address of the generic instance in mono memory
 * \param type_data type data
 * \param value where put the result value
 * \return EXE_INVALID_ARGS if address or type_data is ADDR_ZERO
 * \return EXE_OK if success
 */
execode
value_managed_valuetype(uint32_t addr, uint32_t type_data, monovalue& value);

/**
 * \brief handler type function for mono managed string
 *
 * \param addr address of the string in mono memory
 * \param value where put the result value
 * \return EXE_INVALID_ARGS if address or type_data is ADDR_ZERO
 * \return EXE_OVERFLOW if request count of char is bigger than
 * MAX_ALLOCABLE_ITEMS \return EXE_OK if success
 */
execode value_managed_string(uint32_t addr, monovalue& value) noexcept;

/**
 * \brief Get the value int object
 *
 * \param value monovalue class
 * \param dest where put the result value
 * \return EXE_INVALID_TYPE if data value is not integer
 * \return EXE_OK if success
 */
inline execode
get_value_int(monovalue& value, int& dest) noexcept
{
	if (value.type_code != I && value.type_code != I4) {
		return EXE_INVALID_TYPE;
	}

	dest = value.data.ui32;

	return EXE_OK;
}

/**
 * \brief Get the class object address
 *
 * \param value monovalue class
 * \return uint32_t address of the class if success
 * \return ADDR_ZERO if class in not initialized
 */
inline uint32_t
get_class(const monovalue& value) noexcept
{
	if (!value.init)
		return ADDR_ZERO;

	// TODO: Check for other types
	return value.klass_def;
}

/**
 * \brief Get the class object to monoclass
 *
 * \param value monovalue class
 * \param klass where put the class result
 * \return EXE_ERROR if class fetch process failed
 * \return EXE_OK if success
 */
inline execode
get_class(const monovalue& value, monoclass& klass) noexcept
{
	uint32_t klass_addr = get_class(value);

	if (klass_addr == 0)
		return EXE_ERROR;

	return get_class(klass_addr, klass);
}

/**
 * \brief Get the value object by print it on a string
 *
 * \param value value of the object
 * \param str string where print the value
 * \param maxlen max lenght of the string
 * \return EXE_INVALID_ARGS if str is nullptr or value is invalid
 * \return EXE_NOT_IMPL if can't handle this type
 * \return EXE_OK if success
 */
execode get_value_string(const monovalue& value,
			 char* str,
			 int maxlen = DEF_BUFFER_STR) noexcept;

/**
 * \brief clear the memory of a allocated value (important for strings and
 * arrays)
 *
 * \param value value of the object
 * \return EXE_OK if success
 */
inline execode
clear_value(monovalue& value) noexcept
{
	switch (value.type_code) {
	case BOOLEAN:
	case CHAR:
	case I1:
	case U1:
	case I2:
	case U2:
	case I:
	case I4:
	case U:
	case U4:
	case I8:
	case U8:
	case R4:
	case R8:
		value.data = { 0 };
		break;

	case STRING:
		if (value.str.data != nullptr) {
			free_memory(value.str.data);
			value.str.data	= nullptr;
			value.str.count = 0;
		}
		break;
	case SZARRAY:
		if (value.array.data != nullptr) {
			free_memory(value.array.data);
			value.array.data  = nullptr;
			value.array.count = 0;
		}
		break;

	case VALUETYPE:
	case CLASS:
	case GENERICINST:
	default:
		break;
	}

	value.init	 = false;
	value.type_code	 = VOID;
	value.klass_data = ADDR_ZERO;
	value.klass_def	 = ADDR_ZERO;

	return EXE_OK;
}

}    // namespace mtk::msp

#endif