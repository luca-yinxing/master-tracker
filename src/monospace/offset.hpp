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

#ifndef AT_MONO_OFFSET
#define AT_MONO_OFFSET

//! \file

#include <stdint.h>

namespace mtk::msp
{
constexpr const uint32_t ASSEMBLY_IMAGE = 0x44;

constexpr const uint32_t IMAGE_CLASS_CACHE = 0x354;

constexpr const uint32_t HASH_TABLE_SIZE = 0xc;

constexpr const uint32_t HASH_TABLE_TABLE = 0x14;

constexpr const uint32_t CLASS_BITFIELD = 0x14;

constexpr const uint32_t CLASS_BYVAL_ARG = 0x74;

constexpr const uint32_t CLASS_FIELDCOUNT = 0xa4;

constexpr const uint32_t CLASS_FIELDS = 0x60;

constexpr const uint32_t CLASS_FIELDSIZE = 0x10;

constexpr const uint32_t CLASS_NAME = 0x2c;

constexpr const uint32_t CLASS_NAMESPACE = 0x30;

constexpr const uint32_t CLASS_NESTEDIN = 0x24;

constexpr const uint32_t CLASS_NEXT_CLASS_CACHE = 0xa8;

constexpr const uint32_t CLASS_PARENT = 0x20;

constexpr const uint32_t CLASS_RUNTIME_INFO = 0x84;

constexpr const uint32_t CLASS_SIZE = 0x5c;

constexpr const uint32_t CLASS_RUNTIME_INFO_DOMAIN_VTABLE = 0x4;

constexpr const uint32_t REFERENCED_ASSEMBLIES = 0x6c;

constexpr const uint32_t CLASS_VTABLE_SIZE = 0x38;

constexpr const uint32_t CLASS_KIND = 0x1e;

constexpr const uint32_t CLASS_SIZEOF = 0x94;

constexpr const uint32_t VTABLE = 0x28;

constexpr const uint32_t VTABLE_DATA = 0xc;

constexpr const uint32_t CLASS_OFFSET = 0xc;
}    // namespace mtk::msp

#endif