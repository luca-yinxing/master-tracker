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

# HSData Tests
### read_deckstring
add_executable(test_hsdata_read_deckstring read_deckstring.cpp)
target_link_libraries(test_hsdata_read_deckstring master_tracker_hsdata)
add_test(test_hsdata_read_deckstring test_hsdata_read_deckstring)
### read_deckstring_name
add_executable(test_hsdata_read_deckstring_name read_deckstring_name.cpp)
target_link_libraries(test_hsdata_read_deckstring_name master_tracker_hsdata)
add_test(test_hsdata_read_deckstring_name test_hsdata_read_deckstring_name)
### read_deckstring_page
add_executable(test_hsdata_read_deckstring_page read_deckstring_page.cpp)
target_link_libraries(test_hsdata_read_deckstring_page master_tracker_hsdata)
add_test(test_hsdata_read_deckstring_page test_hsdata_read_deckstring_page)
### write_deckstring
add_executable(test_hsdata_write_deckstring write_deckstring.cpp)
target_link_libraries(test_hsdata_write_deckstring master_tracker_hsdata)
add_test(test_hsdata_write_deckstring test_hsdata_write_deckstring)
### write_deckstring_page
add_executable(test_hsdata_write_deckstring_page write_deckstring_page.cpp)
target_link_libraries(test_hsdata_write_deckstring_page master_tracker_hsdata)
add_test(test_hsdata_write_deckstring_page test_hsdata_write_deckstring_page)
### sort_cost_less
add_executable(test_hsdata_sort_cost_less sort_cost_less.cpp)
target_link_libraries(test_hsdata_sort_cost_less master_tracker_hsdata)
add_test(test_hsdata_sort_cost_less test_hsdata_sort_cost_less)
### sort_cost_more
add_executable(test_hsdata_sort_cost_more sort_cost_more.cpp)
target_link_libraries(test_hsdata_sort_cost_more master_tracker_hsdata)
add_test(test_hsdata_sort_cost_more test_hsdata_sort_cost_more)
### sort_name_less
add_executable(test_hsdata_sort_name_less sort_name_less.cpp)
target_link_libraries(test_hsdata_sort_name_less master_tracker_hsdata)
add_test(test_hsdata_sort_name_less test_hsdata_sort_name_less)
### playcard_list
add_executable(test_hsdata_playcard_list playcard_list.cpp)
target_link_libraries(test_hsdata_playcard_list master_tracker_hsdata)
add_test(test_hsdata_playcard_list test_hsdata_playcard_list)