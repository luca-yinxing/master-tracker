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

# HSJson Tests
### download_file
add_executable(test_hsjson_download_file download_file.cpp)
target_link_libraries(test_hsjson_download_file master_tracker_hsjson)
add_test(test_hsjson_download_file test_hsjson_download_file)
### download_image
add_executable(test_hsjson_download_image download_image.cpp)
target_link_libraries(test_hsjson_download_image master_tracker_hsjson)
add_test(test_hsjson_download_image test_hsjson_download_image)
### download_image_list
add_executable(test_hsjson_download_image_list download_image_list.cpp)
target_link_libraries(test_hsjson_download_image_list master_tracker_hsjson)
add_test(test_hsjson_download_image_list test_hsjson_download_image_list)
### download_tile
add_executable(test_hsjson_download_tile download_tile.cpp)
target_link_libraries(test_hsjson_download_tile master_tracker_hsjson)
add_test(test_hsjson_download_tile test_hsjson_download_tile)
### download_tile_list
add_executable(test_hsjson_download_tile_list download_tile_list.cpp)
target_link_libraries(test_hsjson_download_tile_list master_tracker_hsjson)
add_test(test_hsjson_download_tile_list test_hsjson_download_tile_list)
### read_file
add_executable(test_hsjson_read_file read_file.cpp)
target_link_libraries(test_hsjson_read_file master_tracker_hsjson)
add_test(test_hsjson_read_file test_hsjson_read_file)
### query_std_charge
add_executable(test_hsjson_query_std_charge query_std_charge.cpp)
target_link_libraries(test_hsjson_query_std_charge master_tracker_hsjson)
add_test(test_hsjson_query_std_charge test_hsjson_query_std_charge)
### query_all
add_executable(test_hsjson_query_all query_all.cpp)
target_link_libraries(test_hsjson_query_all master_tracker_hsjson)
add_test(test_hsjson_query_all test_hsjson_query_all)
### query_deck
add_executable(test_hsjson_query_deck query_deck.cpp)
target_link_libraries(test_hsjson_query_deck master_tracker_hsjson)
add_test(test_hsjson_query_deck test_hsjson_query_deck)