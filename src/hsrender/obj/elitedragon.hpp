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

#ifndef MTK_HSRENDER_ELITEDRAGON_H
#define MTK_HSRENDER_ELITEDRAGON_H

#include "../renderobj.hpp"

namespace mtk
{
class EliteDragon : public RenderObj
{
      private:
	inline static const Coords MINION_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 436,
		.sh = 325,
		.dx = 188,
		.dy = 52,
		.dw = 436,
		.dh = 325,
	};

	inline static const Coords SPELL_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 476,
		.sh = 259,
		.dx = 185,
		.dy = 91,
		.dw = 476,
		.dh = 259,
	};

	inline static const Coords HERO_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 444,
		.sh = 298,
		.dx = 172,
		.dy = 40,
		.dw = 444,
		.dh = 298,
	};

	inline static const Coords WEAPON_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 420,
		.sh = 247,
		.dx = 199,
		.dy = 62,
		.dw = 420,
		.dh = 247,
	};

	std::string m_str_texture;
	Coords m_coords_texture;

      public:
	EliteDragon(card_type type) noexcept
	{
		switch (type) {
		case MINION:
			m_str_texture	 = "assets/elite-minion.png";
			m_coords_texture = MINION_COORDS;
			break;
		case SPELL:
			m_str_texture	 = "assets/elite-spell.png";
			m_coords_texture = SPELL_COORDS;
			break;
		case HERO:
			m_str_texture	 = "assets/elite-hero.png";
			m_coords_texture = HERO_COORDS;
			break;
		case WEAPON:
			m_str_texture	 = "assets/elite-weapon.png";
			m_coords_texture = WEAPON_COORDS;
			break;
		default:
			return;
		}
		m_textures.insert({ m_str_texture, nullptr });
	};
	EliteDragon(const Coords& coords) noexcept : RenderObj(coords) { }

	EliteDragon(const EliteDragon& cpy) noexcept = default;
	~EliteDragon() noexcept			     = default;

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