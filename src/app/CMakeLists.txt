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
find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

set (src_app
    main.cpp
)

add_executable (master_tracker_main_app ${src_app})

target_link_libraries(master_tracker_main_app
    master_tracker_gui
    Qt5::Gui
    Qt5::Widgets
    Qt5::Core
)

add_package(master_tracker_main_app "" "")