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

#ifndef HSJSON_CARDMAP_H
#define HSJSON_CARDMAP_H

#include "card.hpp"

#include <map>
#include <memory>

namespace mtk
{
class CardMap
{
      private:
	std::map<uint32_t, std::shared_ptr<Card>> m_map;

      public:
	CardMap() noexcept		     = default;
	CardMap(const CardMap& cpy) noexcept = default;
	~CardMap() noexcept		     = default;

	void clear() noexcept { m_map.clear(); }

	// TODO: Add bytesize?
	size_t size() noexcept { return m_map.size(); }

	const std::map<uint32_t, std::shared_ptr<Card>>& map() const
	{
		return m_map;
	}

	std::map<uint32_t, std::shared_ptr<Card>>& map() { return m_map; }

	void map(const std::map<uint32_t, std::shared_ptr<Card>>& v)
	{
		m_map = v;
	}

	void add_card(uint32_t id, const std::shared_ptr<Card>& card) noexcept
	{
		m_map.insert({ id, card });
	}

	void to_codelist(std::vector<std::string>& codelist) noexcept;

	inline const std::shared_ptr<Card>
	find_by_code(const std::string& code) const noexcept;

	inline const std::shared_ptr<Card>
	find_by_name(const std::string& name) const noexcept;
};

inline const std::shared_ptr<Card>
CardMap::find_by_code(const std::string& code) const noexcept
{
	// TODO: What if code is multiple -> first element
	for (auto i = m_map.begin(); i != m_map.end(); i++) {
		if (i->second->code() == code) {
			return i->second;
		}
	}

	return nullptr;
}

inline const std::shared_ptr<Card>
CardMap::find_by_name(const std::string& name) const noexcept
{
	// TODO: What if code is multiple -> first element
	for (auto i = m_map.begin(); i != m_map.end(); i++) {
		if (i->second->name() == name) {
			return i->second;
		}
	}

	return nullptr;
}

}    // namespace mtk

#endif