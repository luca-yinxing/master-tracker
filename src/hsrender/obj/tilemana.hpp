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

#ifndef MTK_HSRENDER_TILEMANA_H
#define MTK_HSRENDER_TILEMANA_H

#include "../renderobj.hpp"

namespace mtk
{
class TileMana : public RenderObj
{
      private:
	inline static const Coords COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 0,
		.dy = 0,
		.dw = 59,
		.dh = 59,
	};

	inline static const Coords TEXT_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 0,
		.dy = 0,
		.dw = 59,
		.dh = 59,
	};

	inline static const QColor BACKGROUND = QColor(46, 82, 119);

	inline static const float FONTSIZE  = 52.f;
	inline static const float LINEWIDTH = 5.f;

	std::string m_text;

      public:
	TileMana(int cost) noexcept { m_text = std::to_string(cost); };
	TileMana(const Coords& coords) noexcept : RenderObj(coords) { }

	TileMana(const TileMana& cpy) noexcept = default;
	~TileMana() noexcept		       = default;

	void render(QPainter& painter) override
	{
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.setRenderHint(QPainter::TextAntialiasing);

		painter.fillRect(COORDS.dx(),
				 COORDS.dy(),
				 COORDS.dw(),
				 COORDS.dh(),
				 BACKGROUND);

		if (!m_text.empty()) {
			QFont font("Belwe Bd BT");
			font.setPixelSize(FONTSIZE);

			QPen pen(Qt::black);
			pen.setWidthF(LINEWIDTH);
			pen.setJoinStyle(Qt::RoundJoin);
			pen.setCapStyle(Qt::RoundCap);

			draw_text_center(painter,
					 TEXT_COORDS,
					 m_text,
					 font,
					 pen);
		}
	}
};
}    // namespace mtk

#endif