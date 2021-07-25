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

#ifndef MTK_HSRENDER_CARDFRAME_H
#define MTK_HSRENDER_CARDFRAME_H

#include "../renderobj.hpp"

namespace mtk
{
class CardFrame : public RenderObj
{
      private:
	inline static const Coords HERO_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 527,
		.sh = 795,
		.dx = 70,
		.dy = 87,
		.dw = 527,
		.dh = 795,
	};

	inline static const Coords WEAPON_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 527,
		.sh = 775,
		.dx = 80,
		.dy = 103,
		.dw = 527,
		.dh = 775,
	};

	inline static Coords SPELL_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 70,
		.dy = 133,
		.dw = 527,
		.dh = 746,
	};

	inline static Coords MINION_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 528,
		.sh = 793,
		.dx = 70,
		.dy = 89,
		.dw = 528,
		.dh = 793,
	};

	inline static Coords HP_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 564,
		.sh = 841,
		.dx = 56,
		.dy = 65,
		.dw = 564,
		.dh = 841,
	};

	std::string m_str_texture;
	Coords m_coords_texture;

      public:
	CardFrame(card_type type, card_class klass) noexcept
	{
		std::string str_class;
		switch (type) {
		case MINION:
			str_class = conv_class_string(klass);
			m_str_texture =
			    "assets/frame-minion-" + str_class + ".png";
			m_coords_texture = MINION_COORDS;
			break;
		case SPELL:
			str_class = conv_class_string(klass);
			m_str_texture =
			    "assets/frame-spell-" + str_class + ".png";
			m_coords_texture = SPELL_COORDS;
			break;
		case HERO:
			str_class = conv_class_string(klass);
			m_str_texture =
			    "assets/frame-hero-" + str_class + ".png";
			m_coords_texture = HERO_COORDS;
			break;
		case WEAPON:
			str_class = conv_class_string(klass);
			m_str_texture =
			    "assets/frame-weapon-" + str_class + ".png";
			m_coords_texture = WEAPON_COORDS;
			break;
		case HERO_POWER:
			m_str_texture	 = "assets/hero-power-player.png";
			m_coords_texture = HP_COORDS;
			break;
		default:
			return;
		}
		m_textures.insert({ m_str_texture, nullptr });
	};
	CardFrame(const Coords& coords) noexcept : RenderObj(coords) { }

	CardFrame(const CardFrame& cpy) noexcept = default;
	~CardFrame() noexcept			 = default;

	void render(QPainter& painter) override
	{
		if (m_str_texture.empty()) {
			return;
		}

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