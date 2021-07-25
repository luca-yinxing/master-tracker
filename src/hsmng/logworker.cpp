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

#include "logworker.hpp"
#include <QFileInfo>

namespace mtk
{
char LogWorker::buffer[MTK_LOGWORKER_BUFFER_SIZE];

LogWorker::LogWorker(QObject* parent) : QObject(parent)
{
	seek	  = 0;
	numline	  = 0;
	file	  = nullptr;
	component = INVALID_LOG;
}

execode
LogWorker::init(const std::string& path, log_component cmp)
{
	if (path.empty() || cmp == INVALID_LOG) {
		return EXE_INVALID_ARGS;
	}

	this->seek    = 0;
	this->numline = 0;

	const char* component_str = conv_log_component_string(cmp);
	const char* component_ext = ".log";

	std::string str_file =
	    path + STR_OS_DIR_SEP + component_str + component_ext;

	this->component = cmp;

	file = new QFile(str_file.c_str());

	return EXE_OK;
}

execode
LogWorker::open()
{
	if (file == nullptr) {
		return EXE_ERROR;
	}
	if (!file->isOpen()) {
		if (!file->exists()) {
			return EXE_NOT_FOUND;
		}
		if (!file->open(QIODevice::ReadOnly)) {
			return EXE_CANT_OPEN;
		}
		seek = QFileInfo(*file).size();
	}
	return EXE_OK;
}

LogWorker::~LogWorker()
{
	clear();
}

execode
LogWorker::clear()
{
	file->close();
	delete file;
	file = nullptr;

	return EXE_OK;
}

void
LogWorker::reset()
{
	this->seek    = 0;
	this->numline = 0;
}

int
LogWorker::read_line(char* str, size_t sz)
{
	int i = 0;
	while (i < sz) {
		if (!file->getChar(&str[i])) {
			return -1;
		}

		if (str[i++] == '\n') {
			return i;
		}
	}
	return sz + 1;
}

execode
LogWorker::read_log()
{
	file->seek(this->seek);

	int bytes_read = 0;

	while ((bytes_read = read_line(buffer, MTK_LOGWORKER_BUFFER_SIZE - 1)) >
	       0) {
		if (bytes_read == MTK_LOGWORKER_BUFFER_SIZE) {
			return EXE_OVERFLOW;
		}

		buffer[bytes_read] = 0;

		emit new_line(this->component,
			      buffer,
			      ++this->numline,
			      this->seek);
		this->seek += bytes_read;
	}

	return EXE_OK;
}

}    // namespace mtk