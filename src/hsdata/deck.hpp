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

#ifndef HSDATA_DECK_H
#define HSDATA_DECK_H

#include "card.hpp"
#include "deckcard.hpp"
#include "utils/utils.hpp"

#include <memory>

//! \file

namespace mtk
{
constexpr int DEF_MAX_DECK_CARDS = 64;
constexpr int DEF_MAX_DECK_NUM	 = 27;

/**
 * \brief Data structure for Hearthstone Deck
 *
 */
class Deck
{
      private:
	int64_t m_id = -1;
	/**
	 * \brief Deck Card list
	 *
	 */
	std::vector<DeckCard> m_cards;
	/**
	 * \brief hero Card code from Hearthstone database
	 *
	 */
	std::shared_ptr<Card> m_herocard;

	/**
	 * \brief format of the Deck (Standard, Wild, Classic)
	 *
	 */
	format_type m_format = INVALID_FORMAT;
	/**
	 * \brief string with Deck name
	 *
	 */
	std::string m_name;
	/**
	 * \brief create date time
	 *
	 */
	uint64_t m_create_date = 0;

      public:
	Deck() noexcept		       = default;
	Deck(const Deck& cpy) noexcept = default;
	~Deck() noexcept	       = default;

	void clear() noexcept { *this = Deck(); }

	void clear_cards() noexcept { m_cards.clear(); }
	void clear_name() noexcept { m_name.clear(); }

	int64_t id() const noexcept { return m_id; }

	const std::vector<DeckCard>& cards() const noexcept { return m_cards; }

	std::vector<DeckCard>& cards() noexcept { return m_cards; }

	std::shared_ptr<Card> herocard() const noexcept { return m_herocard; }

	format_type format() const noexcept { return m_format; }

	const std::string& name() const noexcept { return m_name; }

	uint64_t create_date() const noexcept { return m_create_date; }

	void id(int64_t v) noexcept { m_id = v; }

	void cards(const std::vector<DeckCard>& v) noexcept { m_cards = v; }

	void herocard(const std::shared_ptr<Card>& v) noexcept
	{
		m_herocard = v;
	}

	void herocard(const Card& v) noexcept
	{
		m_herocard = std::make_shared<Card>(v);
	}

	void format(format_type v) noexcept { m_format = v; }

	void name(const std::string& v) noexcept { m_name = v; }

	void create_date(uint64_t v) noexcept { m_create_date = v; }

	card_class hero_class() const noexcept
	{
		return m_herocard->get_main_class();
	}

	void
	to_card_list(std::vector<std::shared_ptr<Card>>& card_list) noexcept;
};

}    // namespace mtk
#endif
