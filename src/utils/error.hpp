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

#ifndef UTILS_ERROR_H
#define UTILS_ERROR_H

//! \file

#include <ctime>
#include <stdarg.h>
#include <stdio.h>

namespace mtk
{
enum loglevel
{
	LOG_SILENT = 0,
	LOG_ERROR  = 1,
	LOG_INFO   = 2,
	LOG_DEBUG  = 3
};

enum logtime
{
	LOG_NOTIME	 = 0,
	LOG_TIME_ISO8601 = 1
};

inline void
print_log(loglevel lvl, logtime time_fmt, const char* fmt, ...)
{
	constexpr int STRING_TIME_LENGTH_MAX = 64;
	va_list list;

	if (lvl == LOG_SILENT)
		return;

	if (time_fmt != LOG_NOTIME) {
		time_t rawtime;
		time(&rawtime);
		struct tm* timeinfo = localtime(&rawtime);
		char buffer[STRING_TIME_LENGTH_MAX];

		if (time_fmt == LOG_TIME_ISO8601)
			strftime(buffer,
				 STRING_TIME_LENGTH_MAX,
				 "[%T]",
				 timeinfo);

		printf(buffer);
	}

	if (lvl == LOG_ERROR) {
		printf("[ERROR] ");
	}

	if (lvl == LOG_INFO) {
		printf("[INFO] ");
	}

	if (lvl == LOG_DEBUG) {
		printf("[DEBUG] ");
	}

	va_start(list, fmt);

	vprintf(fmt, list);
	// flush buffer too
	printf("\n");

	va_end(list);
}

}    // namespace mtk

#endif