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

#ifndef MTK_HSRENDER_WATERMARK_H
#define MTK_HSRENDER_WATERMARK_H

#include "../renderobj.hpp"

namespace mtk
{
class Watermark : public RenderObj
{
      private:
	inline static const Coords MINION_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 231,
		.dy = 604,
		// watermark.dy = 594;
		.dw = 225,
		.dh = 225,
	};

	inline static const Coords SPELL_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 232,
		.dy = 612,
		// watermark.dy = 594;
		.dw = 210,
		.dh = 210,
	};

	inline static const Coords HERO_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 247,
		.dy = 625,
		.dw = 170,
		.dh = 170,
	};

	inline static const Coords WEAPON_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 241,
		.dy = 599,
		.dw = 220,
		.dh = 220,
	};

	std::string m_str_texture;
	Coords m_coords_texture;

	float m_alpha = 0.f;

      public:
	Watermark(card_type type, card_set set) noexcept
	{
		switch (type) {
		case MINION:
			composition(QPainter::CompositionMode_Multiply);
			m_coords_texture = MINION_COORDS;
			m_alpha		 = 0.6;
			break;
		case SPELL:
			composition(QPainter::CompositionMode_Multiply);
			m_coords_texture = SPELL_COORDS;
			m_alpha		 = 0.7;
			break;
		case HERO:
			composition(QPainter::CompositionMode_Multiply);
			m_coords_texture = HERO_COORDS;
			m_alpha		 = 0.6;
			break;
		case WEAPON:
			composition(QPainter::CompositionMode_Lighten);
			m_coords_texture = WEAPON_COORDS;
			m_alpha		 = 0.1;
			break;
		default:
			return;
		}
		std::string str_set = conv_set_string(set);
		m_str_texture	    = "assets/set-" + str_set + ".png";
		m_textures.insert({ m_str_texture, nullptr });
	};
	Watermark(const Coords& coords) noexcept : RenderObj(coords) { }

	Watermark(const Watermark& cpy) noexcept = default;
	~Watermark() noexcept			 = default;

	void render(QPainter& painter) override
	{
		if (m_str_texture.empty()) {
			return;
		}

		painter.setRenderHint(QPainter::Antialiasing);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);

		painter.setOpacity(m_alpha);

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
	}
};
}    // namespace mtk

#endif