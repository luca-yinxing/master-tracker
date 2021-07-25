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

#include "card.hpp"

#include <algorithm>	// for sort function

namespace mtk
{
static inline bool
s_cmpstr(const std::string& a, const std::string& b, bool substr) noexcept
{
	if (substr) {
		return a.find(b) != std::string::npos;
	} else {
		return a.compare(b) == 0;
	}
}

// TODO: It's fine?
template <class t>
static inline bool
s_cmpvec(std::vector<t> a, std::vector<t> b) noexcept
{
	std::sort(a.begin(), a.end());
	std::sort(b.begin(), b.end());

	return std::includes(a.begin(), a.end(), b.begin(), b.end());
}

uint64_t
Card::compare(const Card& c, uint64_t fields, bool substr) noexcept
{
	uint64_t flags = 0;

	if (fields == 0) {
		return EXE_OK;
	}

	if (fields & ID) {
		if (m_id == c.m_id) {
			flags |= ID;
		}
	}

	if (fields & CODE) {
		flags |= s_cmpstr(m_code, c.m_code, false) ? CODE : 0;
	}

	if (fields & COST) {
		if (m_cost == c.m_cost) {
			flags |= COST;
		}
	}

	if (fields & NAME) {
		flags |= s_cmpstr(m_name, c.m_name, substr) ? NAME : 0;
	}

	if (fields & CLASSES) {
		flags |= s_cmpvec(m_classes, c.m_classes) ? CLASSES : 0;
	}

	if (fields & MECHANIC) {
		flags |= s_cmpvec(m_mechanic, c.m_mechanic) ? MECHANIC : 0;
	}

	if (fields & RARITY) {
		if (m_rarity == c.m_rarity) {
			flags |= RARITY;
		}
	}

	if (fields & TYPE) {
		if (m_type == c.m_type) {
			flags |= TYPE;
		}
	}

	if (fields & SET) {
		if (m_set == c.m_set) {
			flags |= SET;
		}
	}

	if (fields & RACE) {
		if (m_race == c.m_race) {
			flags |= RACE;
		}
	}

	if (fields & TEXT) {
		flags |= s_cmpstr(m_text, c.m_text, substr) ? TEXT : 0;
	}

	if (fields & FLAVOR) {
		flags |= s_cmpstr(m_flavor, c.m_flavor, substr) ? FLAVOR : 0;
	}

	if (fields & ARTIST) {
		flags |= s_cmpstr(m_artist, c.m_artist, substr) ? ARTIST : 0;
	}

	if (fields & SPELLSCHOOL) {
		if (m_spellschool == c.m_spellschool) {
			flags |= SPELLSCHOOL;
		}
	}

	if (fields & FACTION) {
		if (m_faction == c.m_faction) {
			flags |= FACTION;
		}
	}

	if (fields & COLLECTIBLE) {
		if (m_collectible == c.m_collectible) {
			flags |= COLLECTIBLE;
		}
	}

	if (fields & ELITE) {
		if (m_elite == c.m_elite) {
			flags |= ELITE;
		}
	}

	if (fields & HEALTH) {
		if (m_health == c.m_health) {
			flags |= HEALTH;
		}
	}

	if (fields & ATTACK) {
		if (m_attack == c.m_attack) {
			flags |= ATTACK;
		}
	}

	if (fields & OVERLOAD) {
		if (m_overload == c.m_overload) {
			flags |= OVERLOAD;
		}
	}

	if (fields & DURABILITY) {
		if (m_durability == c.m_durability) {
			flags |= DURABILITY;
		}
	}

	if (fields & SPELLDAMAGE) {
		if (m_spelldamage == c.m_spelldamage) {
			flags |= SPELLDAMAGE;
		}
	}

	if (fields & ARMOR) {
		if (m_armor == c.m_armor) {
			flags |= ARMOR;
		}
	}

	if (fields & TAG) {
		flags |= s_cmpvec(m_tag, c.m_tag) ? TAG : 0;
	}

	if (fields & QUESTREWARD) {
		flags |= s_cmpstr(m_questreward, c.m_questreward, false) ?
				   QUESTREWARD :
				   0;
	}

	return flags;
}

}    // namespace mtk