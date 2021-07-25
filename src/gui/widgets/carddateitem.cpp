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

#include "carddateitem.hpp"

#include <hsdata/conv.hpp>

#include <QDateTime>

using namespace mtk;

namespace mtk
{
void
CardDateItem::init(uint64_t since_epoch)
{
	setData(Qt::UserRole, QVariant((qulonglong)since_epoch));

	QDateTime create_date;
	create_date.setSecsSinceEpoch(since_epoch);
	setText(create_date.toString("yyyy-MM-dd hh:mm:ss"));
	return;
}

}    // namespace mtk