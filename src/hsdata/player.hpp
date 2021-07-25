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

#ifndef MT_HSDATA_PLAYER_H
#define MT_HSDATA_PLAYER_H

#include "deck.hpp"
#include "playcard.hpp"
#include "types.hpp"

#include <utils/utils.hpp>

namespace mtk
{
constexpr const int FORMAT_TYPE_NUM = 3;

constexpr const int HAND_SIZE_NUM  = 10;
constexpr const int BOARD_SIZE_NUM = 7;

struct MedalInfo {
	int league_id	    = -1;
	int season_id	    = -1;
	int legend	    = -1;
	int star	    = -1;
	int star_best	    = -1;
	int star_earned	    = -1;
	int winstreak	    = -1;
	int season_games    = -1;
	int season_wins	    = -1;
	int star_xwin	    = -1;
	int league_bestever = -1;
	int star_bestever   = -1;
	format_type format  = INVALID_FORMAT;
};

struct PlayerInfo {
	int id		 = -1;
	player_side side = INVALID_SIDE;
	int cardback	 = -1;
	uint64_t bnet_hi = 0;
	uint64_t bnet_lo = 0;
	char16_t* name	 = nullptr;

	MedalInfo medal[FORMAT_TYPE_NUM];
};

inline execode
set_player_info_name(PlayerInfo& dest, const char16_t* name) noexcept
{
	if (name == nullptr) {
		return EXE_INVALID_ARGS;
	}
	dest.name = new_string16_copy(name);

	if (dest.name == nullptr) {
		return EXE_ERROR;
	}

	return EXE_OK;
}

inline execode
set_player_info_name(PlayerInfo& dest, const char* name) noexcept
{
	if (name == nullptr) {
		return EXE_INVALID_ARGS;
	}

	const size_t size = (strlen(name) + 1) * sizeof(char16_t);
	dest.name	  = new_string16(size);

	if (dest.name == nullptr) {
		return EXE_ERROR;
	}

	return to_utf16(name, dest.name, size);
}

inline execode
set_player_info(PlayerInfo& obj) noexcept
{
	obj.id	     = -1;
	obj.side     = INVALID_SIDE;
	obj.cardback = -1;
	obj.bnet_hi  = 0;
	obj.bnet_lo  = 0;
	obj.name     = nullptr;

	return EXE_OK;
}

inline execode
set_player_info(const PlayerInfo& src, PlayerInfo& dest) noexcept
{
	dest.id	      = src.id;
	dest.side     = src.side;
	dest.cardback = src.cardback;
	dest.bnet_hi  = src.bnet_hi;
	dest.bnet_lo  = src.bnet_lo;
	set_player_info_name(dest, src.name);

	return EXE_OK;
}

inline execode
free_player_info(PlayerInfo& c)
{
	free(c.name);

	set_player_info(c);

	return EXE_OK;
}

class Player
{
	using sptr_playcard = std::shared_ptr<PlayCard>;
	using uptr_playcard = std::unique_ptr<PlayCard>;

      private:
	int m_id			   = -1;
	std::shared_ptr<PlayerInfo> m_info = nullptr;
	std::shared_ptr<Deck> m_pdeck	   = nullptr;

	std::vector<std::shared_ptr<PlayCard>> m_cards;

      public:
	Player() noexcept		   = default;
	Player(const Player& cpy) noexcept = default;
	~Player() noexcept		   = default;

	void clear() noexcept { *this = Player(); }

	int id() const noexcept { return m_id; }
	std::shared_ptr<PlayerInfo> info() const noexcept { return m_info; }
	std::shared_ptr<Deck> pdeck() const noexcept { return m_pdeck; }

	const std::vector<std::shared_ptr<PlayCard>>& cards() const noexcept
	{
		return m_cards;
	}

	std::vector<std::shared_ptr<PlayCard>>& cards() noexcept
	{
		return m_cards;
	}

	void id(int v) noexcept { m_id = v; }
	void info(const std::shared_ptr<PlayerInfo>& v) noexcept { m_info = v; }
	void pdeck(const std::shared_ptr<Deck>& v) noexcept { m_pdeck = v; }

	void pdeck(const Deck& v) noexcept
	{
		m_pdeck = std::make_shared<Deck>(v);
	}

	void cards(const std::vector<std::shared_ptr<PlayCard>>& v) noexcept
	{
		m_cards = v;
	}

	void cards_add(const std::vector<std::shared_ptr<PlayCard>>& v) noexcept
	{
		m_cards.insert(m_cards.end(), v.begin(), v.end());
	}

	void cards_add(const PlayCard& v) noexcept
	{
		m_cards.push_back(std::make_shared<PlayCard>(v));
	}

	std::shared_ptr<PlayCard>& cards_at(uint32_t id) noexcept
	{
		for (std::shared_ptr<PlayCard>& c : m_cards) {
			if (c->id() == id) {
				return c;
			}
		}
	}

	void cards_replace(const PlayCard& tmpcard, uint32_t id) noexcept
	{
		for (std::shared_ptr<PlayCard>& c : m_cards) {
			if (c->id() == id) {
				c = std::make_shared<PlayCard>(tmpcard);
			}
		}
	}

	void cards_change_zone(card_zone zone, uint32_t id) noexcept
	{
		for (std::shared_ptr<PlayCard>& c : m_cards) {
			if (c->id() == id) {
				c->zone(zone);
			}
		}
	}
};
/*
inline PlayCard*
get_player_card(const player* obj, int id) noexcept
{
	if (obj == nullptr || obj->cards == nullptr) {
		return nullptr;
	}

	for (int i = 0; i < obj->cards_num; i++) {
		if (obj->cards[i].id == id) {
			return &obj->cards[i];
		}
	}

	return nullptr;
}
*/

/*
inline execode
set_player_cards(player* obj) noexcept
{
	if (obj == nullptr || obj->player_deck == nullptr) {
		return EXE_INVALID_ARGS;
	}

	int number     = get_deck_cards_number(obj->player_deck);
	obj->cards     = (PlayCard*)malloc(number * sizeof(PlayCard));
	obj->cards_num = 0;

	if (get_playcard_list(obj->player_deck, obj->cards, number) != number) {
		return EXE_OVERFLOW;
	}
	obj->cards_num = number;

	return EXE_OK;
}

*/
}    // namespace mtk

#endif