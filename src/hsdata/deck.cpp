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

#include "deck.hpp"

namespace mtk
{
void
Deck::to_card_list(std::vector<std::shared_ptr<Card>>& card_list) noexcept
{
	card_list.clear();
	card_list.reserve(m_cards.size());

	for (const DeckCard& c : m_cards) {
		card_list.push_back(c.ptr());
	}
}

void
Deck::complete_info(const CardMap& cardmap) noexcept
{
	for (DeckCard& c : m_cards) {
		c.ptr(cardmap.find_by_id(c.id()));
	}
}

static inline bool
sort_cost_asc(DeckCard& a, DeckCard& b) noexcept
{
	if (a.ptr() == nullptr || b.ptr() == nullptr) {
		return false;
	}
	if (a.ptr()->cost() == b.ptr()->cost()) {
		return a.ptr()->name().compare(b.ptr()->name()) < 0;
	}
	return a.ptr()->cost() < b.ptr()->cost();
}

static inline bool
sort_cost_desc(DeckCard& a, DeckCard& b) noexcept
{
	if (a.ptr() == nullptr || b.ptr() == nullptr) {
		return false;
	}
	if (a.ptr()->cost() == b.ptr()->cost()) {
		return a.ptr()->name().compare(b.ptr()->name()) < 0;
	}
	return a.ptr()->cost() > b.ptr()->cost();
}

void
Deck::sort_cost(bool asc) noexcept
{
	if (asc) {
		std::sort(m_cards.begin(), m_cards.end(), &sort_cost_asc);
	} else {
		std::sort(m_cards.begin(), m_cards.end(), &sort_cost_desc);
	}
}

}    // namespace mtk