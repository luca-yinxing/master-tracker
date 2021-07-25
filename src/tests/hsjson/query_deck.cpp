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

#include <hsdata/deckstring.hpp>

#include <hsjson/jsoncard.hpp>

#include <stdio.h>

using namespace mtk;
char random_shaman_deck_string[] =
    "AAECAdL6AwbkuAPczAOczgP1zgPi7AOXoAQM27gDmLkD4cwD/tED8NQDqN4Dqt4D4OwDre4DjZ8E+Z8E/p8EAA==";

int
main(int argc, char** argv)
{
	Deck deck_shaman;
	read_deckstring(random_shaman_deck_string, deck_shaman);

	std::vector<std::shared_ptr<Card>> card_list;
	deck_shaman.to_card_list(card_list);

	CardMap carddb;

	CardQuery query;
	query.lang(IT_IT);
	query.select(NAME | COST);
	query.data(card_list);
	query.where(ID);

	get_json_carddb("/tmp/cards.json", query, carddb);

	if (carddb.size() != 18)
		return 1;

	return 0;
}