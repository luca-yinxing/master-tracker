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

#ifndef MT_HSJSON_LOCALE_H
#define MT_HSJSON_LOCALE_H

#include <stdio.h>

#include <utils/utils.hpp>

#include <hsdata/conv.hpp>

namespace mtk
{
execode
get_locale_language(const std::string& path, language& lang) noexcept
{
	if (path.empty()) {
		return EXE_INVALID_ARGS;
	}

	std::string str_file = path + STR_OS_DIR_SEP + STR_URL_SEP + "text_";

	for (size_t i = 0; i < conv_array_language_len; i++) {
		if (file_exists(str_file + conv_array_language_string[i] +
				".cache")) {
			lang = conv_array_language[i];
			return EXE_OK;
		}
	}

	return EXE_NOT_FOUND;
}

}    // namespace mtk

#endif