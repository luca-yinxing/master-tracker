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
find_package(Qt5 COMPONENTS Core Sql REQUIRED)

set (src_hsmng
    gamewatcher.cpp
    logworker.cpp
    gamemanager.cpp
    dbmanager.cpp
    settings.cpp
)

set (header_hsmng
    gamewatcher.hpp
    logworker.hpp
    gamemanager.hpp
    dbmanager.hpp
    settings.hpp
    constant.hpp
    sqlquery.hpp
)


add_definitions(-DMTK_HSBUILD="84593")

add_library(master_tracker_hsmng SHARED ${src_hsmng})

set_target_properties(master_tracker_hsmng PROPERTIES ENABLE_EXPORTS ON)
set_target_properties(master_tracker_hsmng PROPERTIES PUBLIC_HEADER "${header_hsmng}")
set_target_properties(master_tracker_hsmng PROPERTIES VERSION "0.1")
set_target_properties(master_tracker_hsmng PROPERTIES SOVERSION "0")

target_link_libraries(master_tracker_hsmng
    Qt5::Core
    Qt5::Sql
    master_tracker_hsdata
    master_tracker_hsjson
    master_tracker_hsms
)

add_package(master_tracker_hsmng "/mtk/hsmng" "")