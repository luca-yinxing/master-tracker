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

#ifndef MTK_GUI_DECKNAMEITEM_H
#define MTK_GUI_DECKNAMEITEM_H

#include <QFont>
#include <QTableWidgetItem>

#include <hsdata/conv.hpp>
#include <hsdata/deck.hpp>

namespace mtk
{
class DeckNameItem : public QTableWidgetItem
{
      private:
	std::shared_ptr<Deck> m_ptr = nullptr;

      public:
	inline DeckNameItem() noexcept { }
	inline ~DeckNameItem() noexcept { }

	void init(const std::string& texture,
		  const std::shared_ptr<Deck>& pdeck);

	inline bool operator<(const QTableWidgetItem& other) const
	{
		return data(Qt::UserRole).toString() <
		       other.data(Qt::UserRole).toString();
	}

	inline const std::shared_ptr<Deck>& ptr() const noexcept
	{
		return m_ptr;
	}
};

}    // namespace mtk

#endif