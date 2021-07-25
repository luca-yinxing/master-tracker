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

# Monospace Tests
### get_pid
add_executable(test_monospace_get_pid get_pid.cpp)
target_link_libraries(test_monospace_get_pid monospace)
add_test(test_monospace_get_pid test_monospace_get_pid)
### find_module
add_executable(test_monospace_find_module find_module.cpp)
target_link_libraries(test_monospace_find_module monospace)
add_test(test_monospace_find_module test_monospace_find_module)
### find_root_domain
add_executable(test_monospace_find_root_domain find_root_domain.cpp)
target_link_libraries(test_monospace_find_root_domain monospace)
add_test(test_monospace_find_root_domain test_monospace_find_root_domain)
### find_assembly
add_executable(test_monospace_find_assembly find_assembly.cpp)
target_link_libraries(test_monospace_find_assembly monospace)
add_test(test_monospace_find_assembly test_monospace_find_assembly)
### load_class_list
add_executable(test_monospace_load_class_list load_class_list.cpp)
target_link_libraries(test_monospace_load_class_list monospace)
add_test(test_monospace_load_class_list test_monospace_load_class_list)