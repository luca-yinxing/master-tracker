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

#ifndef MTK_HSRENDER_LINEAR_GRADIENT_H
#define MTK_HSRENDER_LINEAR_GRADIENT_H

#include "../renderobj.hpp"

#include <QBrush>
#include <QLinearGradient>

namespace mtk
{
class LinearGradient : public RenderObj
{
      private:
	std::map<float, QColor> m_colormap;

	inline static Coords COORDS = {
		.sx = 10,
		.sy = 85,
		.sw = 256,
		.sh = 59,
		.dx = 59,
		.dy = 0,
		.dw = 256,
		.dh = 59,
	};

      public:
	LinearGradient(const std::map<float, QColor>& colormap) noexcept :
	    m_colormap(colormap)
	{
	}
	LinearGradient(const std::map<float, QColor>& colormap,
		       const Coords& coords) noexcept :
	    m_colormap(colormap),
	    RenderObj(coords)
	{
	}

	LinearGradient(const LinearGradient& cpy) noexcept = default;
	~LinearGradient() noexcept			   = default;

	void render(QPainter& painter) override
	{
		QLinearGradient gradient(COORDS.dx(),
					 COORDS.dy(),
					 COORDS.dw(),
					 COORDS.dh());
		for (auto& step : m_colormap) {
			gradient.setColorAt(step.first, step.second);
		}
		QBrush brush(gradient);

		painter.fillRect(COORDS.dx(),
				 COORDS.dy(),
				 COORDS.dw(),
				 COORDS.dh(),
				 brush);
	}
};
}    // namespace mtk

#endif