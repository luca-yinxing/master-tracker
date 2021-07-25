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

#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H

//! \file
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/stat.h>

#include <string>

namespace mtk
{
/**
 * \brief enum for return codes
 *
 */
enum execode
{
	/**
	 * \brief function return correctly without runtime errors
	 *
	 */
	EXE_OK = 0,
	/**
	 * \brief function return a generic error
	 *
	 */
	EXE_ERROR = 1,
	/**
	 * \brief function return that arguments are invalid
	 *
	 */
	EXE_INVALID_ARGS,
	/**
	 * \brief function return an attempt of access to an out of range array
	 * position
	 *
	 */
	EXE_OUT_OF_RANGE,
	/**
	 * \brief function return an attempt to use an invalid type
	 *
	 */
	EXE_INVALID_TYPE,
	/**
	 * \brief function return an not implemented feature is called
	 *
	 */
	EXE_NOT_IMPL,
	/**
	 * \brief function return an overflow memory situation
	 *
	 */
	EXE_OVERFLOW,
	/**
	 * \brief function return an not found value
	 *
	 */
	EXE_NOT_FOUND,
	/**
	 * \brief function return an open file descriptor error
	 *
	 */
	EXE_CANT_OPEN,
	/**
	 * \brief function return false
	 *
	 */
	EXE_FALSE,
	/**
	 * \brief function return an os error
	 *
	 */
	EXE_ERROR_OS,
};

/**
 * \brief Constant definition for memory address zero
 *
 */
constexpr const uint32_t ADDR_ZERO = 0U;

constexpr const char* STR_NULL = "";

constexpr const char CHAR_NULL = '\0';

constexpr const int DEF_BUFFER_STR = 128;

constexpr const int DEF_BUFFER_STR_LONG = 256;

constexpr const int DEF_BUFFER_CLASSLIST = 8192;

constexpr const int DEF_BUFFER_FIELDLIST = 256;

constexpr const char* STR_OS_DIR_SEP = "/";

constexpr const char* STR_URL_SEP = "/";

/**
 * \brief Get the length of a string16
 *
 * \param src source string16
 * \return -1 src is nullptr
 * \return int if success
 */
inline int
str16len(const char16_t* src) noexcept
{
	if (src == nullptr) {
		return -1;
	}

	int i = 0;

	while (src[i] != 0) {
		i++;
	}

	return i;
}

/**
 * \brief Copy a string16 into another memory string16 for a given length
 *
 * \param dest destination char16 string
 * \param src source char16 string
 * \param len length of the copy
 * \return int -1 if dest or src are nullptr
 * \return int how many char are copied
 */
inline int
str16ncpy(char16_t* dest, const char16_t* src, int len) noexcept
{
	if (dest == nullptr || src == nullptr)
		return -1;

	int i = 0;

	for (; i < len && src[i] != 0; i++) {
		dest[i] = src[i];
	}

	return i;
}

/**
 * \brief Copy a string16 into another memory string16
 *
 * \param dest destination char16 string
 * \param src source char16 string
 * \return int -1 if dest or src are nullptr
 * \return int how many char are copied
 */
inline int
str16cpy(char16_t* dest, char16_t* src) noexcept
{
	if (dest == nullptr || src == nullptr)
		return -1;

	int i = 0;

	do {
		dest[i] = src[i];
	} while (src[i++] != 0);

	return i;
}

inline char*
new_string(int length = DEF_BUFFER_STR) noexcept
{
	if (length < 0)
		return nullptr;

	char* str   = new char[length + 1];
	str[length] = '\0';

	return str;
}

inline char16_t*
new_string16(int length = DEF_BUFFER_STR) noexcept
{
	if (length < 0)
		return nullptr;

	char16_t* str = (char16_t*)malloc((length + 1) * sizeof(char16_t));
	str[length]   = '\0';

	return str;
}

inline void*
new_array(int items, int szof) noexcept
{
	if (items <= 0 || szof <= 0)
		return nullptr;

	return malloc(items * szof);
}

inline void*
realloc_array(void* ptr, int items, int szof) noexcept
{
	if (items <= 0 || szof <= 0)
		return nullptr;

	return realloc(ptr, items * szof);
}

inline void*
new_array_zero(int items, int szof) noexcept
{
	if (items <= 0 || szof <= 0)
		return nullptr;

	return calloc(items, szof);
}

inline void*
new_memory(int memsize) noexcept
{
	if (memsize <= 0)
		return nullptr;

	return malloc(memsize);
}

inline void*
new_memory_zero(int memsize) noexcept
{
	if (memsize <= 0)
		return nullptr;

	return calloc(memsize, 1);
}

inline void*
realloc_memory(void* ptr, size_t memsize) noexcept
{
	if (memsize <= 0)
		return nullptr;

	return realloc(ptr, memsize);
}

inline char*
new_string_copy(const char* src) noexcept
{
	if (src == nullptr)
		return nullptr;

	int length = strlen(src);
	char* dest = new_string(length);
	strncpy(dest, src, length);
	return dest;
}

inline char16_t*
new_string16_copy(const char16_t* src) noexcept
{
	if (src == nullptr)
		return nullptr;

	int length     = str16len(src);
	char16_t* dest = new_string16(length);
	str16ncpy(dest, src, length);
	return dest;
}

inline void*
new_memory_copy(const void* src, int length)
{
	if (src == nullptr || length <= 0)
		return nullptr;

	void* dest = new_memory(length);
	memcpy(dest, src, length);
	return dest;
}

inline void*
new_array_copy(const void* src, int items, int szof)
{
	if (src == nullptr || items <= 0 || szof <= 0)
		return nullptr;

	size_t memsize = items * szof;

	void* dest = new_memory(memsize);
	memcpy(dest, src, memsize);
	return dest;
}

inline void*
new_array_zero_copy(const void* src, int items, int szof) noexcept
{
	if (src == nullptr || items <= 0 || szof <= 0)
		return nullptr;

	void* dest = new_array_zero(items, szof);
	memcpy(dest, src, items * szof);
	return dest;
}

inline execode
free_memory(void* mem) noexcept
{
	if (mem == nullptr)
		return EXE_ERROR;

	free(mem);
	return EXE_OK;
}

/**
 * \brief Get a copy of a given position from a byte array in a uint32.
 *
 * \param mem the byte array
 * \param start the position inside the array, unexpected behavior if _mem[x] <
 * _start \return uint32_t copy of the value
 */
inline uint32_t
to_ui32(uint8_t* mem, uint32_t start = 0) noexcept
{
	return *(reinterpret_cast<uint32_t*>(mem + start));
}

/**
 * \brief Cut values from a char16 string to tranform in a char8 string
 *
 * \param src source char16 string
 * \param dest destination char string
 * \param maxlen max length for both
 * \return EXE_INVALID_ARGS if src or dest are nullptr
 * \return EXE_OK if success
 */
inline execode
to_utf8(const char16_t* src, char* dest, int maxlen = DEF_BUFFER_STR) noexcept
{
	if (src == nullptr || dest == nullptr)
		return EXE_INVALID_ARGS;

	int i = 0;
	while (src[i] != 0) {
		if (i >= maxlen) {
			return EXE_OVERFLOW;
		}
		dest[i] = src[i];
		i++;
	}

	dest[i] = 0;

	return EXE_OK;
}

inline execode
to_utf16(const char src[],
	 char16_t dest[],
	 int maxlen = DEF_BUFFER_STR) noexcept
{
	if (src == nullptr || dest == nullptr)
		return EXE_INVALID_ARGS;

	int i = 0;
	while (src[i] != 0) {
		if (i >= maxlen) {
			return EXE_OVERFLOW;
		}
		dest[i] = src[i];
		i++;
	}

	dest[i] = 0;

	return EXE_OK;
}

inline execode
strconcat(char* buffer, const char* s1, const char* s2) noexcept
{
	if (buffer == nullptr || s1 == nullptr || s2 == nullptr) {
		return EXE_INVALID_ARGS;
	}

	strcpy(buffer, s1);
	strcat(buffer, s2);

	return EXE_OK;
}

inline execode
strconcat(char* buffer, const char* s1, const char* s2, const char* s3) noexcept
{
	if (buffer == nullptr || s1 == nullptr || s2 == nullptr ||
	    s3 == nullptr) {
		return EXE_INVALID_ARGS;
	}

	strcpy(buffer, s1);
	strcat(buffer, s2);
	strcat(buffer, s3);

	return EXE_OK;
}

inline execode
strconcat(char* buffer, const char** items, int items_num) noexcept
{
	if (buffer == nullptr || items == nullptr || items_num < 1) {
		return EXE_INVALID_ARGS;
	}
	if (items[0] == nullptr) {
		return EXE_ERROR;
	}

	strcpy(buffer, items[0]);

	for (int i = 1; i < items_num; i++) {
		if (items[i] == nullptr) {
			return EXE_ERROR;
		}
		strcat(buffer, items[i]);
	}

	return EXE_OK;
}

inline execode
check_unique_array_int(const int* data, size_t count) noexcept
{
	if (data == nullptr) {
		return EXE_INVALID_ARGS;
	}

	for (size_t a = 0; a < count; a++) {
		for (size_t b = 0; b < count; b++) {
			if (data[a] == data[b]) {
				return EXE_FALSE;
			}
		}
	}

	return EXE_OK;
}

inline execode
check_unique_array_int(const int* first,
		       size_t first_count,
		       const int* second,
		       size_t second_count) noexcept
{
	if (first == nullptr || second == nullptr) {
		return EXE_INVALID_ARGS;
	}

	for (size_t a = 0; a < first_count; a++) {
		for (size_t b = 0; b < second_count; b++) {
			if (first[a] == second[b]) {
				return EXE_FALSE;
			}
		}
	}
	return EXE_OK;
}

inline bool
strok(const char* str) noexcept
{
	return str != nullptr && *str != CHAR_NULL;
}

inline bool
file_exists(const std::string& path) noexcept
{
	if (path.empty()) {
		return false;
	}

	return access(path.c_str(), R_OK) == 0;
}

inline execode
os_mkdir(const std::string& path, uint mode = 0755)
{
	if (path.empty() || mode == 0) {
		return EXE_INVALID_ARGS;
	}

	if (file_exists(path)) {
		return EXE_OK;
	}

	if (mkdir(path.c_str(), mode) != 0) {
		return EXE_ERROR_OS;
	}

	return EXE_OK;
}

inline execode
strlower(char* str)
{
	if (str == NULL) {
		return EXE_INVALID_ARGS;
	}

	size_t len = strlen(str);
	for (size_t i = 0; i < len; i++) {
		str[i] = tolower(str[i]);
	}

	return EXE_OK;
}

inline execode
strlower(const char* src, char* dest)
{
	if (src == NULL || dest == NULL) {
		return EXE_INVALID_ARGS;
	}

	size_t len = strlen(src);
	for (size_t i = 0; i < len; i++) {
		dest[i] = tolower(src[i]);
	}

	return EXE_OK;
}

inline execode
strlists(char* buffer,
	 const char** items,
	 size_t items_num,
	 size_t start	 = 0,
	 const char* sep = ",") noexcept
{
	if (buffer == NULL || items == NULL || items_num < 1 || sep == NULL) {
		return EXE_INVALID_ARGS;
	}
	if (items[start] == NULL) {
		return EXE_ERROR;
	}

	strcpy(buffer, items[start]);

	for (size_t i = start + 1; i < items_num; i++) {
		if (items[i] == NULL) {
			return EXE_ERROR;
		}
		strcat(buffer, sep);
		strcat(buffer, items[i]);
	}

	return EXE_OK;
}

}    // namespace mtk

#endif