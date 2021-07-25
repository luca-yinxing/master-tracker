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
find_package(CURL REQUIRED)


set (src_hsjson
    jsoncard.cpp
    jsondl.cpp
)

set (header_hsjson
    jsoncard.hpp
    jsondl.hpp
    locale.hpp
)

add_library(master_tracker_hsjson SHARED ${src_hsjson})

set_target_properties(master_tracker_hsjson PROPERTIES ENABLE_EXPORTS ON)
set_target_properties(master_tracker_hsjson PROPERTIES PUBLIC_HEADER "${header_render}")
set_target_properties(master_tracker_hsjson PROPERTIES VERSION "0.1")
set_target_properties(master_tracker_hsjson PROPERTIES SOVERSION "0")

target_link_libraries(master_tracker_hsjson
    Qt5::Core
    master_tracker_hsdata
    ${CURL_LIBRARIES}
)

install(TARGETS master_tracker_hsjson
    PUBLIC_HEADER
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mtk/hsjson
)