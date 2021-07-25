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

#include "../process.h"

#include "../utils.h"

#include <oleacc.h>
#include <psapi.h>
#include <tlhelp32.h>

namespace mtk::msp
{
int
get_pid(const char* name)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, nullptr);

	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			if (strcmp(entry.szExeFile, name) == 0) {
				HANDLE win_pid =
				    OpenProcess(PROCESS_ALL_ACCESS,
						FALSE,
						entry.th32ProcessID);
				return reinterpret_cast<int>(win_pid);

				// TODO: CloseHandle at the end
				// CloseHandle(hProcess);
				// CloseHandle(snapshot);
			}
		}
	}
}

execode
find_module(int pid, moduleinfo* m_info, const char* m_name)
{
	const win_pid	   = nullptr;
	DWORD n_modules	   = 0;
	HMODULE* p_modules = new HMODULE[MODULE_LIST_MAX_LENGTH];

	char* filename = new char[FILENAME_MAX_LENGTH];

	if (pid < 0)
		return EXE_INVALID_ARGS;
	if (m_info == nullptr)
		return EXE_INVALID_ARGS;
	if (m_name == nullptr)
		return EXE_INVALID_ARGS;

	win_pid = reinterpret_cast<HANDLE>(pid);

	if (!EnumProcessModulesEx(win_pid,
				  p_modules,
				  MODULE_LIST_MAX_LENGTH,
				  &n_modules,
				  0x03)) {
		// TODO: How handle error?
		// std::cout << GetLastError() << std::endl;
	}

	for (int i = 0; i < n_modules; i++) {
		// TODO: How handle error?
		GetModuleFileNameExA(win_pid,
				     p_modules[i],
				     filename,
				     FILENAME_MAX_LENGTH);

		if (std::string(filename).find(m_name) != std::string::npos) {
			MODULEINFO tmp_module;
			// TODO: How handle error?
			GetModuleInformation(win_pid,
					     p_modules[i],
					     &tmp_module,
					     sizeof(MODULEINFO));

			m_info->address =
			    reinterpret_cast<uint32_t>(tmp_module.lpBaseOfDll);
			m_info.size = tmp_module.SizeOfImage;
			m_info.name = filename;

			break;
		}
	}

	delete[] p_modules;
	delete[] filename;

	return EXE_OK;
}
}    // namespace mtk::msp
