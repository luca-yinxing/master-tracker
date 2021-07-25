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

#ifndef MT_HSMS_PLAYER_H
#define MT_HSMS_PLAYER_H

#include <hsdata/player.hpp>

#include <monospace/monospace.hpp>
#include <utils/utils.hpp>

namespace mtk
{
execode init_hsms(uint32_t& assembly_addr) noexcept;

execode
get_player_info(uint32_t assembly_addr,
		std::vector<std::shared_ptr<PlayerInfo>>& pinfo_list) noexcept;

execode get_player_deck_pick_id(uint32_t assembly_addr,
				int64_t& deck_id) noexcept;

int get_player_deck_list(uint32_t assembly_addr,
			 std::vector<std::shared_ptr<Deck>>& decklist) noexcept;
}    // namespace mtk

#endif