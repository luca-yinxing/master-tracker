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

#ifndef MTK_HSRENDER_GEMMANA_H
#define MTK_HSRENDER_GEMMANA_H

#include "../renderobj.hpp"

namespace mtk
{
class GemMana : public RenderObj
{
      private:
	inline static const Coords COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 47,
		.dy = 105,
		.dw = 140,
		.dh = 144,
	};

	inline static const Coords TEXT_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 115,
		.dy = 174,
		.dw = 0,
		.dh = 0,
	};

	inline static const Coords HP_TEXT_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 338,
		.dy = 124,
		.dw = 0,
		.dh = 0,
	};

	inline static const float FONTSIZE  = 130;
	inline static const float LINEWIDTH = 10.f;

	std::string m_str_texture;
	std::string m_text;
	Coords m_coords_text;

      public:
	GemMana(card_type type, int cost) noexcept
	{
		m_text = std::to_string(cost);

		if (type == HERO_POWER) {
			m_coords_text = HP_TEXT_COORDS;
		} else {
			m_coords_text = TEXT_COORDS;
			m_str_texture = "assets/cost-mana.png";
			m_textures.insert({ m_str_texture, nullptr });
		}
	};
	GemMana(const Coords& coords) noexcept : RenderObj(coords) { }

	GemMana(const GemMana& cpy) noexcept = default;
	~GemMana() noexcept		     = default;

	void render(QPainter& painter) override
	{
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.setRenderHint(QPainter::TextAntialiasing);

		if (!m_str_texture.empty()) {
			std::shared_ptr<QPixmap> ptr_tex =
			    m_textures.at(m_str_texture);
			painter.drawPixmap(COORDS.dx(),
					   COORDS.dy(),
					   COORDS.dw(),
					   COORDS.dh(),
					   *ptr_tex,
					   COORDS.sx(),
					   COORDS.sy(),
					   COORDS.sw(),
					   COORDS.sh());
		}
		if (!m_text.empty()) {
			QFont font("Belwe Bd BT");
			font.setPixelSize(FONTSIZE);

			QPen pen(Qt::black);
			pen.setWidthF(LINEWIDTH);
			pen.setJoinStyle(Qt::RoundJoin);
			pen.setCapStyle(Qt::RoundCap);

			draw_text_center(painter,
					 m_coords_text,
					 m_text,
					 font,
					 pen);
		}
	}
};
}    // namespace mtk

#endif