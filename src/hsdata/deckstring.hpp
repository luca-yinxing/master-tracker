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

#ifndef HSDATA_DECKSTRING_H
#define HSDATA_DECKSTRING_H

#include "deck.hpp"

#include <utils/utils.hpp>

namespace mtk
{
constexpr int VERSION_DECKSTRING = 1;

/**
 * \brief Read from a byte array an unsigned integer with 8 bit
 *
 * \param data byte array first position
 * \return uint8_t
 */
inline uint8_t
read_ui8(const u_char* data) noexcept
{
	return (uint8_t)data[0];
}

/**
 * \brief Read from a byte array an unsigned integer with 16 bit
 *
 * \param data byte array first position
 * \return uint16_t
 */
inline uint16_t
read_ui16(const u_char* data) noexcept
{
	uint16_t value = 0;

	for (int i = 0; i < 2; i++) {
		value = data[i] << (i * 8);
	}

	return value;
}

/**
 * \brief Read from a byte array an unsigned integer with 32 bit
 *
 * \param data byte array first position
 * \return uint32_t
 */
inline uint32_t
read_ui32(const u_char* data) noexcept
{
	uint32_t value = 0;

	for (int i = 0; i < 4; i++) {
		value = data[i] << (i * 8);
	}

	return value;
}

/**
 * \brief Read from a byte array an unsigned integer with 64 bit
 *
 * \param data byte array first position
 * \return uint64_t
 */
inline uint64_t
read_ui64(const u_char* data) noexcept
{
	uint64_t value = 0;

	for (int i = 0; i < 8; i++) {
		value = data[i] << (i * 8);
	}

	return value;
}

/**
 * \brief Read from a byte array a variable length data atleast unsigned integer
 * with 64 bit
 *
 * \param data byte array first position
 * \return bytes that have been read
 */
inline int
read_uivar(const u_char* data, uint64_t& dest, int bytes_left) noexcept
{
	dest = 0;
	for (int i = 0; i < bytes_left; i++) {
		dest |= (data[i] & 127) << (7 * i);
		if (!(data[i] & 128))
			return i + 1;
	}

	return 0;
}

/**
 * \brief Read from a byte array a variable length data atleast unsigned integer
 * with 32 bit
 *
 * \param data byte array first position
 * \return bytes that have been read
 */
inline int
read_uivar(const u_char* data, uint32_t& dest, int bytes_left) noexcept
{
	dest = 0;
	for (int i = 0; i < bytes_left; i++) {
		dest |= (data[i] & 127) << (7 * i);
		if (!(data[i] & 128))
			return i + 1;
	}

	return 0;
}

/**
 * \brief Write to a byte array an unsigned integer with 8 bit
 *
 * \param data byte array first position
 * \return 1
 */
inline int
write_ui8(u_char* data, uint8_t value) noexcept
{
	data[0] = value;
	return 1;
}

/**
 * \brief Write to a byte array a variable length data
 *
 * \param data byte array first position
 * \return byte written
 */
inline int
write_uivar(u_char* data, uint64_t value) noexcept
{
	int i = 0;
	while (value > 127) {
		data[i++] = (((uint8_t)(value & 127)) | 128);
		value >>= 7;
	}
	data[i++] = (((uint8_t)value) & 127);
	return i;
}

/**
 * \brief Read a deckstring page to get Deck name
 *
 * \param deckstring string with Deck page
 * \param name buffer string where store name
 * \param maxlen lenght of the buffer name
 * \return EXE_INVALID_ARGS if deckstring or name is nullptr
 * \return EXE_OVERFLOW if cannot store the name in the buffer
 * \return EXE_NOT_FOUND if name is not found
 * \return EXE_OK if success
 */
execode read_deckstring_page_name(const char* deckstring,
				  char* name,
				  int maxlen = DEF_BUFFER_STR) noexcept;
/**
 * \brief Read a deckstring
 *
 * \param deckstring string with base64 coded deckstring
 * \param hero_id a memory where store the hero id
 * \param format a memory where store the format
 * \param dest a buffer array where store the DeckCard list
 * \param maxlen the length of the buffer array dest
 * \return EXE_INVALID_ARGS if deckstring is nullptr
 * \return EXE_OK if success
 */
execode read_deckstring(const char* deckstring,
			uint32_t& hero_id,
			format_type& format,
			std::vector<DeckCard>& dest) noexcept;

/**
 * \brief Read a deckstring and place in Deck object
 *
 * \param deckstring string with base64 coded deckstring
 * \param dest deck object
 * \return EXE_INVALID_ARGS if deckstring is nullptr
 * \return EXE_OK if success
 */
execode read_deckstring(const char* deckstring, Deck& dest) noexcept;
/**
 * \brief Read a deckstring page and place in Deck object
 *
 * \param deckstring string with base64 coded deckstring
 * \param dest deck object
 * \return EXE_INVALID_ARGS if deckstring is nullptr
 * \return EXE_OK if success
 */
execode read_deckstring_page(const char* deckstring, Deck& dest) noexcept;

/**
 * \brief Write a deckstring page from a Deck object
 *
 * \param src source deck object
 * \param dest destination string
 * \return EXE_INVALID_ARGS if deckstring is nullptr
 * \return EXE_OK if success
 */
execode write_deckstring_page(const Deck& src, std::string& dest) noexcept;
/**
 * \brief Write a deckstring from a Deck object
 *
 * \param src source deck object
 * \param deckstring buffer string where place the deckstring page
 * \param maxlen length of the string buffer
 * \param dest destination string
 * \return EXE_INVALID_ARGS if deckstring is nullptr
 * \return EXE_OK if success
 */
execode write_deckstring(const Deck& src, std::string& dest) noexcept;
/**
 * \brief Write a deckstring from a Deck object
 *
 * \param src source DeckCard array
 * \param hero_id deck with hero id
 * \param format deck format
 * \param dest destination string
 * \return EXE_OK if success
 */
execode write_deckstring(const std::vector<DeckCard>& src,
			 uint32_t hero_id,
			 format_type format,
			 std::string& dest) noexcept;

}    // namespace mtk

#endif