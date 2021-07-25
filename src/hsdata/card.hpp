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

#ifndef HSDATA_CARD_H
#define HSDATA_CARD_H

//! \file

#include "types.hpp"
#include "utils/utils.hpp"

#include <string>
#include <vector>

namespace mtk
{
constexpr const int DEF_CARD_CLASSES  = 8;
constexpr const int DEF_CARD_MECHANIC = 16;
constexpr const int DEF_CARD_TAG      = 16;

/**
 * \brief Data structure for Hearthstone cards
 *
 */
class Card
{
      private:
	/**
	 * \brief unique id in Hearthstone database
	 *
	 */
	uint32_t m_id = 0;
	/**
	 * \brief unique string code in Hearthstone database
	 *
	 */
	std::string m_code;
	/**
	 * \brief localized string name of the Card
	 *
	 */
	std::string m_name;
	/**
	 * \brief Card rarity enum
	 *
	 */
	card_rarity m_rarity = INVALID_RARITY;
	/**
	 * \brief Card type enum
	 *
	 */
	card_type m_type = INVALID_TYPE;
	/**
	 * \brief array of Card class (mainly for multiclass Card)
	 *
	 */
	std::vector<card_class> m_classes;
	/**
	 * \brief Card race enum
	 *
	 */
	card_race m_race = INVALID_RACE;
	/**
	 * \brief Card set enum
	 *
	 */
	card_set m_set = INVALID_SET;
	/**
	 * \brief Card spellschool enum
	 *
	 */
	card_spellschool m_spellschool = INVALID_SPELLSCHOOL;
	/**
	 * \brief Card faction enum (not really used in hearthstone gameplay)
	 *
	 */
	card_faction m_faction = INVALID_FACTION;
	/**
	 * \brief Card mana cost
	 *
	 */
	int m_cost = 0;
	/**
	 * \brief is this Card collectible or not
	 *
	 */
	bool m_collectible = false;
	/**
	 * \brief is this Card elite (maybe in some case legendary != elite idk)
	 *
	 */
	bool m_elite = false;

	/**
	 * \brief array with all Card mechanic for gameplay purpose
	 *
	 */
	std::vector<card_mechanic> m_mechanic;
	/**
	 * \brief localized string text of the Card
	 *
	 */
	std::string m_text;
	/**
	 * \brief string for artist name
	 *
	 */
	std::string m_artist;
	/**
	 * \brief localized string for Card flavor
	 *
	 */
	std::string m_flavor;
	/**
	 * \brief attack point of the Card if has this property else is 0
	 *
	 */
	int m_attack = 0;
	/**
	 * \brief health point of the Card if has this property else is 0
	 *
	 */
	int m_health = 0;
	/**
	 * \brief overload point of the Card if has this property else is 0
	 *
	 */
	int m_overload = 0;
	/**
	 * \brief durability point of the Card if has this property else is 0
	 *
	 */
	int m_durability = 0;
	/**
	 * \brief spelldamage point of the Card if has this property else is 0
	 *
	 */
	int m_spelldamage = 0;
	/**
	 * \brief armor point of the Card if has this property else is 0
	 *
	 */
	int m_armor = 0;
	/**
	 * \brief array with all Card tag for gameplay purpose (not clear the
	 * diff from mechanics)
	 *
	 */
	std::vector<card_tag> m_tag;
	/**
	 * \brief questreward string of the Card if has this property is another
	 * Card code else is nullptr
	 *
	 */
	std::string m_questreward;

      public:
	/**
	 * \brief Construct a new Card object
	 *
	 */
	Card() noexcept = default;
	Card(uint32_t id) noexcept : m_id(id) { }
	Card(const std::string& code) noexcept : m_code(code) { }
	Card(uint32_t id, const std::string& code) noexcept :
	    m_id(id), m_code(code)
	{
	}
	/**
	 * \brief Construct a new Card object
	 *
	 * \param cpy copy Card object
	 */
	Card(const Card& cpy) noexcept = default;
	/**
	 * \brief Destroy the Card object
	 *
	 */
	~Card() noexcept = default;

