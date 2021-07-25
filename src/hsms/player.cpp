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

#include "player.hpp"

using namespace mtk::msp;
using namespace mtk;

namespace mtk
{
execode
init_hsms(uint32_t& assembly_addr) noexcept
{
	process_id = get_pid("Hearthstone.exe");

	if (process_id == -1) {
		return EXE_ERROR;
	}

	moduleinfo m_info;

	find_module(process_id, &m_info, "mono-2.0-bdwgc.dll");

	uint32_t root_domain_addr = find_root_domain(m_info);

	assembly_addr = find_assembly(root_domain_addr, "Assembly-CSharp");

	return EXE_OK;
}

execode
get_player_info(uint32_t assembly_addr,
		std::vector<std::shared_ptr<PlayerInfo>>& pinfo_list) noexcept
{
	if (assembly_addr == ADDR_ZERO) {
		return EXE_INVALID_ARGS;
	}

	pinfo_list.clear();

	monovalue value;
	find_class(assembly_addr, "GameState", value);

	monovalue instance;
	if (get_field_value(value, "s_instance", instance) != EXE_OK) {
		return EXE_ERROR;
	}

	monovalue playermap;
	get_field_value(instance, "m_playerMap", playermap);

	monovalue playerids;
	get_field_value(playermap, "keySlots", playerids);

	monovalue playerdata;
	get_field_value(playermap, "valueSlots", playerdata);

	monovalue pdata;
	monovalue pdata_name;
	monovalue pdata_account;
	monovalue pdata_medalinfo;
	monovalue pdata_medalinfo_curr;
	monovalue pdata_medalinfo_values;
	monovalue pdata_side_type;
	int pdata_side = 0;
	char16_t name_buffer[DEF_BUFFER_STR];

	// FIXME: can be more than two?
	const size_t length = 2;
	pinfo_list.resize(length);

	for (size_t i = 0; i < length; i++) {
		get_array_value(playerdata, i, pdata);
		get_array_value_int(playerids, i, pinfo_list[i]->id);
		get_field_value(pdata, "m_side", pdata_side_type);
		get_field_value_int(pdata_side_type, "value__", pdata_side);
		// TODO: Some check?
		pinfo_list[i]->side = (player_side)pdata_side;
		get_field_value_string16(pdata,
					 "m_name",
					 name_buffer,
					 DEF_BUFFER_STR);
		pinfo_list[i]->name = new_string16_copy(name_buffer);
		get_field_value_int(pdata,
				    "m_cardBackId",
				    pinfo_list[i]->cardback);
		get_field_value(pdata, "m_gameAccountId", pdata_account);
		get_field_value_ui64(pdata_account,
				     "m_hi",
				     pinfo_list[i]->bnet_hi);
		get_field_value_ui64(pdata_account,
				     "m_lo",
				     pinfo_list[i]->bnet_lo);

		get_field_value(pdata, "m_medalInfo", pdata_medalinfo);

		get_field_value(pdata_medalinfo,
				"m_currMedalInfo",
				pdata_medalinfo_curr);

		get_field_value(pdata_medalinfo_curr,
				"valueSlots",
				pdata_medalinfo_values);

		for (int f = 0; f < FORMAT_TYPE_NUM; f++) {
			monovalue tmp_medalinfo;
			get_array_value(pdata_medalinfo_values,
					i,
					tmp_medalinfo);
			get_field_value_int(tmp_medalinfo,
					    "leagueId",
					    pinfo_list[i]->medal[f].league_id);
			get_field_value_int(
			    tmp_medalinfo,
			    "earnedStars",
			    pinfo_list[i]->medal[f].star_earned);
			get_field_value_int(tmp_medalinfo,
					    "starLevel",
					    pinfo_list[i]->medal[f].star);
			get_field_value_int(tmp_medalinfo,
					    "bestStarLevel",
					    pinfo_list[i]->medal[f].star_best);
			get_field_value_int(tmp_medalinfo,
					    "winStreak",
					    pinfo_list[i]->medal[f].winstreak);
			get_field_value_int(tmp_medalinfo,
					    "legendIndex",
					    pinfo_list[i]->medal[f].legend);
			get_field_value_int(tmp_medalinfo,
					    "seasonId",
					    pinfo_list[i]->medal[f].season_id);
			get_field_value_int(
			    tmp_medalinfo,
			    "seasonWins",
			    pinfo_list[i]->medal[f].season_wins);
			get_field_value_int(
			    tmp_medalinfo,
			    "seasonGames",
			    pinfo_list[i]->medal[f].season_games);
			get_field_value_int(
			    tmp_medalinfo,
			    "bestEverLeagueId",
			    pinfo_list[i]->medal[f].league_bestever);
			get_field_value_int(
			    tmp_medalinfo,
			    "bestEverStarLevel",
			    pinfo_list[i]->medal[f].star_bestever);
			get_field_value_int(tmp_medalinfo,
					    "starsPerWin",
					    pinfo_list[i]->medal[f].star_xwin);

			monovalue tmp_medalinfo_format;
			get_field_value(pdata_medalinfo,
					"format",
					tmp_medalinfo_format);
			int format_id = 0;
			get_field_value_int(tmp_medalinfo_format,
					    "value__",
					    format_id);
			pinfo_list[i]->medal[f].format = (format_type)format_id;
		}
	}

	return EXE_OK;
}

execode
get_player_deck_pick_id(uint32_t assembly_addr, int64_t& deck_id) noexcept
{
	if (assembly_addr == ADDR_ZERO) {
		return EXE_INVALID_ARGS;
	}

	monovalue value;
	find_class(assembly_addr, "DeckPickerTrayDisplay", value);

	monovalue instance;
	if (get_field_value(value, "s_instance", instance) != EXE_OK) {
		return EXE_ERROR;
	}

	monovalue deckbox;
	get_field_value(instance, "m_selectedCustomDeckBox", deckbox);

	return get_field_value_i64(deckbox, "m_deckID", deck_id);
}

int
get_player_deck_list(uint32_t assembly_addr,
		     std::vector<std::shared_ptr<Deck>>& decklist) noexcept
{
	if (assembly_addr == ADDR_ZERO) {
		return -1;
	}

	int pushed_item = 0;

	monovalue value;
	find_class(assembly_addr, "CollectionManager", value);

	monovalue instance;
	if (get_field_value(value, "s_instance", instance) != EXE_OK) {
		return EXE_ERROR;
	}

	monovalue deckmap;
	get_field_value(instance, "m_decks", deckmap);

	monovalue deckdata;
	get_field_value(deckmap, "valueSlots", deckdata);

	int count = 0;
	get_field_value_int(deckmap, "count", count);
	char buffer[DEF_BUFFER_STR];
	char cardcode_buffer[DEF_BUFFER_STR];

	Card hero_card;
	Deck tmp_deck;
	int64_t tmp_id		 = 0;
	uint64_t tmp_create_date = 0;
	int tmp_format		 = 0;

	for (int i = 0; i < count; i++) {
		monovalue tmp;
		get_array_value(deckdata, i, tmp);

		get_field_value_string(tmp, "m_name", buffer, DEF_BUFFER_STR);

		tmp_deck.name(buffer);

		get_field_value_string(tmp,
				       "HeroCardID",
				       buffer,
				       DEF_BUFFER_STR);

		hero_card.code(buffer);

		tmp_deck.herocard(hero_card);

		get_field_value_i64(tmp, "ID", tmp_id);
		tmp_deck.id(tmp_id);

		get_field_value_ui64(tmp, "CreateDate", tmp_create_date);
		tmp_deck.create_date(tmp_create_date);

		monovalue tmp_mono_format;

		get_field_value(tmp,
				"<FormatType>k__BackingField",
				tmp_mono_format);
		// TODO: enum value? (value type)
		get_field_value_int(tmp_mono_format, "value__", tmp_format);

		if (tmp_format != WILD && tmp_format != STANDARD &&
		    tmp_format != CLASSIC) {
			tmp_format = INVALID_FORMAT;
		}

		tmp_deck.format((format_type)tmp_format);

		monovalue tmp_cardlist;
		get_field_value(tmp, "m_slots", tmp_cardlist);

		int tmp_cardlist_size = 0;
		get_field_value_int(tmp_cardlist, "_size", tmp_cardlist_size);

		monovalue tmp_cardlist_data;
		get_field_value(tmp_cardlist, "_items", tmp_cardlist_data);

		std::vector<DeckCard> buffer_deckcard_list;
		buffer_deckcard_list.resize(tmp_cardlist_size);

		for (int k = 0; k < tmp_cardlist_size; k++) {
			monovalue tmp_card;
			get_array_value(tmp_cardlist_data, k, tmp_card);

			get_field_value_string(tmp_card,
					       "m_cardId",
					       cardcode_buffer,
					       DEF_BUFFER_STR);

			buffer_deckcard_list[k].code(cardcode_buffer);

			monovalue tmp_card_count;
			get_field_value(tmp_card, "m_count", tmp_card_count);

			int tmp_card_count_size = 0;
			get_field_value_int(tmp_card_count,
					    "_size",
					    tmp_card_count_size);

			monovalue tmp_card_count_data;
			get_field_value(tmp_card_count,
					"_items",
					tmp_card_count_data);

			int value_card_count	 = 0;
			int value_card_count_sum = 0;

			for (int c = 0; c < tmp_card_count_size; c++) {
				get_array_value_int(tmp_card_count_data,
						    c,
						    value_card_count);

				value_card_count_sum += value_card_count;
			}

			buffer_deckcard_list[k].count(value_card_count_sum);
		}

		tmp_deck.cards(buffer_deckcard_list);
		decklist.push_back(std::make_shared<Deck>(tmp_deck));
		pushed_item++;
	}

	return pushed_item;
}
}    // namespace mtk