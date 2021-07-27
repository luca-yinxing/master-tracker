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

#ifndef MTK_HSRENDER_UTILS_H
#define MTK_HSRENDER_UTILS_H

//#include <QFont>
#include <QFontMetrics>

#include <QPainter>
#include <QPainterPath>

#include <math.h>

namespace mtk
{
class Coords
{
	int m_sx = 0;
	int m_sy = 0;
	int m_sw = 0;
	int m_sh = 0;

	int m_dx = 0;
	int m_dy = 0;
	int m_dw = 0;
	int m_dh = 0;

      public:
	Coords() noexcept = default;
	Coords(int dw, int dh) noexcept : m_dw(dw), m_dh(dh) { }
	Coords(int dx, int dy, int dw, int dh) noexcept :
	    m_dx(dx), m_dy(dy), m_dw(dw), m_dh(dh)
	{
	}
	Coords(int sx,
	       int sy,
	       int sw,
	       int sh,
	       int dx,
	       int dy,
	       int dw,
	       int dh) noexcept :
	    m_sx(sx),
	    m_sy(sy), m_sw(sw), m_sh(sh), m_dx(dx), m_dy(dy), m_dw(dw), m_dh(dh)
	{
	}

	Coords(const Coords& cpy) noexcept = default;
	~Coords() noexcept		   = default;

	int sx() const noexcept { return m_sx; }
	int sy() const noexcept { return m_sy; }
	int sw() const noexcept { return m_sw; }
	int sh() const noexcept { return m_sh; }

	int dx() const noexcept { return m_dx; }
	int dy() const noexcept { return m_dy; }
	int dw() const noexcept { return m_dw; }
	int dh() const noexcept { return m_dh; }

	void sx(int v) noexcept { m_sx = v; }
	void sy(int v) noexcept { m_sy = v; }
	void sw(int v) noexcept { m_sw = v; }
	void sh(int v) noexcept { m_sh = v; }

	void dx(int v) noexcept { m_dx = v; }
	void dy(int v) noexcept { m_dy = v; }
	void dw(int v) noexcept { m_dw = v; }
	void dh(int v) noexcept { m_dh = v; }
};

struct render_char {
	QChar c;
	float w;
};

inline void
adj_font_size(QFont& font,
	      const std::string& text,
	      float max_width,
	      float min_size = 10.f)
{
	if (max_width == 0) {
		return;
	}

	float font_size	   = font.pixelSize();
	float render_width = 0.f;
	do {
		font_size -= 1;
		font.setPixelSize(font_size);
		QFontMetrics fm(font);
		render_width = fm.horizontalAdvance(text.c_str());
	} while (render_width > max_width && font_size > min_size);
}

inline void
point_curve(const std::vector<QPoint>& curve,
	    float pos,
	    float& x,
	    float& y,
	    float& r)
{
	float rx, ry;
	rx = 3 * pow(1 - pos, 2) * (curve[1].x() - curve[0].x()) +
	     6 * (1 - pos) * pos * (curve[2].x() - curve[1].x()) +
	     3 * pow(pos, 2) * (curve[3].x() - curve[2].x());

	ry = 3 * pow(1 - pos, 2) * (curve[1].y() - curve[0].y()) +
	     6 * (1 - pos) * pos * (curve[2].y() - curve[1].y()) +
	     3 * pow(pos, 2) * (curve[3].y() - curve[2].y());

	x = pow(1 - pos, 3) * curve[0].x() +
	    3 * pow(1 - pos, 2) * pos * curve[1].x() +
	    3 * (1 - pos) * pow(pos, 2) * curve[2].x() +
	    pow(pos, 3) * curve[3].x();

	y = pow(1 - pos, 3) * curve[0].y() +
	    3 * pow(1 - pos, 2) * pos * curve[1].y() +
	    3 * (1 - pos) * pow(pos, 2) * curve[2].y() +
	    pow(pos, 3) * curve[3].y();

	r = atan2(ry, rx) * 180 / M_PI;
}

inline void
calc_chars_width(render_char rc[], size_t len, QFontMetrics& fm)
{
	const int em = fm.horizontalAdvance("M");

	for (size_t i = 0; i < len; i++) {
		rc[i].w = fm.horizontalAdvance(rc[i].c) + 0.05 * em;
		if (rc[i].c == ' ') {
			rc[i].w = 0.2 * em;
		} else if (rc[i].c == '\'') {
			rc[i].w = 0.27 * em;
		}
	}
}

inline void
draw_text_center(QPainter& painter,
		 const Coords& coords,
		 const std::string& text,
		 const QFont& font,
		 const QPen& pen,
		 const QColor& fillcolor = Qt::white)
{
	QPainterPath path;

	path.setFillRule(Qt::WindingFill);
	QFontMetrics fm(font);
	float font_height = fm.height();
	float font_width  = fm.horizontalAdvance(text.c_str());
	float x		  = 0;
	float y		  = 0;
	if (coords.dx() == 0 && coords.dy() == 0) {
		x = (coords.dw() / 2) - (font_width / 2);
		y = (coords.dh() / 2) + (font_height / 4);
	} else {
		x = coords.dx() - (font_width / 2);
		y = coords.dy() + (font_height / 4);
	}
	path.addText(x, y, font, text.c_str());

	painter.strokePath(path, pen);
	painter.fillPath(path, fillcolor);
}

}    // namespace mtk

#endif