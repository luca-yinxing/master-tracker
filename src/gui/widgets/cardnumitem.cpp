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

#include "cardnumitem.hpp"

#include <hsdata/conv.hpp>

using namespace mtk;

namespace mtk
{
void
CardNumItem::init(int count, card_rarity rarity, const QColor& background)
{
	setData(Qt::UserRole, count);

	const unsigned char* rarity_color = conv_rarity_color(rarity);

	setBackground(background);
	setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	setForeground(
	    QColor(rarity_color[0], rarity_color[1], rarity_color[2]));
	setText(QString::number(count));
	return;
}

}    // namespace mtk