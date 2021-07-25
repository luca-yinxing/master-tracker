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

#ifndef MT_HSMNG_LOGWORKER_H
#define MT_HSMNG_LOGWORKER_H

#include <QFile>
#include <QObject>

#include <utils/utils.hpp>

#include <hsdata/conv.hpp>

namespace mtk
{
constexpr size_t MTK_LOGWORKER_BUFFER_SIZE = 1024;

class LogWorker : public QObject
{
	Q_OBJECT
      public:
	LogWorker(QObject* parent = nullptr);
	~LogWorker();

	execode init(const std::string& path, log_component cmp);
	execode open();
	execode read_log();

	execode clear();

      private:
	static char buffer[MTK_LOGWORKER_BUFFER_SIZE];
	size_t seek;
	size_t numline;
	QFile* file;
	log_component component;

	int read_line(char* str, size_t sz);
	void reset();

      signals:
	void logReset();
	void new_line(log_component component,
		      QString line,
		      size_t numline,
		      size_t seek);
};

}    // namespace mtk

#endif	  // LOGWORKER_H
