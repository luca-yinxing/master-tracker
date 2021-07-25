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

#ifndef MTK_HSRENDER_CARDBANNERNAME_H
#define MTK_HSRENDER_CARDBANNERNAME_H

#include "../renderobj.hpp"

namespace mtk
{
class CardBannerName : public RenderObj
{
      private:
	inline static const Coords MINION_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 96,
		.dy = 469,
		.dw = 485,
		.dh = 113,
	};

	inline static const Coords SPELL_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 507,
		.sh = 155,
		.dx = 80,
		.dy = 457,
		.dw = 507,
		.dh = 155,
	};

	inline static const Coords HERO_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 490,
		.sh = 122,
		.dx = 91,
		.dy = 458,
		.dw = 490,
		.dh = 122,
	};

	inline static const Coords WEAPON_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 514,
		.sh = 108,
		.dx = 87,
		.dy = 468,
		.dw = 514,
		.dh = 108,
	};

	std::string m_str_texture;
	Coords m_coords_texture;

      public:
	CardBannerName(card_type type) noexcept
	{
		switch (type) {
		case MINION:
			m_str_texture	 = "assets/name-banner-minion.png";
			m_coords_texture = MINION_COORDS;
			break;
		case SPELL:
			m_str_texture	 = "assets/name-banner-spell.png";
			m_coords_texture = SPELL_COORDS;
			break;
		case HERO:
			m_str_texture	 = "assets/name-banner-hero.png";
			m_coords_texture = HERO_COORDS;
			break;
		case WEAPON:
			m_str_texture	 = "assets/name-banner-weapon.png";
			m_coords_texture = WEAPON_COORDS;
			break;
		default:
			return;
		}
		m_textures.insert({ m_str_texture, nullptr });
	};
	CardBannerName(const Coords& coords) noexcept : RenderObj(coords) { }

	CardBannerName(const CardBannerName& cpy) noexcept = default;
	~CardBannerName() noexcept			   = default;

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