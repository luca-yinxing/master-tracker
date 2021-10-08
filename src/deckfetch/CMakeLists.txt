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

find_package(Qt5 COMPONENTS Core REQUIRED)

set (src_deckfetch
    main.cpp
)

add_definitions(-DVERSION_DECKFETCH_APP="0.1")

add_executable (deckfetch_app ${src_deckfetch})

target_link_libraries(deckfetch_app
    master_tracker_hsjson
    master_tracker_hsdata
    master_tracker_hsms
    Qt5::Core
)

add_package(deckfetch_app "" "")
