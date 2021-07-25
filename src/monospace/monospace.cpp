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

#include "monospace.hpp"

#include "monofield.hpp"
#include "offset.hpp"
#include "vmread.hpp"

#include <utils/utils.hpp>

#include <cstring>

namespace mtk::msp
{
uint32_t
find_root_domain(const moduleinfo& mi)
{
	if (mi.address == ADDR_ZERO || mi.size == 0) {
		return 0;
	}
	// Return value of root domain address
	uint32_t root_domain_address = 0;

	uint8_t* mono_bytes = new uint8_t[mi.size];

	// Read all the memory of mono module
	read_data(mono_bytes, mi.address, mi.size);

	uint32_t start_index = to_ui32(mono_bytes, 0x3c);    // lfanew

	// Export directory address
	uint32_t export_directory_index = start_index + 0x78;
	// Export directory data
	uint32_t export_directory = to_ui32(mono_bytes, export_directory_index);

	// Number of functions in the module
	uint32_t n_function = to_ui32(mono_bytes, export_directory + 0x14);

	// Function address array index
	uint32_t function_address_array_index =
	    to_ui32(mono_bytes, export_directory + 0x1c);

	// Function name array index
	uint32_t function_name_array_index =
	    to_ui32(mono_bytes, export_directory + 0x20);

	for (uint32_t i = 0; i < (n_function * sizeof(uint32_t));
	     i += sizeof(uint32_t)) {
		uint32_t function_name_index =
		    to_ui32(mono_bytes, function_name_array_index + i);
		char* function_name =
		    reinterpret_cast<char*>(mono_bytes + function_name_index);
		if (strcmp(function_name, "mono_get_root_domain") == 0) {
			root_domain_address =
			    mi.address +
			    to_ui32(mono_bytes,
				    function_address_array_index + i);
			break;
		}
	}

	// free the memory
	delete[] mono_bytes;

	return root_domain_address;
}

uint32_t
find_assembly(uint32_t root_addr, const char* assembly)
{
	uint32_t assembly_address = 0;

	// Pointer to struct of type _MonoDomain
	uint32_t domain_address = read_ui32(root_addr + 1);
	// _MonoDomain first 4 bytes
	uint32_t domain = read_ui32(domain_address);
	// std::cout << "domain:" << domain << std::endl;

	// Pointer to array of structs of type _MonoAssembly
	uint32_t assembly_address_array =
	    read_ui32(domain + REFERENCED_ASSEMBLIES);

	for (uint32_t tmp_addr = assembly_address_array; tmp_addr != 0;
	     tmp_addr	       = read_ui32(tmp_addr + 0x4)) {
		// TODO: assembly_name_addr is just bad
		uint32_t assembly_name_addr = read_ui32(tmp_addr);
		char assembly_name[DEF_BUFFER_STR];
		read_string(assembly_name, read_ui32(assembly_name_addr + 0x8));
		if (strcmp(assembly_name, assembly) == 0) {
			assembly_address =
			    read_ui32(assembly_name_addr + ASSEMBLY_IMAGE);
			break;
		}
	}

	return assembly_address;
}

}    // namespace mtk::msp