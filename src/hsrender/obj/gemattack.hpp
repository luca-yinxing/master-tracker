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

#ifndef MTK_HSRENDER_GEMATTACK_H
#define MTK_HSRENDER_GEMATTACK_H

#include "../renderobj.hpp"

namespace mtk
{
class GemAttack : public RenderObj
{
      private:
	inline static const Coords MINION_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 154,
		.sh = 173,
		.dx = 36,
		.dy = 721,
		.dw = 154,
		.dh = 173,
	};

	inline static const Coords WEAPON_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 135,
		.sh = 133,
		.dx = 65,
		.dy = 753,
		.dw = 135,
		.dh = 133,
	};

	inline static const Coords TEXT_MINION_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 125,
		.dy = 824,
		.dw = 0,
		.dh = 0,
	};

	inline static const Coords TEXT_WEAPON_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 136,
		.dy = 820,
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
	GemAttack(card_type type, int value) noexcept
	{
		switch (type) {
		case MINION:
			m_str_texture	 = "assets/attack-minion.png";
			m_coords_texture = MINION_COORDS;
			m_coords_text	 = TEXT_MINION_COORDS;
			break;
		case WEAPON:
			m_str_texture	 = "assets/attack-weapon.png";
			m_coords_texture = WEAPON_COORDS;
			m_coords_text	 = TEXT_WEAPON_COORDS;
			break;
		default:
			return;
		}

		m_text = std::to_string(value);
		m_textures.insert({ m_str_texture, nullptr });
	};
	GemAttack(const Coords& coords) noexcept : RenderObj(coords) { }

	GemAttack(const GemAttack& cpy) noexcept = default;
	~GemAttack() noexcept			 = default;

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