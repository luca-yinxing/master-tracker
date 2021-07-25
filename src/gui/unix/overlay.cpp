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

#include <X11/X.h>
#include <X11/Xlib.h>

namespace mtk
{
Window*
get_window_list(Display* disp, unsigned long* len)
{
	Atom prop = XInternAtom(disp, "_NET_CLIENT_LIST", False), type;
	int form;
	unsigned long remain;
	unsigned char* list;

	if (XGetWindowProperty(disp,
			       XDefaultRootWindow(disp),
			       prop,
			       0,
			       1024,
			       False,
			       33,
			       &type,
			       &form,
			       len,
			       &remain,
			       &list) != Success) {    // XA_WINDOW
		return 0;
	}

	return (Window*)list;
}

char*
get_window_name(Display* disp, Window win)
{
	Atom prop = XInternAtom(disp, "WM_NAME", False), type;
	int form;
	unsigned long remain, len;
	unsigned char* list;

	if (XGetWindowProperty(disp,
			       win,
			       prop,
			       0,
			       1024,
			       False,
			       AnyPropertyType,
			       &type,
			       &form,
			       &len,
			       &remain,
			       &list) != Success) {    // XA_STRING

		return nullptr;
	}

	return (char*)list;
}

execode
get_window_geometry(const char* winname, Rect* retval)
{
	if (winname == nullptr || retval == nullptr) {
		return EXE_INVALID_ARGS;
	}

	Display* disp = XOpenDisplay(nullptr);

	Window* list;
	char* name;
	unsigned long len;

	list = (Window*)get_window_list(disp, &len);

	for (int i = 0; i < (int)len; i++) {
		name = get_window_name(disp, list[i]);
		if (strcmp(name, winname) == 0) {
			int x = 0, y = 0;
			unsigned int w = 0, h = 0;
			unsigned int border_width_return = 0, depth_return = 0;
			Window root;
			XGetGeometry(disp,
				     list[i],
				     &root,
				     &x,
				     &y,
				     &w,
				     &h,
				     &border_width_return,
				     &depth_return);

			retval->x = x;
			retval->y = y;
			retval->w = w;
			retval->h = h;
		}
		free(name);
	}

	free(list);

	return EXE_OK;
}

}    // namespace mtk