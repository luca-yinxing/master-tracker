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

#include <hsdata/playcard.hpp>

#include <stdio.h>

using namespace mtk;

char random_shaman_deck_string[] =
    "AAECAdL6AwbkuAPczAOczgP1zgPi7AOXoAQM27gDmLkD4cwD/tED8NQDqN4Dqt4D4OwDre4DjZ8E+Z8E/p8EAA==";
int
main(int argc, char** argv)
{
	// TODO: Re-enable and fix this test
	/*
	deck deck_shaman;
	set_deck(deck_shaman);
	if (read_deckstring(random_shaman_deck_string, deck_shaman) != EXE_OK) {
	    return 1;
	}

	int cards_num = deck_shaman.cards_num;
	if (cards_num != 30) {
	    return 1;
	}

	playcard cards[cards_num];

	if (get_playcard_list(&deck_shaman, cards, cards_num) != cards_num) {
	    return 1;
	}
    */
	return 0;
}