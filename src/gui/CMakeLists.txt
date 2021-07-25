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

if (WIN32)
    set ( src_monospace_os
        win32/overlay.cpp
    )
endif()

if (UNIX)
    find_package(X11 REQUIRED)
    set ( src_monospace_os
        unix/overlay.cpp
    )
endif()

set (src_gui
    mainwindow.cpp
    playwindow.cpp
    deckwindow.cpp
    enemywindow.cpp
    widgets/deckwidget.cpp
    widgets/decklistwidget.cpp
    widgets/cardlistwidget.cpp
    widgets/carddetailwidget.cpp
    widgets/cardnameitem.cpp
    widgets/cardnumitem.cpp
    widgets/cardiconitem.cpp
    widgets/carddateitem.cpp
    widgets/filterwidget.cpp
    widgets/decknameitem.cpp
    ${src_monospace_os}
)

set (header_gui
    mainwindow.hpp
    playwindow.hpp
    deckwindow.hpp
    enemywindow.hpp
    widgets/deckwidget.hpp
    widgets/decklistwidget.hpp
    widgets/cardlistwidget.hpp
    widgets/carddetailwidget.hpp
    widgets/cardnameitem.hpp
    widgets/cardnumitem.hpp
    widgets/cardiconitem.hpp
    widgets/carddateitem.hpp
    widgets/filterwidget.hpp
    widgets/decknameitem.hpp
    overlay.hpp
    styleitemdelegate.hpp
)

set (src_uis_gui # ArenaTracker UIs file
    mainwindow.ui
    playwindow.ui
    deckwindow.ui
    enemywindow.ui
    carddetailwidget.ui
    deckwidget.ui
    decklistwidget.ui
    cardlistwidget.ui
)

QT5_WRAP_UI(uis_gui ${src_uis_gui})

include_directories(widgets/)

add_library (master_tracker_gui SHARED ${src_gui} ${uis_gui} ${rscs_gui})

set_target_properties(master_tracker_gui PROPERTIES ENABLE_EXPORTS ON)
set_target_properties(master_tracker_gui PROPERTIES PUBLIC_HEADER "${header_gui}")
set_target_properties(master_tracker_gui PROPERTIES VERSION "0.1")
set_target_properties(master_tracker_gui PROPERTIES SOVERSION "0")


target_link_libraries(master_tracker_gui # Add linked libraries
    Qt5::Gui
    Qt5::Widgets
    Qt5::Core
    master_tracker_hsmng
    master_tracker_hsrender
    ${X11_LIBRARIES}
)

install(TARGETS master_tracker_gui
    PUBLIC_HEADER
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mtk/gui
)