	/**
	 * \brief Free code memory
	 *
	 */
	void clear_code() noexcept { m_code.clear(); }
	/**
	 * \brief Free name memory
	 *
	 */
	void clear_name() noexcept { m_name.clear(); }
	/**
	 * \brief Free classes memory
	 *
	 */
	void clear_classes() noexcept { m_classes.clear(); }
	/**
	 * \brief Free text memory
	 *
	 */
	void clear_text() noexcept { m_text.clear(); }
	/**
	 * \brief Free artist memory
	 *
	 */
	void clear_artist() noexcept { m_artist.clear(); }
	/**
	 * \brief Free flavor memory
	 *
	 */
	void clear_flavor() noexcept { m_flavor.clear(); }
	/**
	 * \brief Free mechanic memory
	 *
	 */
	void clear_mechanic() noexcept { m_mechanic.clear(); }
	/**
	 * \brief Free tag memory
	 *
	 */
	void clear_tag() noexcept { m_tag.clear(); }
	/**
	 * \brief Free questreward memory
	 *
	 */
	void clear_questreward() noexcept { m_questreward.clear(); }

	/**
	 * \brief Free all
	 *
	 */
	void clear() noexcept { *this = Card(); }

	/**
	 * \brief Get the id
	 *
	 * \return uint32_t
	 */
	uint32_t id() const noexcept { return m_id; }
	/**
	 * \brief Get the code
	 *
	 * \return const std::string&
	 */
	const std::string& code() const noexcept { return m_code; }
	/**
	 * \brief Get the name
	 *
	 * \return const std::string&
	 */
	const std::string& name() const noexcept { return m_name; }
	/**
	 * \brief Get the rarity
	 *
	 * \return card_rarity
	 */
	card_rarity rarity() const noexcept { return m_rarity; }
	/**
	 * \brief Get the type
	 *
	 * \return card_type
	 */
	card_type type() const noexcept { return m_type; }
	/**
	 * \brief Get the classes
	 *
	 * \return const std::vector<card_class>&
	 */
	const std::vector<card_class>& classes() const noexcept
	{
		return m_classes;
	}
	/**
	 * \brief Get the race
	 *
	 * \return card_race
	 */
	card_race race() const noexcept { return m_race; }
	/**
	 * \brief Get the set
	 *
	 * \return card_set
	 */
	card_set set() const noexcept { return m_set; }
	/**
	 * \brief Get the spellschool
	 *
	 * \return card_spellschool
	 */
	card_spellschool spellschool() const noexcept { return m_spellschool; }
	/**
	 * \brief Get the faction
	 *
	 * \return card_faction
	 */
	card_faction faction() const noexcept { return m_faction; }
	/**
	 * \brief Get the cost
	 *
	 * \return int
	 */
	int cost() const noexcept { return m_cost; }
	/**
	 * \brief Get the collectable
	 *
	 * \return true
	 * \return false
	 */
	bool collectible() const noexcept { return m_collectible; }
	/**
	 * \brief Get the elite
	 *
	 * \return true
	 * \return false
	 */
	bool elite() const noexcept { return m_elite; }
	/**
	 * \brief Get the mechanic
	 *
	 * \return const std::vector<card_mechanic>&
	 */
	const std::vector<card_mechanic>& mechanic() const noexcept
	{
		return m_mechanic;
	}

	/**
	 * \brief Get the text
	 *
	 * \return const std::string&
	 */
	const std::string& text() const noexcept { return m_text; }
	/**
	 * \brief Get the artist
	 *
	 * \return const std::string&
	 */
	const std::string& artist() const noexcept { return m_artist; }
	/**
	 * \brief Get the flavor
	 *
	 * \return const std::string&
	 */
	const std::string& flavor() const noexcept { return m_flavor; }
	/**
	 * \brief Get the attack
	 *
	 * \return int
	 */
	int attack() const noexcept { return m_attack; }
	/**
	 * \brief Get the health
	 *
	 * \return int
	 */
	int health() const noexcept { return m_health; }
	/**
	 * \brief Get the overload
	 *
	 * \return int
	 */
	int overload() const noexcept { return m_overload; }
	/**
	 * \brief Get the durability
	 *
	 * \return int
	 */
	int durability() const noexcept { return m_durability; }
	/**
	 * \brief Get the spelldamage
	 *
	 * \return int
	 */
	int spelldamage() const noexcept { return m_spelldamage; }
	/**
	 * \brief Get the armor
	 *
	 * \return int
	 */
	int armor() const noexcept { return m_armor; }
	/**
	 * \brief Get the tag
	 *
	 * \return const std::vector<card_tag>&
	 */
	const std::vector<card_tag>& tag() const noexcept { return m_tag; }

