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

#ifndef MTK_HSRENDER_CENTER_TEXT_H
#define MTK_HSRENDER_CENTER_TEXT_H

#include "../renderobj.hpp"

namespace mtk
{
class CenterText : public RenderObj
{
      private:
	std::string m_text;
	std::string m_fontname;
	float m_fontsize  = 12.f;
	float m_linewidth = 1.f;
	bool m_bold	  = false;

      public:
	CenterText(const std::string& text) noexcept : m_text(text) { }
	CenterText(const std::string& text,
		   const std::string& fontname,
		   float fontsize  = 12.f,
		   float linewidth = 1.f,
		   bool bold	   = false) noexcept :
	    m_text(text),
	    m_fontname(fontname), m_fontsize(fontsize), m_linewidth(linewidth),
	    m_bold(bold)
	{
	}

	CenterText(const std::string& text,
		   const std::string& fontname,
		   const Coords& coords,
		   float fontsize  = 12.f,
		   float linewidth = 1.f,
		   bool bold	   = false) noexcept :
	    m_text(text),
	    m_fontname(fontname), m_fontsize(fontsize), m_linewidth(linewidth),
	    m_bold(bold), RenderObj(coords)
	{
	}

	CenterText(const std::string& text, const Coords& coords) noexcept :
	    m_text(text), RenderObj(coords)
	{
	}

	CenterText(const CenterText& cpy) noexcept = default;
	~CenterText() noexcept			   = default;

	void render(QPainter& painter) override
	{
		QFont font(m_fontname.c_str());
		// font.setBold(true);
		// font.setKerning(true);
		font.setPixelSize(m_fontsize);

		adj_font_size(font, m_text, m_coords.dw() - 10, 12.f);

		QPen pen(Qt::black);
		pen.setWidthF(m_linewidth);
		pen.setJoinStyle(Qt::RoundJoin);
		pen.setCapStyle(Qt::RoundCap);

		draw_text_center(painter, m_coords, m_text, font, pen);
	}
};
}    // namespace mtk

#endif