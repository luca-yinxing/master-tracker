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

}    // namespace mtk