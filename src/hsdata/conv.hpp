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

#ifndef HSDATA_CONV_H
#define HSDATA_CONV_H

#include "types.hpp"
#include <utils/utils.hpp>

#include <stdio.h>

namespace mtk
{
extern const format_type conv_array_format[];
extern const size_t conv_array_format_len;
extern const char* conv_array_format_string[];

extern const card_zone conv_array_zone[];
extern const size_t conv_array_zone_len;
extern const char* conv_array_zone_string[];

extern const language conv_array_language[];
extern const size_t conv_array_language_len;
extern const char* conv_array_language_string[];

extern const log_component conv_array_log_component[];
extern const size_t conv_array_log_component_len;
extern const char* conv_array_log_component_string[];

extern const screen_state conv_array_screen_state[];
extern const size_t conv_array_screen_state_len;
extern const char* conv_array_screen_state_string[];

extern const card_rarity conv_array_rarity[];
extern const size_t conv_array_rarity_len;
extern const char* conv_array_rarity_string[];
extern const unsigned char conv_array_rarity_color[][3];

extern const card_type conv_array_type[];
extern const size_t conv_array_type_len;
extern const char* conv_array_type_string[];

extern const card_race conv_array_race[];
extern const size_t conv_array_race_len;
extern const char* conv_array_race_string[];
extern const char* conv_array_race_string_en[];

extern const card_class conv_array_class[];
extern const size_t conv_array_class_len;
extern const char* conv_array_class_string[];
extern const unsigned char conv_array_class_color[][3];
extern const char* conv_array_class_string_en[];
extern const char* conv_array_class_string_number[];

extern const card_set conv_array_set[];
extern const size_t conv_array_set_len;
extern const char* conv_array_set_string[];

extern const card_mechanic conv_array_mechanic[];
extern const size_t conv_array_mechanic_len;
extern const char* conv_array_mechanic_string[];

extern const card_spellschool conv_array_spellschool[];
extern const size_t conv_array_spellschool_len;
extern const char* conv_array_spellschool_string[];
extern const char* conv_array_spellschool_string_en[];

extern const card_faction conv_array_faction[];
extern const size_t conv_array_faction_len;
extern const char* conv_array_faction_string[];

extern const card_tag conv_array_tag[];
extern const size_t conv_array_tag_len;
extern const char* conv_array_tag_string[];

/**
 * \brief Convert a string with language code in the enum
 *
 * \param str value of language string
 * \return language enum
 */
inline language
conv_string_language(const char* str) noexcept
{
	for (size_t i = 0; i < conv_array_language_len; i++) {
		if (strcmp(conv_array_language_string[i], str) == 0) {
			return conv_array_language[i];
		}
	}
	return INVALID_LANGUAGE;
}

inline language
conv_string_language(const std::string& str) noexcept
{
	return conv_string_language(str.c_str());
}

/**
 * \brief Convert a language enum into a string with language code
 *
 * \param v value of language enum
 * \return constant language string code
 */
inline const char*
conv_language_string(language v) noexcept
{
	for (size_t i = 0; i < conv_array_language_len; i++) {
		if (conv_array_language[i] == v) {
			return conv_array_language_string[i];
		}
	}
	return conv_array_language_string[0];
}

/**
 * \brief Convert a string with zone code in the enum
 *
 * \param str value of zone string
 * \return zone enum
 */
inline card_zone
conv_string_zone(const char* str) noexcept
{
	for (size_t i = 0; i < conv_array_zone_len; i++) {
		if (strcmp(conv_array_zone_string[i], str) == 0) {
			return conv_array_zone[i];
		}
	}
	return INVALID_ZONE;
}

inline card_zone
conv_string_zone(const std::string& str) noexcept
{
	return conv_string_zone(str.c_str());
}

/**
 * \brief Convert a zone enum into a string with zone code
 *
 * \param v value of zone enum
 * \return constant zone string code
 */
inline const char*
conv_zone_string(card_zone v) noexcept
{
	for (size_t i = 0; i < conv_array_zone_len; i++) {
		if (conv_array_zone[i] == v) {
			return conv_array_zone_string[i];
		}
	}
	return conv_array_zone_string[0];
}

/**
 * \brief Convert a log component enum into a string with log component code
 *
 * \param v value of log component enum
 * \return constant log component string code
 */
inline const char*
conv_log_component_string(log_component v) noexcept
{
	for (size_t i = 0; i < conv_array_log_component_len; i++) {
		if (conv_array_log_component[i] == v) {
			return conv_array_log_component_string[i];
		}
	}
	return conv_array_log_component_string[0];
}

/**
 * \brief Convert a screen state enum into a string with screen state
 *
 * \param v value of screen state enum
 * \return constant screen state string
 */
inline const char*
conv_screen_state_string(screen_state v) noexcept
{
	for (size_t i = 0; i < conv_array_screen_state_len; i++) {
		if (conv_array_screen_state[i] == v) {
			return conv_array_screen_state_string[i];
		}
	}
	return conv_array_screen_state_string[0];
}

/**
 * \brief Convert a screen state string into a screen state enum
 *
 * \param v string of screen state
 * \return constant screen state enum
 */
inline screen_state
conv_string_screen_state(const char* v) noexcept
{
	for (size_t i = 0; i < conv_array_screen_state_len; i++) {
		if (strcmp(conv_array_screen_state_string[i], v) == 0) {
			return conv_array_screen_state[i];
		}
	}
	return conv_array_screen_state[0];
}

inline screen_state
conv_string_screen_state(const std::string& str) noexcept
{
	return conv_string_screen_state(str.c_str());
}

/**
 * \brief Convert a format enum into a string with format code
 *
 * \param v value of format enum
 * \return constant format string
 */
inline const char*
conv_format_string(format_type v) noexcept
{
	for (size_t i = 0; i < conv_array_format_len; i++) {
		if (conv_array_format[i] == v) {
			return conv_array_format_string[i];
		}
	}
	return conv_array_format_string[0];
}

/**
 * \brief Convert a string with rarity code in the enum
 *
 * \param str value of rarity string
 * \return card_rarity enum
 */
inline card_rarity
conv_string_rarity(const char* str) noexcept
{
	for (size_t i = 0; i < conv_array_rarity_len; i++) {
		if (strcmp(conv_array_rarity_string[i], str) == 0) {
			return conv_array_rarity[i];
		}
	}
	return INVALID_RARITY;
}

inline card_rarity
conv_string_rarity(const std::string& str) noexcept
{
	return conv_string_rarity(str.c_str());
}

inline const char*
conv_rarity_string(card_rarity v) noexcept
{
	for (size_t i = 0; i < conv_array_rarity_len; i++) {
		if (conv_array_rarity[i] == v) {
			return conv_array_rarity_string[i];
		}
	}
	return conv_array_rarity_string[0];
}

inline const unsigned char*
conv_rarity_color(card_rarity v) noexcept
{
	for (size_t i = 0; i < conv_array_rarity_len; i++) {
		if (conv_array_rarity[i] == v) {
			return conv_array_rarity_color[i];
		}
	}
	return conv_array_rarity_color[0];
}

/**
 * \brief Convert a string with type code in the enum
 *
 * \param str value of type string
 * \return card_type enum
 */
inline card_type
conv_string_type(const char* str) noexcept
{
	for (size_t i = 0; i < conv_array_type_len; i++) {
		if (strcmp(conv_array_type_string[i], str) == 0) {
			return conv_array_type[i];
		}
	}
	return INVALID_TYPE;
}

inline card_type
conv_string_type(const std::string& str) noexcept
{
	return conv_string_type(str.c_str());
}

inline const char*
conv_type_string(card_type v) noexcept
{
	for (size_t i = 0; i < conv_array_type_len; i++) {
		if (conv_array_type[i] == v) {
			return conv_array_type_string[i];
		}
	}
	return conv_array_type_string[0];
}

/**
 * \brief Convert a string with race code in the enum
 *
 * \param str value of race string
 * \return card_race enum
 */
inline card_race
conv_string_race(const char* str) noexcept
{
	for (size_t i = 0; i < conv_array_race_len; i++) {
		if (strcmp(conv_array_race_string[i], str) == 0) {
			return conv_array_race[i];
		}
	}
	return INVALID_RACE;
}

inline card_race
conv_string_race(const std::string& str) noexcept
{
	return conv_string_race(str.c_str());
}

inline const char*
conv_race_string(card_race v) noexcept
{
	for (size_t i = 0; i < conv_array_race_len; i++) {
		if (conv_array_race[i] == v) {
			return conv_array_race_string[i];
		}
	}
	return conv_array_race_string[0];
}

inline const char*
conv_race_string_en(card_race v) noexcept
{
	for (size_t i = 0; i < conv_array_race_len; i++) {
		if (conv_array_race[i] == v) {
			return conv_array_race_string_en[i];
		}
	}
	return conv_array_race_string_en[0];
}

/**
 * \brief Convert a string with class code in the enum
 *
 * \param str value of class string
 * \return card_class enum
 */
inline card_class
conv_string_class(const char* str) noexcept
{
	for (size_t i = 0; i < conv_array_class_len; i++) {
		if (strcmp(conv_array_class_string[i], str) == 0) {
			return conv_array_class[i];
		}
	}
	return INVALID_CLASS;
}

inline card_class
conv_string_class(const std::string& str) noexcept
{
	return conv_string_class(str.c_str());
}

inline const char*
conv_class_string(card_class v) noexcept
{
	for (size_t i = 0; i < conv_array_class_len; i++) {
		if (conv_array_class[i] == v) {
			return conv_array_class_string[i];
		}
	}
	return conv_array_class_string[0];
}

inline const char*
conv_class_string_en(card_class v) noexcept
{
	for (size_t i = 0; i < conv_array_class_len; i++) {
		if (conv_array_class[i] == v) {
			return conv_array_class_string_en[i];
		}
	}
	return conv_array_class_string_en[0];
}

inline const unsigned char*
conv_class_color(card_class v) noexcept
{
	for (size_t i = 0; i < conv_array_class_len; i++) {
		if (conv_array_class[i] == v) {
			return conv_array_class_color[i];
		}
	}
	return conv_array_class_color[0];
}

inline card_class
conv_string_number_class(const char* str) noexcept
{
	for (size_t i = 0; i < conv_array_class_len; i++) {
		if (strcmp(conv_array_class_string_number[i], str) == 0) {
			return conv_array_class[i];
		}
	}
	return INVALID_CLASS;
}

inline card_class
conv_string_number_class(const std::string& str) noexcept
{
	return conv_string_number_class(str.c_str());
}

inline const char*
conv_class_string_number(card_class v) noexcept
{
	for (size_t i = 0; i < conv_array_class_len; i++) {
		if (conv_array_class[i] == v) {
			return conv_array_class_string_number[i];
		}
	}
	return conv_array_class_string_number[0];
}

/**
 * \brief Convert a string with set code in the enum
 *
 * \param str value of set string
 * \return card_set enum
 */
inline card_set
conv_string_set(const char* str) noexcept
{
	for (size_t i = 0; i < conv_array_set_len; i++) {
		if (strcmp(conv_array_set_string[i], str) == 0) {
			return conv_array_set[i];
		}
	}
	return INVALID_SET;
}

inline card_set
conv_string_set(const std::string& str) noexcept
{
	return conv_string_set(str.c_str());
}

inline const char*
conv_set_string(card_set v) noexcept
{
	for (size_t i = 0; i < conv_array_set_len; i++) {
		if (conv_array_set[i] == v) {
			return conv_array_set_string[i];
		}
	}
	return conv_array_set_string[0];
}

/**
 * \brief Convert a string with mechanic code in the enum
 *
 * \param str value of mechanic string
 * \return card_mechanic enum
 */
inline card_mechanic
conv_string_mechanic(const char* str) noexcept
{
	for (size_t i = 0; i < conv_array_mechanic_len; i++) {
		if (strcmp(conv_array_mechanic_string[i], str) == 0) {
			return conv_array_mechanic[i];
		}
	}
	return INVALID_MECHANIC;
}

inline card_mechanic
conv_string_mechanic(const std::string& str) noexcept
{
	return conv_string_mechanic(str.c_str());
}

inline const char*
conv_mechanic_string(card_mechanic v) noexcept
{
	for (size_t i = 0; i < conv_array_mechanic_len; i++) {
		if (conv_array_mechanic[i] == v) {
			return conv_array_mechanic_string[i];
		}
	}
	return conv_array_mechanic_string[0];
}

/**
 * \brief Convert a string with spellschool code in the enum
 *
 * \param str value of spellschool string
 * \return card_spellschool enum
 */
inline card_spellschool
conv_string_spellschool(const char* str) noexcept
{
	for (size_t i = 0; i < conv_array_spellschool_len; i++) {
		if (strcmp(conv_array_spellschool_string[i], str) == 0) {
			return conv_array_spellschool[i];
		}
	}
	return INVALID_SPELLSCHOOL;
}

inline card_spellschool
conv_string_spellschool(const std::string& str) noexcept
{
	return conv_string_spellschool(str.c_str());
}

inline const char*
conv_spellschool_string(card_spellschool v) noexcept
{
	for (size_t i = 0; i < conv_array_spellschool_len; i++) {
		if (conv_array_spellschool[i] == v) {
			return conv_array_spellschool_string[i];
		}
	}
	return conv_array_spellschool_string[0];
}

inline const char*
conv_spellschool_string_en(card_spellschool v) noexcept
{
	for (size_t i = 0; i < conv_array_spellschool_len; i++) {
		if (conv_array_spellschool[i] == v) {
			return conv_array_spellschool_string_en[i];
		}
	}
	return conv_array_spellschool_string[0];
}

/**
 * \brief Convert a string with faction code in the enum
 *
 * \param str value of faction string
 * \return card_faction enum
 */
inline card_faction
conv_string_faction(const char* str) noexcept
{
	for (size_t i = 0; i < conv_array_faction_len; i++) {
		if (strcmp(conv_array_faction_string[i], str) == 0) {
			return conv_array_faction[i];
		}
	}
	return INVALID_FACTION;
}

inline card_faction
conv_string_faction(const std::string& str) noexcept
{
	return conv_string_faction(str.c_str());
}

inline const char*
conv_faction_string(card_faction v) noexcept
{
	for (size_t i = 0; i < conv_array_faction_len; i++) {
		if (conv_array_faction[i] == v) {
			return conv_array_faction_string[i];
		}
	}
	return conv_array_faction_string[0];
}

/**
 * \brief Convert a string with tag code in the enum
 *
 * \param str value of tag string
 * \return card_tag enum
 */
inline card_tag
conv_string_tag(const char* str) noexcept
{
	for (size_t i = 0; i < conv_array_tag_len; i++) {
		if (strcmp(conv_array_tag_string[i], str) == 0) {
			return conv_array_tag[i];
		}
	}
	return INVALID_TAG;
}

inline card_tag
conv_string_tag(const std::string& str) noexcept
{
	return conv_string_tag(str.c_str());
}

inline const char*
conv_tag_string(card_tag v) noexcept
{
	for (size_t i = 0; i < conv_array_tag_len; i++) {
		if (conv_array_tag[i] == v) {
			return conv_array_tag_string[i];
		}
	}
	return conv_array_tag_string[0];
}

}    // namespace mtk

#endif