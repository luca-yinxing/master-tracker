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

#ifndef MTK_HSRENDER_GEMRARITY_H
#define MTK_HSRENDER_GEMRARITY_H

#include "../renderobj.hpp"

namespace mtk
{
class GemRarity : public RenderObj
{
      private:
	inline static const Coords MINION_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 485,
		.sh = 113,
		.dx = 263,
		.dy = 532,
		.dw = 485,
		.dh = 113,
	};

	inline static const Coords SPELL_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 116,
		.sh = 77,
		.dx = 272,
		.dy = 541,
		.dw = 116,
		.dh = 77,
	};

	inline static const Coords HERO_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 311,
		.dy = 529,
		.dw = 42,
		.dh = 58,
	};

	inline static const Coords WEAPON_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 96,
		.sh = 90,
		.dx = 302,
		.dy = 520,
		.dw = 96,
		.dh = 90,
	};

	std::string m_str_texture;
	Coords m_coords_texture;

      public:
	GemRarity(card_type type, card_rarity rarity) noexcept
	{
		std::string str_rarity;
		switch (type) {
		case MINION:
			str_rarity = conv_rarity_string(rarity);
			m_str_texture =
			    "assets/rarity-minion-" + str_rarity + ".png";
			m_coords_texture = MINION_COORDS;
			break;
		case SPELL:
			str_rarity = conv_rarity_string(rarity);
			m_str_texture =
			    "assets/rarity-spell-" + str_rarity + ".png";
			m_coords_texture = SPELL_COORDS;
			break;
		case HERO:
			str_rarity    = conv_rarity_string(rarity);
			m_str_texture = "assets/rarity-" + str_rarity + ".png";
			m_coords_texture = HERO_COORDS;
			break;
		case WEAPON:
			str_rarity = conv_rarity_string(rarity);
			m_str_texture =
			    "assets/rarity-weapon-" + str_rarity + ".png";
			m_coords_texture = WEAPON_COORDS;
			break;
		default:
			return;
		}
		m_textures.insert({ m_str_texture, nullptr });
	};
	GemRarity(const Coords& coords) noexcept : RenderObj(coords) { }

	GemRarity(const GemRarity& cpy) noexcept = default;
	~GemRarity() noexcept			 = default;

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