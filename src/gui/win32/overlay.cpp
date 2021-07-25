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

#include "../overlay.hpp"

#include <windows.h>

namespace mtk
{
execode
get_window_geometry(const char* winname, Rect* retval)
{
	if (winname == nullptr || retval == nullptr) {
		return EXE_INVALID_ARGS;
	}

	HWND target_window;

	target_window = FindWindowA(winname, 0);
	if (target_window == nullptr) {
		return EXE_ERROR;
	}

	RECT winrect;
	if (!GetWindowRect(target_window, &winrect)) {
		return EXE_ERROR;
	}

	retval->x = winrect.left;
	retval->y = winrect.top;
	retval->w = winrect.right - winrect.left;
	retval->h = winrect.bottom - winrect.top;

	return EXE_OK;
}

}    // namespace mtk