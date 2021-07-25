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

#include <monospace/monospace.hpp>
#include <monospace/process.hpp>
#include <monospace/vmread.hpp>

using namespace mtk;
using namespace mtk::msp;

int
main(int argc, char** argv)
{
	moduleinfo m_info;

	process_id = get_pid("Hearthstone.exe");

	if (process_id == -1)
		return 1;

	if (find_module(process_id, &m_info, "mono-2.0-bdwgc.dll") != EXE_OK)
		return 1;
	if (find_root_domain(m_info) == 0)
		return 1;

	return 0;
}