	/**
	 * \brief Get the questreward
	 *
	 * \return const std::string
	 */
	const std::string& questreward() const noexcept
	{
		return m_questreward;
	}
	/**
	 * \brief Set the id
	 *
	 * \param v
	 */
	void id(uint32_t v) noexcept { m_id = v; }
	/**
	 * \brief Set the code
	 *
	 * \param v
	 */
	void code(const std::string& v) noexcept { m_code = v; }
	/**
	 * \brief Set the name
	 *
	 * \param v
	 */
	void name(const std::string& v) noexcept { m_name = v; }
	/**
	 * \brief Set the rarity
	 *
	 * \param v
	 */
	void rarity(card_rarity v) noexcept { m_rarity = v; }
	/**
	 * \brief Set the type
	 *
	 * \param v
	 */
	void type(card_type v) noexcept { m_type = v; }

	/**
	 * \brief Set the classes
	 *
	 * \param v
	 */
	void classes(const std::vector<card_class>& v) noexcept
	{
		m_classes = v;
	}
	/**
	 * \brief Set the classes
	 *
	 * \param v
	 */
	void classes(card_class v) noexcept
	{
		m_classes.clear();
		m_classes.push_back(v);
	}
	/**
	 * \brief Set the race
	 *
	 * \param v
	 */
	void race(card_race v) noexcept { m_race = v; }
	/**
	 * \brief Set the set
	 *
	 * \param v
	 */
	void set(card_set v) noexcept { m_set = v; }
	/**
	 * \brief Set the spellschool
	 *
	 * \param v
	 */
	void spellschool(card_spellschool v) noexcept { m_spellschool = v; }
	/**
	 * \brief Set the faction
	 *
	 * \param v
	 */
	void faction(card_faction v) noexcept { m_faction = v; }
	/**
	 * \brief Set the cost
	 *
	 * \param v
	 */
	void cost(int v) noexcept { m_cost = v; }
	/**
	 * \brief Set the collectible
	 *
	 * \param v
	 */
	void collectible(bool v) noexcept { m_collectible = v; }
	/**
	 * \brief Set the elite
	 *
	 * \param v
	 */
	void elite(bool v) noexcept { m_elite = v; }
	/**
	 * \brief Set the mechanic
	 *
	 * \param v
	 */
	void mechanic(const std::vector<card_mechanic>& v) noexcept
	{
		m_mechanic = v;
	}
	/**
	 * \brief Set the text
	 *
	 * \param v
	 */
	void text(const std::string& v) noexcept { m_text = v; }
	/**
	 * \brief Set the artist
	 *
	 * \param v
	 */
	void artist(const std::string& v) noexcept { m_artist = v; }
	/**
	 * \brief Set the flavor
	 *
	 * \param v
	 */
	void flavor(const std::string& v) noexcept { m_flavor = v; }
	/**
	 * \brief Set the attack
	 *
	 * \param v
	 */
	void attack(int v) noexcept { m_attack = v; }
	/**
	 * \brief Set the health
	 *
	 * \param v
	 */
	void health(int v) noexcept { m_health = v; }
	/**
	 * \brief Set the overload
	 *
	 * \param v
	 */
	void overload(int v) noexcept { m_overload = v; }
	/**
	 * \brief Set the durability
	 *
	 * \param v
	 */
	void durability(int v) noexcept { m_durability = v; }
	/**
	 * \brief Set the spelldamage
	 *
	 * \param v
	 */
	void spelldamage(int v) noexcept { m_spelldamage = v; }
	/**
	 * \brief Set the armor
	 *
	 * \param v
	 */
	void armor(int v) noexcept { m_armor = v; }
	/**
	 * \brief Set the tag
	 *
	 * \param v
	 */
	void tag(const std::vector<card_tag>& v) noexcept { m_tag = v; }
	/**
	 * \brief Set the questreward
	 *
	 * \param v
	 */
	void questreward(const std::string v) noexcept { m_questreward = v; }

	void add_classes(card_class v) noexcept { m_classes.push_back(v); }
	void add_mechanic(card_mechanic v) noexcept { m_mechanic.push_back(v); }
	void add_tag(card_tag v) noexcept { m_tag.push_back(v); }

	card_class get_main_class() const noexcept
	{
		if (m_classes.size() > 0)
			return m_classes[0];

		return INVALID_CLASS;
	}

	/**
	 * \brief Compare Card properties by fields
	 *
	 * \param a first Card object
	 * \param b second Card object
	 * \param fields list of fields
	 * \param partial_str search string by partial substring or strict equal
	 * \return uint64_t list of the field equal found
	 */
	uint64_t
	compare(const Card& c, uint64_t fields, bool substr = false) noexcept;
};

}    // namespace mtk

#endif
