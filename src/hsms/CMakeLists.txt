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


set (src_hsms
    player.cpp
)

set (header_hsms
    player.hpp
)


add_library(master_tracker_hsms SHARED ${src_hsms})

set_target_properties(master_tracker_hsms PROPERTIES ENABLE_EXPORTS ON)
set_target_properties(master_tracker_hsms PROPERTIES PUBLIC_HEADER "${header_hsms}")
set_target_properties(master_tracker_hsms PROPERTIES VERSION "0.1")
set_target_properties(master_tracker_hsms PROPERTIES SOVERSION "0")

target_link_libraries(master_tracker_hsms
    master_tracker_hsdata
    monospace
)

install(TARGETS master_tracker_hsms
    PUBLIC_HEADER
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mtk/hsms
)