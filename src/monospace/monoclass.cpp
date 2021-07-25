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

#include "monoclass.hpp"

namespace mtk::msp
{
execode
get_class(uint32_t addr, monoclass& mc) noexcept
{
	if (addr == ADDR_ZERO) {
		return EXE_INVALID_ARGS;
	}

	mc.address = addr;
	mc.offset  = get_class_offset(addr);

	mc.name = new_string();
	get_class_name(addr, mc.name);
	mc.namespace_name = new_string();
	get_class_namespace(addr, mc.namespace_name);

	mc.size		  = get_class_size(addr);
	mc.runtime_info	  = get_class_runtimeinfo(addr);
	mc.vtable_address = mc.runtime_info == ADDR_ZERO ?
				      ADDR_ZERO :
				      get_class_vtable_addr(mc.runtime_info);
	mc.vtable_size	  = mc.runtime_info == ADDR_ZERO ?
				      ADDR_ZERO :
				      get_class_vtable_size(addr);

	mc.bitfield = get_class_bitfield(addr);

	mc.fieldcount = get_class_fieldcount(addr);
	// mc.fields = nullptr;

	mc.parent   = get_class_parent(addr);
	mc.nestedin = get_class_parent(addr);

	get_class_type(addr, mc.type);
	mc.kind = get_class_kind(addr);

	return EXE_OK;
}

int
get_class_list(uint32_t assembly_addr, uint32_t* class_list, int maxlen)
{
	if (assembly_addr == ADDR_ZERO)
		return -1;
	if (class_list == nullptr)
		return -1;

	int pushed_item		       = 0;
	constexpr uint32_t CLASS_CACHE = 0x354;

	uint32_t class_cache_size =
	    read_ui32(assembly_addr + CLASS_CACHE + HASH_TABLE_SIZE);
	uint32_t class_cache_table_array =
	    read_ui32(assembly_addr + CLASS_CACHE + HASH_TABLE_TABLE);

	for (uint32_t item = 0; item < (class_cache_size * sizeof(uint32_t));
	     item += sizeof(uint32_t)) {
		for (uint32_t addr = read_ui32(class_cache_table_array + item);
		     addr != 0;
		     addr = read_ui32(addr + CLASS_NEXT_CLASS_CACHE)) {
			if (pushed_item < maxlen)
				class_list[pushed_item++] = addr;
		}
	}

	return pushed_item;
}

uint32_t
find_class(uint32_t assembly_addr, const char* name) noexcept
{
	if (assembly_addr == ADDR_ZERO)
		return ADDR_ZERO;
	if (name == nullptr)
		return ADDR_ZERO;

	constexpr uint32_t CLASS_CACHE = 0x354;

	uint32_t class_cache_size =
	    read_ui32(assembly_addr + CLASS_CACHE + HASH_TABLE_SIZE);
	uint32_t class_cache_table_array =
	    read_ui32(assembly_addr + CLASS_CACHE + HASH_TABLE_TABLE);
	char buffer[DEF_BUFFER_STR];

	for (uint32_t item = 0; item < (class_cache_size * sizeof(uint32_t));
	     item += sizeof(uint32_t)) {
		for (uint32_t addr = read_ui32(class_cache_table_array + item);
		     addr != 0;
		     addr = read_ui32(addr + CLASS_NEXT_CLASS_CACHE)) {
			get_class_name(addr, buffer);
			if (strcmp(buffer, name) == 0) {
				return addr;
			}
		}
	}

	return ADDR_ZERO;
}

}    // namespace mtk::msp