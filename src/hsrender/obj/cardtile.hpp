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

#ifndef MTK_HSRENDER_CARDTILE_H
#define MTK_HSRENDER_CARDTILE_H

#include "../renderobj.hpp"

namespace mtk
{
class CardTile : public RenderObj
{
      private:
	std::string m_str_texture;

	inline static Coords ART_COORDS = {
		.sx = 10,
		.sy = 85,
		.sw = 256,
		.sh = 59,
		.dx = 59,
		.dy = 0,
		.dw = 256,
		.dh = 59,
	};

	static constexpr float TILE_SCALE = 0.60f;

      public:
	CardTile(const std::string& texture) noexcept
	{
		m_str_texture = texture;
		m_textures.insert({ m_str_texture, nullptr });
	}
	CardTile(const CardTile& cpy) noexcept = default;
	~CardTile() noexcept		       = default;

	void render(QPainter& painter) override
	{
		std::shared_ptr<QPixmap> ptr_art = m_textures.at(m_str_texture);
		QPixmap scale_art =
		    ptr_art->scaled(ptr_art->width() * TILE_SCALE,
				    ptr_art->height() * TILE_SCALE);

		painter.setRenderHint(QPainter::Antialiasing);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);

		painter.drawPixmap(ART_COORDS.dx(),
				   ART_COORDS.dy(),
				   ART_COORDS.dw(),
				   ART_COORDS.dh(),
				   scale_art,
				   ART_COORDS.sx(),
				   ART_COORDS.sy(),
				   ART_COORDS.sw(),
				   ART_COORDS.sh());
	}
};
}    // namespace mtk

#endif