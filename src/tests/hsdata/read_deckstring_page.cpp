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

#include <stdio.h>

using namespace mtk;

char random_shaman_deck_string[] =
    "### Mazzo Sciamano2\n"
    "# Classe: Sciamano\n"
    "# Formato: Standard\n"
    "# Anno del Grifone\n"
    "# \n"
    "# 2x (0) Fioritura Fulminea\n"
    "# 2x (1) Dardo Fulminante\n"
    "# 2x (1) Elementale Fiammeggiante\n"
    "# 1x (1) Iniziato Intrepido\n"
    "# 2x (2) Arma Roccia Dura\n"
    "# 2x (2) Bacchettaia\n"
    "# 2x (2) Custode dell'Arena\n"
    "# 2x (2) Studente Diligente\n"
    "# 1x (2) Thalnos\n"
    "# 2x (3) Apparizione della Palude\n"
    "# 2x (3) Assalto della Tempesta\n"
    "# 1x (3) Dama Vashj\n"
    "# 2x (3) Fulminatore Arido\n"
    "# 1x (3) Istruttrice Cuorfiammante\n"
    "# 1x (3) Oratrice Gidra\n"
    "# 2x (3) Portale: Sacrespire\n"
    "# 1x (4) Bru'kan\n"
    "# 2x (5) Martelfato\n"
    "# \n"
    "AAECAdL6AwbkuAPczAOczgP1zgPi7AOXoAQM27gDmLkD4cwD/tED8NQDqN4Dqt4D4OwDre4DjZ8E+Z8E/p8EAA==\n"
    "# \n"
    "# Per utilizzare questo mazzo, copialo negli appunti e crea un nuovo mazzo in Hearthstone";

int
main(int argc, char** argv)
{
	Deck deck_shaman;
	read_deckstring_page(random_shaman_deck_string, deck_shaman);

	if (deck_shaman.name() != "Mazzo Sciamano2") {
		return 1;
	}

	std::vector<DeckCard> card_list;
	card_list.push_back(DeckCard(56420, 1));
	card_list.push_back(DeckCard(58972, 1));
	card_list.push_back(DeckCard(59164, 1));
	card_list.push_back(DeckCard(59253, 1));
	card_list.push_back(DeckCard(63074, 1));
	card_list.push_back(DeckCard(69655, 1));
	card_list.push_back(DeckCard(56411, 2));
	card_list.push_back(DeckCard(56472, 2));
	card_list.push_back(DeckCard(58977, 2));
	card_list.push_back(DeckCard(59646, 2));
	card_list.push_back(DeckCard(60016, 2));
	card_list.push_back(DeckCard(61224, 2));
	card_list.push_back(DeckCard(61226, 2));
	card_list.push_back(DeckCard(63072, 2));
	card_list.push_back(DeckCard(63277, 2));
	card_list.push_back(DeckCard(69517, 2));
	card_list.push_back(DeckCard(69625, 2));
	card_list.push_back(DeckCard(69630, 2));

	if (card_list.size() != deck_shaman.cards().size()) {
		return 1;
	}

	size_t found_items = 0;

	for (const DeckCard& dc : deck_shaman.cards()) {
		for (DeckCard& cl : card_list) {
			if (dc.id() == cl.id() && dc.count() == cl.count()) {
				found_items++;
			}
		}
	}

	if (found_items != deck_shaman.cards().size()) {
		return 1;
	}

	return 0;
}