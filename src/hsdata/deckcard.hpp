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

#ifndef MT_HSDATA_DECKCARD_H
#define MT_HSDATA_DECKCARD_H

#include "card.hpp"

#include <memory>

namespace mtk
{
/**
 * \brief Data structure for DeckCard
 *
 */
struct DeckCard {
	/**
	 * \brief unique id of the Card from Hearthstone Database
	 *
	 */
	uint32_t m_id = 0;
	/**
	 * \brief pointer to Card information if present
	 *
	 */
	std::shared_ptr<Card> m_ptr;
	/**
	 * \brief how many copies of the Card are in the deck
	 *
	 */
	uint32_t m_count = 0;

	std::string m_code;

      public:
	DeckCard() noexcept = default;
	DeckCard(uint32_t id, uint32_t count) noexcept :
	    m_id(id), m_count(count)
	{
	}
	DeckCard(const DeckCard& cpy) noexcept = default;
	~DeckCard() noexcept		       = default;

	void clear() { *this = DeckCard(); }

	uint32_t id() const noexcept { return m_id; }

	std::shared_ptr<Card> ptr() const noexcept { return m_ptr; }

	uint32_t count() const noexcept { return m_count; }

	const std::string& code() const noexcept { return m_code; }

	void id(uint32_t v) noexcept { m_id = v; }

	void ptr(const std::shared_ptr<Card>& v) noexcept { m_ptr = v; }

	void count(uint32_t v) noexcept { m_count = v; }

	void code(const std::string& v) { m_code = v; }

	// char* created_by = nullptr;
	// bool outsider = false;
};

}    // namespace mtk

#endif