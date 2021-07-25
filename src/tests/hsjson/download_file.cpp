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

#include <hsjson/jsondl.hpp>

#include <stdio.h>

using namespace mtk;

int
main(int argc, char** argv)
{
	void* curl_handle = init_json_curl();

	if (download_json_cards(curl_handle, "/tmp/cards.json.part") !=
	    EXE_OK) {
		return 1;
	}

	free_json_curl(curl_handle);

	rename("/tmp/cards.json.part", "/tmp/cards.json");

	return 0;
}