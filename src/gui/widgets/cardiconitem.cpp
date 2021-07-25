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

#include "cardiconitem.hpp"

#include <hsdata/conv.hpp>
//#include <hsrender/render.hpp>

#include <QPainter>

using namespace mtk;

namespace mtk
{
void
CardIconItem::init(int value, const std::string& icon_path)
{
	setData(Qt::UserRole, value);

	QPixmap p(icon_path.c_str());

	QPixmap canvas(QSize(50, 35));
	canvas.fill(Qt::transparent);
	QPainter painter;
	painter.begin(&canvas);

	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	/*
		painter.drawPixmap(QRect(0, 0, REF_TILE_HEIGHT,
	   REF_TILE_HEIGHT), QPixmap(icon_path.c_str()));
	*/
	// painter.fillRect(canvas.rect(), b);

	painter.end();

	setBackground(canvas);
	return;
}

}    // namespace mtk