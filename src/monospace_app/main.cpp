// Copyright (C) 2021 Luca Gasperini
//
// This file is part of Master Tracker.
//
// Master Tracker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Master Tracker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Master Tracker.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>

#include <dirent.h>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <vector>

#include <fcntl.h>

#include <assert.h>
#include <cstring>

#include <unistdio.h>

#include <utils/utils.hpp>

#include <monospace/monoclass.hpp>
#include <monospace/monofield.hpp>
#include <monospace/monospace.hpp>
#include <monospace/monovalue.hpp>
#include <monospace/offset.hpp>
#include <monospace/process.hpp>
#include <monospace/vmread.hpp>

using namespace mtk::msp;
using namespace mtk;

int
main(int argc, char** argv)
{
	process_id = get_pid("Hearthstone.exe");

	if (process_id == -1) {
		printf("ERROR: Cannot find the process.\n");
		return 1;
	}

	moduleinfo m_info;

	find_module(process_id, &m_info, "mono-2.0-bdwgc.dll");

	uint32_t root_domain_addr = find_root_domain(m_info);

	uint32_t assembly_addr =
	    find_assembly(root_domain_addr, "Assembly-CSharp");

	char user_input[DEF_BUFFER_STR];
	char user_value[DEF_BUFFER_STR];
	char user_command[DEF_BUFFER_STR] = { '\0' };
	char class_path_buffer[DEF_BUFFER_STR];
	char class_path[256]	 = { '\0' };
	int class_path_bw	 = 0;
	monovalue* value_current = nullptr;
	monovalue value_list[64];
	int value_index = 0;

	while (strcmp(user_command, "quit") != 0 &&
	       strcmp(user_command, "exit")) {
		class_path_bw = 0;
		class_path[0] = '\0';

		for (int i = 0; i < value_index; i++) {
			get_class_name(get_class(value_list[i]),
				       class_path_buffer);
			if (value_index - 1 == i)
				class_path_bw +=
				    sprintf(class_path + class_path_bw,
					    "%s",
					    class_path_buffer);
			else
				class_path_bw +=
				    sprintf(class_path + class_path_bw,
					    "%s/",
					    class_path_buffer);
		}
		if (class_path_bw > 0) {
			printf("/%s$ ", class_path);
		} else {
			printf("/$ ");
		}
		fgets(user_input, DEF_BUFFER_STR, stdin);

		sscanf(user_input, "%s %s", user_command, user_value);

		if (strcmp(user_command, "in") == 0) {
			execode result_in;
			if (value_current == nullptr)
				result_in = find_class(assembly_addr,
						       user_value,
						       value_list[value_index]);
			else
				result_in =
				    get_field_value(*value_current,
						    user_value,
						    value_list[value_index]);

			if (result_in != EXE_OK) {
				printf(
				    "ERROR: Cannot find class or field \"%s\".\n",
				    user_value);
			} else {
				value_current = &(value_list[value_index]);
				value_index++;
			}
		}
		if (strcmp(user_command, "list") == 0) {
			if (value_current == nullptr) {
				uint32_t class_list[DEF_BUFFER_CLASSLIST];
				int count =
				    get_class_list(assembly_addr, class_list);
				char buffer_name[DEF_BUFFER_STR];
				for (int i = 0; i < count; i++) {
					get_class_name(class_list[i],
						       buffer_name);
					printf("%s\n", buffer_name);
				}
			} else {
				uint32_t field_list[DEF_BUFFER_FIELDLIST];
				int count =
				    get_field_list(*value_current, field_list);
				char buffer_name[DEF_BUFFER_STR];
				char buffer_value[DEF_BUFFER_STR];
				monovalue tmp = {};
				for (int i = 0; i < count; i++) {
					get_field_name(field_list[i],
						       buffer_name);
					if (get_field_value(*value_current,
							    field_list[i],
							    tmp) != EXE_OK) {
						printf("%s = error_get_value\n",
						       buffer_name);
					} else {
						get_value_string(tmp,
								 buffer_value);
						printf("%s = %s\n",
						       buffer_name,
						       buffer_value);
						clear_value(tmp);
					}
				}
			}
		}
		if (strcmp(user_command, "at") == 0 &&
		    value_current != nullptr) {
			get_array_value(*value_current,
					atoi(user_value),
					value_list[value_index]);
			value_current = &(value_list[value_index]);
			value_index++;
		}
		if (strcmp(user_command, "value") == 0 &&
		    value_current != nullptr) {
			char buffer[DEF_BUFFER_STR];
			get_value_string(*value_current, buffer);
			printf("%s\n", buffer);
		}

		if (strcmp(user_command, "back") == 0) {
			if (value_index > 0) {
				value_index--;
				if (value_index - 1 >= 0) {
					clear_value(*value_current);
					value_current =
					    &(value_list[value_index - 1]);
				} else {
					clear_value(*value_current);
					value_current = nullptr;
				}
			}
		}
		if (strcmp(user_command, "root") == 0) {
			for (int i = 0; i < value_index; i++) {
				clear_value(value_list[i]);
			}
			value_current = 0;
			value_index   = 0;
		}
	}

	return 0;
}