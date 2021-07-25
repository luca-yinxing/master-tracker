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

#ifndef MT_HSDATA_PLAYCARD_H
#define MT_HSDATA_PLAYCARD_H

#include "card.hpp"
#include "deck.hpp"
#include "deckcard.hpp"

#include <memory>

namespace mtk
{
/**
 * \brief Data structure for PlayCard
 *
 */
class PlayCard
{
      private:
	/**
	 * \brief Card id of the game
	 *
	 */
	uint32_t m_id = 0;
	/**
	 * \brief pointer to Card information if present
	 *
	 */
	std::shared_ptr<Card> m_ptr;

	std::string m_code;

	int m_turn = -1;

	card_zone m_zone = INVALID_ZONE;

      public:
	PlayCard() noexcept		       = default;
	PlayCard(const PlayCard& cpy) noexcept = default;
	~PlayCard() noexcept		       = default;

	void clear() { *this = PlayCard(); }

	uint32_t id() const noexcept { return m_id; }

	std::shared_ptr<Card> ptr() const noexcept { return m_ptr; }

	int turn() const noexcept { return m_turn; }

	const std::string& code() const noexcept { return m_code; }

	card_zone zone() const noexcept { return m_zone; }

	void id(uint32_t v) noexcept { m_id = v; }

	void ptr(const std::shared_ptr<Card>& v) noexcept { m_ptr = v; }

	void turn(int v) noexcept { m_turn = v; }

	void code(const std::string& v) { m_code = v; }

	void zone(card_zone v) noexcept { m_zone = v; }

	// char* created_by = nullptr;
	// bool outsider = false;
};

/*
inline execode
set_playcard(PlayCard* c) noexcept
{
	if (c == nullptr) {
		return EXE_INVALID_ARGS;
	}

	c->id	      = -1;
	c->ptr	      = nullptr;
	c->code	      = nullptr;
	c->turn	      = -1;
	c->zone	      = INVALID_ZONE;
	c->created_by = false;

	return EXE_OK;
}

inline execode
set_playcard_code(PlayCard* dest, const char* code) noexcept
{
	if (dest == nullptr || code == nullptr) {
		// TODO: review all set object with a nullptr copy
		dest->code = nullptr;
		return EXE_INVALID_ARGS;
	}

	dest->code = new_string_copy(code);

	if (dest->code == nullptr) {
		return EXE_ERROR;
	}

	return EXE_OK;
}

inline execode
set_playcard(PlayCard* dest, const PlayCard* src) noexcept
{
	if (dest == nullptr || src == nullptr) {
		return EXE_INVALID_ARGS;
	}

	dest->id  = src->id;
	dest->ptr = src->ptr;
	set_playcard_code(dest, src->code);
	dest->turn	 = src->turn;
	dest->zone	 = src->zone;
	dest->created_by = src->created_by;

	return EXE_OK;
}

inline execode
set_playcard(PlayCard* dest, const deckcard* src, int id = -1) noexcept
{
	if (dest == nullptr || src == nullptr) {
		return EXE_INVALID_ARGS;
	}

	dest->id = id;
	set_playcard_code(dest, src->code);
	dest->ptr	 = src->ptr;
	dest->zone	 = DECK;
	dest->created_by = false;
}

inline execode
free_playcard(PlayCard* c) noexcept
{
	if (c == nullptr) {
		return EXE_INVALID_ARGS;
	}

	free(c->code);

	set_playcard(c);

	return EXE_OK;
}

inline int
get_playcard_list(deck* pdeck, PlayCard* list, int length) noexcept
{
	if (pdeck == nullptr || list == nullptr) {
		return -1;
	}

	int i = 0;

	for (int a = 0; a < pdeck->cards_num; a++) {
		for (uint32_t b = 0; b < pdeck->cards[a].count; b++) {
			if (length <= i) {
				return i;
			}
			set_playcard(&list[i++], &pdeck->cards[a]);
		}
	}

	return i;
}
*/
}    // namespace mtk

#endif