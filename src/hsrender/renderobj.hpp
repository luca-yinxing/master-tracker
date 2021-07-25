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

#ifndef MTK_RENDER_RENDEROBJ_H
#define MTK_RENDER_RENDEROBJ_H

#include "render_utils.hpp"

#include <QPainter>
#include <QPixmap>

namespace mtk
{
class RenderObj
{
      protected:
	Coords m_coords;
	std::shared_ptr<QPixmap> m_pixmap;
	bool m_is_rendered     = false;
	bool m_prefill	       = true;
	QColor m_prefill_color = Qt::transparent;

	QPainter::CompositionMode m_composition =
	    QPainter::CompositionMode_SourceOver;

	std::unordered_map<std::string, std::shared_ptr<QPixmap>> m_textures;

      public:
	RenderObj() noexcept = default;
	RenderObj(const Coords& coords) noexcept :
	    m_coords(coords),
	    m_pixmap(std::make_shared<QPixmap>(coords.dw(), coords.dh())),
	    m_is_rendered(false)
	{
	}
	RenderObj(const RenderObj& cpy) noexcept = default;
	virtual ~RenderObj() noexcept		 = default;

	bool is_rendered() const noexcept { return m_is_rendered; }
	void is_rendered(bool v) noexcept { m_is_rendered = v; }

	QPainter::CompositionMode composition() const noexcept
	{
		return m_composition;
	}

	void composition(QPainter::CompositionMode value) noexcept
	{
		m_composition = value;
	}

	const Coords& coords() const noexcept { return m_coords; }
	Coords& coords() noexcept { return m_coords; }

	void coords(const Coords& v) noexcept { m_coords = v; }

	std::shared_ptr<QPixmap>& pixmap() noexcept { return m_pixmap; }

	const std::shared_ptr<QPixmap>& pixmap() const noexcept
	{
		return m_pixmap;
	}

	bool prefill() const noexcept { return m_prefill; }
	void prefill(bool v) noexcept { m_prefill = v; }

	QColor prefill_color() const noexcept { return m_prefill_color; }
	void prefill_color(const QColor& v) noexcept { m_prefill_color = v; }

	void init(const Coords& coords) noexcept
	{
		if (m_pixmap != nullptr) {
			return;
		}

		m_coords = coords;
		m_pixmap = std::make_shared<QPixmap>(coords.dw(), coords.dh());
		m_is_rendered = false;
	}

	void init(const std::shared_ptr<QPixmap>& pixmap) noexcept
	{
		if (m_pixmap != nullptr) {
			return;
		}

		m_coords      = Coords(pixmap->width(), pixmap->height());
		m_pixmap      = pixmap;
		m_is_rendered = false;
	}

	std::unordered_map<std::string, std::shared_ptr<QPixmap>>&
	textures() noexcept
	{
		return m_textures;
	}

	virtual void render(QPainter& painter) = 0;
};
}    // namespace mtk

#endif