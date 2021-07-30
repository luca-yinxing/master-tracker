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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Search on the system Qt5 components
find_package(Qt5 COMPONENTS Core Gui REQUIRED)

set (src_hsrender
    context.cpp
    scene.cpp
)

set (header_hsrender

    context.hpp
    render_utils.hpp
    renderobj.hpp
    scene.hpp
    scenecard.hpp
    scenecardtile.hpp

    obj/bannerrace.hpp
    obj/bannerspell.hpp
    obj/cardart.hpp
    obj/cardbannername.hpp
    obj/cardframe.hpp
    obj/cardname.hpp
    obj/cardtile.hpp
    obj/center_text.hpp
    obj/curved_text.hpp
    obj/elitedragon.hpp
    obj/gemattack.hpp
    obj/gemhealth.hpp
    obj/gemmana.hpp
    obj/gemrarity.hpp
    obj/linear_gradient.hpp
    obj/textblock.hpp
    obj/watermark.hpp

)

add_library(master_tracker_hsrender SHARED ${src_hsrender})

set_target_properties(master_tracker_hsrender PROPERTIES ENABLE_EXPORTS ON)
set_target_properties(master_tracker_hsrender PROPERTIES PUBLIC_HEADER "${header_hsrender}")
set_target_properties(master_tracker_hsrender PROPERTIES VERSION "0.1")
set_target_properties(master_tracker_hsrender PROPERTIES SOVERSION "0")

target_link_libraries(master_tracker_hsrender
    master_tracker_hsdata
    Qt5::Gui
    Qt5::Core
)

add_package(master_tracker_hsrender "/mtk/hsrender" "")