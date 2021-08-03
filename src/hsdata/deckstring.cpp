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

#include "deckstring.hpp"

#include "conv.hpp"
#include <utils/b64.h>

#include <sstream>
#include <stdio.h>

namespace mtk
{
static execode
s_read_until_endline(const char* ptr, char* str, int maxlen)
{
	const char* delta = ptr;
	while (*delta != '\0' && *delta != '\n')
		delta++;

	int name_length = delta - ptr;
	if (maxlen < name_length)
		return EXE_OVERFLOW;

	strncpy(str, ptr, name_length);
	str[name_length] = '\0';
	return EXE_OK;
}

static execode
s_read_deckstring_page_name(const char* ptr, char* name, int maxlen) noexcept
{
	constexpr char start_deckname[]	 = "### ";
	constexpr int start_deckname_len = strlen(start_deckname);

	if (strncmp(start_deckname, ptr, start_deckname_len) == 0) {
		return s_read_until_endline(ptr + start_deckname_len,
					    name,
					    maxlen);
	}

	return EXE_NOT_FOUND;
}

execode
read_deckstring_page_name(const char* deckstring,
			  char* name,
			  int maxlen) noexcept
{
	if (deckstring == nullptr || name == nullptr) {
		return EXE_INVALID_ARGS;
	}

	bool first_char	  = true;
	bool first_inline = true;

	while (*deckstring != '\0') {
		first_inline = (first_char || *(deckstring - 1) == '\n');
		if (*deckstring == '#' && first_inline) {
			execode code = s_read_deckstring_page_name(deckstring,
								   name,
								   maxlen);
			if (code != EXE_NOT_FOUND) {
				return code;
			}
		}
		first_char = false;
		deckstring++;
	}

	return EXE_NOT_FOUND;
}

execode
read_deckstring(const char* deckstring,
		uint32_t& hero_id,
		format_type& format,
		std::vector<DeckCard>& dest) noexcept
{
	uint32_t tmp_id;
	uint32_t tmp_count;

	if (deckstring == nullptr) {
		return EXE_INVALID_ARGS;
	}

	char strbase64[DEF_BUFFER_STR];
	s_read_until_endline(deckstring, strbase64, DEF_BUFFER_STR);

	size_t decoded_size = 0;
	u_char* decoded_start =
	    b64_decode_ex(strbase64, strlen(strbase64), &decoded_size);

	u_char* tmp = decoded_start;

	if (read_ui8(tmp++) != 0x0) {
		return EXE_ERROR;
	}
	if (read_ui8(tmp++) != VERSION_DECKSTRING) {
		return EXE_ERROR;
	}

	format = (format_type)read_ui8(tmp++);

	uint64_t count_hero = 0;
	tmp +=
	    read_uivar(tmp, count_hero, decoded_size - (tmp - decoded_start));

	for (uint64_t i = 0; i < count_hero; i++) {
		tmp += read_uivar(tmp,
				  hero_id,
				  decoded_size - (tmp - decoded_start));
	}

	uint64_t count_cards1 = 0;
	tmp +=
	    read_uivar(tmp, count_cards1, decoded_size - (tmp - decoded_start));
	for (uint64_t i = 0; i < count_cards1; i++) {
		tmp += read_uivar(tmp,
				  tmp_id,
				  decoded_size - (tmp - decoded_start));
		dest.push_back(DeckCard(tmp_id, 1));
	}

	uint64_t count_cards2 = 0;
	tmp +=
	    read_uivar(tmp, count_cards2, decoded_size - (tmp - decoded_start));
	for (uint64_t i = 0; i < count_cards2; i++) {
		tmp += read_uivar(tmp,
				  tmp_id,
				  decoded_size - (tmp - decoded_start));
		dest.push_back(DeckCard(tmp_id, 2));
	}

	uint64_t count_cardsn = 0;
	tmp +=
	    read_uivar(tmp, count_cardsn, decoded_size - (tmp - decoded_start));
	for (uint64_t i = 0; i < count_cardsn; i++) {
		tmp += read_uivar(tmp,
				  tmp_id,
				  decoded_size - (tmp - decoded_start));
		tmp += read_uivar(tmp,
				  tmp_count,
				  decoded_size - (tmp - decoded_start));
		dest.push_back(DeckCard(tmp_id, 2));
	}

	return EXE_OK;
}

execode
read_deckstring(const char* deckstring, Deck& dest) noexcept
{
	if (deckstring == nullptr) {
		return EXE_INVALID_ARGS;
	}

	std::vector<DeckCard> buffer_card;
	format_type format = INVALID_FORMAT;
	uint32_t tmp_id	   = 0;

	read_deckstring(deckstring, tmp_id, format, buffer_card);
	dest.cards(buffer_card);
	dest.format(format);
	// TODO: it's fine?
	std::shared_ptr<Card> ptr_card = std::make_shared<Card>(tmp_id);
	dest.herocard(ptr_card);

	return EXE_OK;
}

execode
read_deckstring_page(const char* deckstring, Deck& dest) noexcept
{
	if (deckstring == nullptr) {
		return EXE_INVALID_ARGS;
	}
	char name_buffer[DEF_BUFFER_STR];

	bool first_char	  = true;
	bool first_inline = true;

	while (*deckstring != '\0') {
		first_inline = (first_char || *(deckstring - 1) == '\n');
		if (*deckstring == '#' && first_inline) {
			s_read_deckstring_page_name(deckstring,
						    name_buffer,
						    DEF_BUFFER_STR);

			dest.name(name_buffer);
		}
		if (*deckstring != '#' && first_inline) {
			return read_deckstring(deckstring, dest);
		}
		deckstring++;
		first_char = false;
	}
	return EXE_NOT_FOUND;
}

execode
write_deckstring_page(const Deck& src, std::string& dest) noexcept
{
	std::string code;

	execode result = write_deckstring(src, code);
	if (result != EXE_OK) {
		return result;
	}

	std::stringstream ss;

	ss << "### " << src.name() << "\n";
	ss << "# Class: " << conv_class_string_en(src.hero_class()) << "\n";
	ss << "# Format: " << conv_format_string(src.format()) << "\n";
	ss << "# \n";

	for (const DeckCard& c : src.cards()) {
		// TODO: card::is_valid function?
		if (c.ptr() != nullptr) {
			ss << "# " << c.count() << "x"
			   << "(" << c.ptr()->cost() << ") " << c.ptr()->name()
			   << "\n";
		}
	}

	ss << "# \n";
	ss << code << "\n";
	ss << "# \n";
	ss << "# To use this deck, copy it to your clipboard and create a new deck in Hearthstone\n";
	ss << "# Generated by Master Tracker - HSData\n";

	dest = ss.str();

	return EXE_OK;
}

execode
write_deckstring_csv(const Deck& src, std::string& dest) noexcept
{
	std::string code;

	execode result = write_deckstring(src, code);
	if (result != EXE_OK) {
		return result;
	}

	std::stringstream ss;

	ss << src.name() << "\n";
	ss << conv_class_string_en(src.hero_class()) << "\n";
	ss << conv_format_string(src.format()) << "\n";
	ss << code << "\n";

	for (const DeckCard& c : src.cards()) {
		// TODO: card::is_valid function?
		if (c.ptr() != nullptr) {
			ss << c.count() << "," << c.ptr()->cost() << ",\""
			   << c.ptr()->name() << "\"\n";
		}
	}

	dest = ss.str();

	return EXE_OK;
}

execode
write_deckstring(const Deck& src, std::string& dest) noexcept
{
	if (src.herocard() == nullptr) {
		return EXE_INVALID_ARGS;
	}

	return write_deckstring(src.cards(),
				src.herocard()->id(),
				src.format(),
				dest);
}

execode
write_deckstring(const std::vector<DeckCard>& src,
		 uint32_t hero_id,
		 format_type format,
		 std::string& dest) noexcept
{
	u_char base64_encoded[DEF_BUFFER_STR_LONG];

	u_char* tmp = base64_encoded;

	std::vector<DeckCard> cards1;
	std::vector<DeckCard> cards2;
	std::vector<DeckCard> cardsn;

	for (const DeckCard& card : src) {
		if (card.count() == 1) {
			cards1.push_back(card);
		} else if (card.count() == 2) {
			cards2.push_back(card);
		} else if (card.count() > 2) {
			cardsn.push_back(card);
		}
	}

	tmp += write_ui8(tmp, 0x0);
	tmp += write_uivar(tmp, VERSION_DECKSTRING);
	tmp += write_uivar(tmp, format);

	tmp += write_uivar(tmp, 1);
	tmp += write_uivar(tmp, hero_id);

	tmp += write_uivar(tmp, cards1.size());
	for (const DeckCard& card : cards1) {
		tmp += write_uivar(tmp, card.id());
	}

	tmp += write_uivar(tmp, cards2.size());
	for (const DeckCard& card : cards2) {
		tmp += write_uivar(tmp, card.id());
	}

	tmp += write_uivar(tmp, cardsn.size());
	for (const DeckCard& card : cards2) {
		tmp += write_uivar(tmp, card.id());
		tmp += write_uivar(tmp, card.count());
	}

	char* encoded_string = b64_encode(base64_encoded, tmp - base64_encoded);

	dest = encoded_string;
	free(encoded_string);

	return EXE_OK;
}

}    // namespace mtk