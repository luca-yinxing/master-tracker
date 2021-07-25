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

set (src_hsdata
    conv.cpp
    card.cpp
    playcard.cpp
    cardmap.cpp
    deck.cpp
    deckcard.cpp
    deckstring.cpp
    player.cpp
)

set (header_hsdata
    conv.hpp
    card.hpp
    playcard.hpp
    cardmap.hpp
    deck.hpp
    deckcard.hpp
    deckstring.hpp
    player.hpp
    types.hpp
)

add_library(master_tracker_hsdata SHARED ${src_hsdata})

set_target_properties(master_tracker_hsdata PROPERTIES ENABLE_EXPORTS ON)
set_target_properties(master_tracker_hsdata PROPERTIES PUBLIC_HEADER "${header_hsdata}")
set_target_properties(master_tracker_hsdata PROPERTIES VERSION "0.1")
set_target_properties(master_tracker_hsdata PROPERTIES SOVERSION "0")

install(TARGETS master_tracker_hsdata
    PUBLIC_HEADER
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mtk/hsdata
)