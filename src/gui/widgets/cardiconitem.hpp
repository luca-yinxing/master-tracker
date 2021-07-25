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

#ifndef MTK_GUI_CARDICONITEM_H
#define MTK_GUI_CARDICONITEM_H

#include <QTableWidgetItem>

#include <hsdata/types.hpp>

namespace mtk
{
class CardIconItem : public QTableWidgetItem
{
      private:
      public:
	inline CardIconItem() noexcept { }
	inline ~CardIconItem() noexcept { }

	void init(int value, const std::string& icon_path);

	inline bool operator<(const QTableWidgetItem& other) const
	{
		return data(Qt::UserRole).toInt() <
		       other.data(Qt::UserRole).toInt();
	}
};
}    // namespace mtk

#endif