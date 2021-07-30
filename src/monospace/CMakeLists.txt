# Copyright (C) 2021 Luca Gasperini
#
# This file is part of Master Tracker.
#
# Master Tracker is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Master Tracker is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Master Tracker.  If not, see <http://www.gnu.org/licenses/>.

if (WIN32)
    set ( src_monospace_os
        win32/process.cpp
        win32/vmread.cpp
    )
endif()

if (UNIX)
    set ( src_monospace_os
        unix/process.cpp
        unix/vmread.cpp
    )
endif()


set (src_monospace
    monospace.cpp
    monoclass.cpp
    monofield.cpp
    monovalue.cpp
    ${src_monospace_os}
)

set (header_monospace
    monospace.hpp
    monoclass.hpp
    monofield.hpp
    monovalue.hpp
    monotype.hpp
    offset.hpp
    process.hpp
    vmread.hpp
)

add_library(monospace SHARED ${src_monospace})

set_target_properties(monospace PROPERTIES ENABLE_EXPORTS ON)
set_target_properties(monospace PROPERTIES PUBLIC_HEADER "${header_monospace}")
set_target_properties(monospace PROPERTIES VERSION "0.1")
set_target_properties(monospace PROPERTIES SOVERSION "0")


add_package(monospace "/mtk/monospace" "")