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

#include "../vmread.hpp"

#include <utils/utils.hpp>

#include <sys/uio.h>

namespace mtk::msp
{
int process_id = -1;

execode
read_data(void* buffer, uint32_t addr, uint32_t size, int pid) noexcept
{
	int unix_pid	= -1;
	void* unix_addr = nullptr;

	if (buffer == nullptr)
		return EXE_INVALID_ARGS;
	if (addr == ADDR_ZERO)
		return EXE_INVALID_ARGS;
	if (size == 0)
		return EXE_INVALID_ARGS;

	unix_addr = reinterpret_cast<void*>(addr);

	// Check the process id
	if (pid == -1) {
		if (process_id == -1)
			return EXE_INVALID_ARGS;
		else
			unix_pid = process_id;
	} else {
		unix_pid = pid;
	}

	struct iovec local[1]{ buffer, size };
	struct iovec remote[1]{ unix_addr, size };

	ssize_t res_size =
	    process_vm_readv(unix_pid,	  // Remote process id
			     local,	  // Local iovec array
			     1,		  // Size of the local iovec array
			     remote,	  // Remote iovec array
			     1,		  // Size of the remote iovec array
			     0);	  // Flags, unused

	// if res_size == -1 is failed
	// if res_size != _size is partial read, data might be corrupted
	if (res_size != (ssize_t)size) {
		return EXE_ERROR;
	}

	return EXE_OK;
}

}    // namespace mtk::msp