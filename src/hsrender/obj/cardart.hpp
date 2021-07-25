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

#ifndef MTK_HSRENDER_CARDART_H
#define MTK_HSRENDER_CARDART_H

#include <QPainterPath>

#include "../renderobj.hpp"

namespace mtk
{
class CardArt : public RenderObj
{
      private:
	std::string m_str_texture;

	inline static const std::vector<QPoint> MINION_POINTS = {
		{ 335, 102 }, { 292, 110 }, { 256, 131 }, { 222, 163 },
		{ 195, 203 }, { 171, 273 }, { 163, 330 }, { 170, 398 },
		{ 200, 474 }, { 266, 547 }, { 302, 563 }, { 343, 567 },
		{ 406, 544 }, { 449, 506 }, { 488, 432 }, { 505, 346 },
		{ 494, 255 }, { 460, 172 }, { 425, 135 }, { 385, 111 },
	};

	inline static const std::vector<QPoint> SPELL_POINTS = {
		{ 338, 171 }, { 425, 179 }, { 544, 213 }, { 551, 474 },
		{ 439, 511 }, { 327, 519 }, { 202, 505 }, { 118, 474 },
		{ 116, 213 }, { 236, 176 },
	};

	inline static const std::vector<QPoint> HERO_POINTS = {
		{ 334, 134 }, { 369, 143 }, { 406, 164 }, { 435, 187 },
		{ 453, 213 }, { 469, 245 }, { 479, 270 }, { 481, 290 },
		{ 483, 332 }, { 483, 380 }, { 483, 438 }, { 484, 485 },
		{ 435, 473 }, { 389, 467 }, { 346, 465 }, { 297, 466 },
		{ 240, 473 }, { 185, 486 }, { 184, 445 }, { 182, 357 },
		{ 184, 302 }, { 188, 271 }, { 198, 240 }, { 210, 217 },
		{ 222, 198 }, { 239, 178 }, { 262, 160 }, { 291, 145 },
	};

	inline static const std::vector<QPoint> WEAPON_POINTS = {
		{ 352, 139 }, { 418, 155 }, { 469, 188 }, { 497, 222 },
		{ 523, 267 }, { 533, 315 }, { 531, 366 }, { 514, 420 },
		{ 485, 461 }, { 444, 496 }, { 375, 515 }, { 309, 515 },
		{ 236, 484 }, { 192, 434 }, { 160, 371 }, { 158, 303 },
		{ 173, 246 }, { 203, 201 }, { 242, 167 }, { 287, 148 },
	};

	inline static const std::vector<QPoint> HP_POINTS = {
		{ 344, 161 }, { 264, 173 }, { 204, 257 }, { 207, 331 },
		{ 234, 394 }, { 333, 431 }, { 424, 407 }, { 465, 355 },
		{ 471, 261 }, { 427, 187 },
	};

	inline static const Coords MINION_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 461,
		.sh = 461,
		.dx = 105,
		.dy = 100,
		.dw = 461,
		.dh = 461,
	};

	inline static const Coords SPELL_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 418,
		.sh = 418,
		.dx = 123,
		.dy = 138,
		.dw = 418,
		.dh = 418,
	};

	inline static const Coords HERO_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 346,
		.sh = 346,
		.dx = 161,
		.dy = 137,
		.dw = 346,
		.dh = 346,
	};

	inline static const Coords WEAPON_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 384,
		.sh = 384,
		.dx = 152,
		.dy = 135,
		.dw = 384,
		.dh = 384,
	};

	inline static const Coords HP_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 261,
		.sh = 261,
		.dx = 208,
		.dy = 163,
		.dw = 261,
		.dh = 261,
	};

	Coords m_coords_texture;

	std::vector<QPoint> m_points;

      public:
	CardArt(card_type type, const std::string& texture) noexcept
	{
		switch (type) {
		case MINION:
			m_points	 = MINION_POINTS;
			m_coords_texture = MINION_COORDS;
			break;
		case SPELL:
			m_points	 = SPELL_POINTS;
			m_coords_texture = SPELL_COORDS;
			break;
		case HERO:
			m_points	 = HERO_POINTS;
			m_coords_texture = HERO_COORDS;
			break;
		case WEAPON:
			m_points	 = WEAPON_POINTS;
			m_coords_texture = WEAPON_COORDS;
			break;
		case HERO_POWER:
			m_points	 = HP_POINTS;
			m_coords_texture = HP_COORDS;
			break;
		default:
			return;
		}
		m_str_texture = texture;
		m_textures.insert({ m_str_texture, nullptr });
	}

	CardArt(const CardArt& cpy) noexcept = default;
	~CardArt() noexcept		     = default;

	void render(QPainter& painter) override
	{
		if (m_str_texture.empty()) {
			return;
		}

		std::shared_ptr<QPixmap> ptr_art = m_textures.at(m_str_texture);
		QPolygonF art_image_buffer;

		for (size_t i = 0; i < m_points.size(); i++) {
			art_image_buffer << m_points[i];
		}

		painter.save();

		QPainterPath path;
		path.addPolygon(art_image_buffer);
		painter.setClipPath(path);

		painter.drawPixmap(m_coords_texture.dx(),
				   m_coords_texture.dy(),
				   m_coords_texture.sw(),
				   m_coords_texture.sh(),
				   *ptr_art,
				   0,
				   0,
				   ptr_art->width(),
				   ptr_art->height());

		painter.restore();
	}
};
}    // namespace mtk

#endif