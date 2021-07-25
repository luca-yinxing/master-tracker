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

#ifndef MTK_HSRENDER_BANNERSPELL_H
#define MTK_HSRENDER_BANNERSPELL_H

#include "../renderobj.hpp"

namespace mtk
{
class BannerSpell : public RenderObj
{
      private:
	inline static const Coords COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 352,
		.sh = 56,
		.dx = 165,
		.dy = 803,
		.dw = 352,
		.dh = 56,
	};

	inline static const Coords TEXT_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 340,
		.dy = 833,
		.dw = 0,
		.dh = 0,
	};

	inline static const float FONTSIZE  = 40;
	inline static const float LINEWIDTH = 5.0f;

	std::string m_str_texture;
	std::string m_text;

      public:
	BannerSpell(card_spellschool spellschool) noexcept
	{
		m_text	      = conv_spellschool_string_en(spellschool);
		m_str_texture = "assets/banner-spellschool.png";
		m_textures.insert({ m_str_texture, nullptr });
	};
	BannerSpell(const Coords& coords) noexcept : RenderObj(coords) { }

	BannerSpell(const BannerSpell& cpy) noexcept = default;
	~BannerSpell() noexcept			     = default;

	void render(QPainter& painter) override
	{
		if (m_str_texture.empty()) {
			return;
		}

		painter.setRenderHint(QPainter::Antialiasing);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		painter.setRenderHint(QPainter::TextAntialiasing);

		std::shared_ptr<QPixmap> ptr_tex = m_textures.at(m_str_texture);
		painter.drawPixmap(COORDS.dx(),
				   COORDS.dy(),
				   COORDS.dw(),
				   COORDS.dh(),
				   *ptr_tex,
				   COORDS.sx(),
				   COORDS.sy(),
				   COORDS.sw(),
				   COORDS.sh());

		QFont font("Belwe Bd BT");
		font.setPixelSize(FONTSIZE);

		QPen pen(Qt::black);
		pen.setWidthF(LINEWIDTH);
		pen.setJoinStyle(Qt::RoundJoin);
		pen.setCapStyle(Qt::RoundCap);

		draw_text_center(painter, TEXT_COORDS, m_text, font, pen);
	}
};
}    // namespace mtk

#endif