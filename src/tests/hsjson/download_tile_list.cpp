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
	void* curl_handle		  = init_json_curl();
	std::vector<std::string> cardlist = { "BT_187",
					      "CORE_EX1_414",
					      "CORE_EX1_543",
					      "CORE_NEW1_010",
					      "CS3_019" };

	if (download_json_image_tile(curl_handle,
				     "/tmp/hsdata_tile",
				     cardlist) != 5) {
		return 1;
	}

	free_json_curl(curl_handle);

	return 0;
}