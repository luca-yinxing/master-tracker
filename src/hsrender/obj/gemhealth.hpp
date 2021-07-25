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

#ifndef MTK_HSRENDER_GEMHEALTH_H
#define MTK_HSRENDER_GEMHEALTH_H

#include "../renderobj.hpp"

namespace mtk
{
class GemHealth : public RenderObj
{
      private:
	inline static const Coords MINION_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 109,
		.sh = 164,
		.dx = 504,
		.dy = 728,
		.dw = 109,
		.dh = 164,
	};

	inline static const Coords TEXT_MINION_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 556,
		.dy = 825,
		.dw = 0,
		.dh = 0,
	};

	inline static const Coords WEAPON_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 126,
		.sh = 140,
		.dx = 501,
		.dy = 744,
		.dw = 126,
		.dh = 140,
	};

	inline static const Coords TEXT_WEAPON_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 563,
		.dy = 819,
		.dw = 0,
		.dh = 0,
	};
	inline static const Coords HERO_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 115,
		.sh = 135,
		.dx = 498,
		.dy = 752,
		.dw = 115,
		.dh = 135,
	};

	inline static const Coords TEXT_HERO_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 554,
		.dy = 822,
		.dw = 0,
		.dh = 0,
	};

	inline static const float FONTSIZE  = 124;
	inline static const float LINEWIDTH = 10.f;

	std::string m_str_texture;
	Coords m_coords_texture;
	Coords m_coords_text;
	std::string m_text;

      public:
	GemHealth(card_type type, int value) noexcept
	{
		switch (type) {
		case MINION:
			m_str_texture	 = "assets/health.png";
			m_coords_texture = MINION_COORDS;
			m_coords_text	 = TEXT_MINION_COORDS;
			break;
		case WEAPON:
			m_str_texture	 = "assets/durability.png";
			m_coords_texture = WEAPON_COORDS;
			m_coords_text	 = TEXT_WEAPON_COORDS;
			break;
		case HERO:
			m_str_texture	 = "assets/armor.png";
			m_coords_texture = HERO_COORDS;
			m_coords_text	 = TEXT_HERO_COORDS;
			break;
		default:
			return;
		}
		m_text = std::to_string(value);
		m_textures.insert({ m_str_texture, nullptr });
	};
	GemHealth(const Coords& coords) noexcept : RenderObj(coords) { }

	GemHealth(const GemHealth& cpy) noexcept = default;
	~GemHealth() noexcept			 = default;

	void render(QPainter& painter) override
	{
		if (m_str_texture.empty()) {
			return;
		}

		painter.setRenderHint(QPainter::Antialiasing);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.setRenderHint(QPainter::TextAntialiasing);

		std::shared_ptr<QPixmap> ptr_tex = m_textures.at(m_str_texture);
		painter.drawPixmap(m_coords_texture.dx(),
				   m_coords_texture.dy(),
				   m_coords_texture.dw(),
				   m_coords_texture.dh(),
				   *ptr_tex,
				   m_coords_texture.sx(),
				   m_coords_texture.sy(),
				   m_coords_texture.sw(),
				   m_coords_texture.sh());

		QFont font("Belwe Bd BT");
		font.setPixelSize(FONTSIZE);

		QPen pen(Qt::black);
		pen.setWidthF(LINEWIDTH);
		pen.setJoinStyle(Qt::RoundJoin);
		pen.setCapStyle(Qt::RoundCap);

		draw_text_center(painter, m_coords_text, m_text, font, pen);
	}
};
}    // namespace mtk

#endif