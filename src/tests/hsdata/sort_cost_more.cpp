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

#include <hsdata/deckcard.hpp>

#include <stdio.h>

using namespace mtk;

int
main(int argc, char** argv)
{
	/*
	deckcard card_list[18];
	card* card_array_ptr[18];
	int size = 18;
	for (int i = 0; i < size; i++) {
		card_array_ptr[i] = (card*)malloc(sizeof(card));
	}

	card_list[0].id		 = 56420;
	card_list[0].count	 = 1;
	card_array_ptr[0]->cost	 = 3;
	card_array_ptr[0]->name	 = "Dama Vashj";
	card_list[0].ptr	 = card_array_ptr[0];
	card_list[1].id		 = 58972;
	card_list[1].count	 = 1;
	card_array_ptr[1]->cost	 = 1;
	card_array_ptr[1]->name	 = "Iniziato Intrepido";
	card_list[1].ptr	 = card_array_ptr[1];
	card_list[2].id		 = 59164;
	card_list[2].count	 = 1;
	card_array_ptr[2]->cost	 = 3;
	card_array_ptr[2]->name	 = "Istruttrice Cuorfiammante";
	card_list[2].ptr	 = card_array_ptr[2];
	card_list[3].id		 = 59253;
	card_list[3].count	 = 1;
	card_array_ptr[3]->cost	 = 3;
	card_array_ptr[3]->name	 = "Oratrice Gidra";
	card_list[3].ptr	 = card_array_ptr[3];
	card_list[4].id		 = 63074;
	card_list[4].count	 = 1;
	card_array_ptr[4]->cost	 = 4;
	card_array_ptr[4]->name	 = "Bru'kan";
	card_list[4].ptr	 = card_array_ptr[4];
	card_list[5].id		 = 69655;
	card_list[5].count	 = 1;
	card_array_ptr[5]->cost	 = 2;
	card_array_ptr[5]->name	 = "Thalnos";
	card_list[5].ptr	 = card_array_ptr[5];
	card_list[6].id		 = 56411;
	card_list[6].count	 = 2;
	card_array_ptr[6]->cost	 = 3;
	card_array_ptr[6]->name	 = "Portale: Sacrespire";
	card_list[6].ptr	 = card_array_ptr[6];
	card_list[7].id		 = 56472;
	card_list[7].count	 = 2;
	card_array_ptr[7]->cost	 = 3;
	card_array_ptr[7]->name	 = "Apparizione della Palude";
	card_list[7].ptr	 = card_array_ptr[7];
	card_list[8].id		 = 58977;
	card_list[8].count	 = 2;
	card_array_ptr[8]->cost	 = 2;
	card_array_ptr[8]->name	 = "Studente Diligente";
	card_list[8].ptr	 = card_array_ptr[8];
	card_list[9].id		 = 59646;
	card_list[9].count	 = 2;
	card_array_ptr[9]->cost	 = 2;
	card_array_ptr[9]->name	 = "Bacchettaia";
	card_list[9].ptr	 = card_array_ptr[9];
	card_list[10].id	 = 60016;
	card_list[10].count	 = 2;
	card_array_ptr[10]->cost = 0;
	card_array_ptr[10]->name = "Fioritura Fulminea";
	card_list[10].ptr	 = card_array_ptr[10];
	card_list[11].id	 = 61224;
	card_list[11].count	 = 2;
	card_array_ptr[11]->cost = 3;
	card_array_ptr[11]->name = "Assalto della Tempesta";
	card_list[11].ptr	 = card_array_ptr[11];
	card_list[12].id	 = 61226;
	card_list[12].count	 = 2;
	card_array_ptr[12]->cost = 2;
	card_array_ptr[12]->name = "Custode dell'Arena";
	card_list[12].ptr	 = card_array_ptr[12];
	card_list[13].id	 = 63072;
	card_list[13].count	 = 2;
	card_array_ptr[13]->cost = 3;
	card_array_ptr[13]->name = "Fulminatore Arido";
	card_list[13].ptr	 = card_array_ptr[13];
	card_list[14].id	 = 63277;
	card_list[14].count	 = 2;
	card_array_ptr[14]->cost = 1;
	card_array_ptr[14]->name = "Elementale Fiammeggiante";
	card_list[14].ptr	 = card_array_ptr[14];
	card_list[15].id	 = 69517;
	card_list[15].count	 = 2;
	card_array_ptr[15]->cost = 2;
	card_array_ptr[15]->name = "Arma Roccia Dura";
	card_list[15].ptr	 = card_array_ptr[15];
	card_list[16].id	 = 69625;
	card_list[16].count	 = 2;
	card_array_ptr[16]->cost = 1;
	card_array_ptr[16]->name = "Dardo Fulminante";
	card_list[16].ptr	 = card_array_ptr[16];
	card_list[17].id	 = 69630;
	card_list[17].count	 = 2;
	card_array_ptr[17]->cost = 5;
	card_array_ptr[17]->name = "Martelfato";
	card_list[17].ptr	 = card_array_ptr[17];

	sort_card_list(card_array_ptr, size, COST, true);

	if (card_array_ptr[0]->cost != 5 || card_array_ptr[size - 1]->cost != 0)
		return 1;

	// for (int i = 0; i < size; i++) {
	//    printf("[%d] %s\n", card_list[i]->cost, card_list[i]->name);
	//}
*/
	return 0;
}