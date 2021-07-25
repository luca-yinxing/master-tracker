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

#include "../process.hpp"

#include <utils/utils.hpp>

#include <dirent.h>
#include <fstream>
#include <string>

namespace mtk::msp
{
int
get_pid(const char* name)
{
	int pid			      = -1;
	const int FILENAME_MAX_LENGTH = 32;

	if (name == nullptr)
		return -1;

	// Open the /proc directory
	DIR* dp = opendir("/proc");
	if (dp == nullptr) {
		return -1;
	}
	// Enumerate all entries in directory until process found
	struct dirent* dirp;
	while (pid < 0 && (dirp = readdir(dp))) {
		// Skip non-numeric entries
		int tmp_pid = atoi(dirp->d_name);
		if (tmp_pid <= 0) {
			continue;
		}
		// Read contents of virtual /proc/{pid}/cmdline file
		char cmdfile_str[FILENAME_MAX_LENGTH];
		snprintf(cmdfile_str,
			 FILENAME_MAX_LENGTH,
			 "/proc/%d/cmdline",
			 tmp_pid);

		std::ifstream cmdfile(cmdfile_str);
		std::string line;
		std::getline(cmdfile, line);

		if (!line.empty()) {
			// Keep first cmdline item which contains the program
			// path
			size_t pos = line.find('\0');
			if (pos != std::string::npos)
				line = line.substr(0, pos);
			// Keep program name only, removing the path
			pos = line.rfind('/');
			if (pos != std::string::npos)
				line = line.substr(pos + 1);
			// Compare against requested process name
			if (name == line) {
				pid = tmp_pid;
				break;
			}
		}
	}

	closedir(dp);

	return pid;
}

execode
find_module(int pid, moduleinfo* m_info, const char* m_name)
{
	const int FILENAME_MAX_LENGTH = 32;
	char mapfile_str[FILENAME_MAX_LENGTH];

	if (pid < 0)
		return EXE_INVALID_ARGS;
	if (m_info == nullptr)
		return EXE_INVALID_ARGS;
	if (m_name == nullptr)
		return EXE_INVALID_ARGS;
	if (m_info->address != ADDR_ZERO)
		return EXE_INVALID_ARGS;
	if (m_info->size != 0)
		return EXE_INVALID_ARGS;

	snprintf(mapfile_str, FILENAME_MAX_LENGTH, "/proc/%d/maps", pid);

	std::ifstream mapfile(mapfile_str);
	std::string line;

	while (std::getline(mapfile, line)) {
		if (line.find(m_name) != std::string::npos) {
			if (m_info->address == ADDR_ZERO) {
				m_info->address =
				    std::stoi(line.substr(0, 8), nullptr, 16);
			}
			m_info->size +=
			    std::stoi(line.substr(9, 8), nullptr, 16) -
			    m_info->address;
		}
	}

	mapfile.close();

	return EXE_OK;
}
}    // namespace mtk::msp