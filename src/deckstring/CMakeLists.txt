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

set (src_deckstring
    main.cpp
)

add_definitions(-DVERSION_DECKSTRING_APP="0.1")

add_executable (deckstring_app ${src_deckstring})

target_link_libraries(deckstring_app
    master_tracker_hsjson
    master_tracker_hsdata
    Qt5::Core
)

add_package(deckstring_app "" "")
