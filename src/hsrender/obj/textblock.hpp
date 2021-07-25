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

#ifndef MTK_HSRENDER_TEXTBLOCK_H
#define MTK_HSRENDER_TEXTBLOCK_H

#include "../renderobj.hpp"

#include <QPainterPath>

#include <QAbstractTextDocumentLayout>
#include <QTextDocument>

namespace mtk
{
class TextBlock : public RenderObj
{
      private:
	std::string m_text;
	std::string m_fontname;
	float m_fontsize   = 12.f;
	float m_penwidth   = 1.f;
	float m_lineheight = 40.f;
	bool m_bold	   = false;
	QColor m_color	   = Qt::black;

	inline static const Coords MINION_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 0,
		.sh = 0,
		.dx = 130,
		.dy = 622,
		.dw = 408,
		.dh = 176,
	};

	inline static const Coords SPELL_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 378,
		.sh = 168,
		.dx = 144,
		.dy = 630,
		.dw = 378,
		.dh = 168,
	};

	inline static const Coords HERO_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 376,
		.sh = 168,
		.dx = 143,
		.dy = 627,
		.dw = 376,
		.dh = 168,
	};

	inline static const Coords WEAPON_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 388,
		.sh = 168,
		.dx = 146,
		.dy = 628,
		.dw = 388,
		.dh = 168,
	};

	inline static const Coords HP_COORDS = {
		.sx = 0,
		.sy = 0,
		.sw = 380,
		.sh = 174,
		.dx = 144,
		.dy = 606,
		.dw = 380,
		.dh = 174,
	};

	Coords m_coords_text;

      public:
	TextBlock(const std::string& text) noexcept : m_text(text) { }
	TextBlock(card_type type,
		  const std::string& text,
		  const std::string& fontname,
		  float fontsize   = 38.f,
		  float penwidth   = 1.f,
		  float lineheight = 40.f,
		  QColor color	   = Qt::black,
		  bool bold	   = false) noexcept :
	    m_text(text),
	    m_fontname(fontname), m_fontsize(fontsize), m_penwidth(penwidth),
	    m_lineheight(lineheight), m_color(color), m_bold(bold)
	{
		switch (type) {
		case MINION:
			m_coords_text = MINION_COORDS;
			break;
		case SPELL:
			m_coords_text = SPELL_COORDS;
			break;
		case HERO:
			m_coords_text = HERO_COORDS;
			break;
		case WEAPON:
			m_coords_text = WEAPON_COORDS;
			break;
		case HERO_POWER:
			m_coords_text = HP_COORDS;
		default:
			return;
		}
	}

	TextBlock(const std::string& text,
		  const std::string& fontname,
		  const Coords& coords,
		  float fontsize = 12.f,
		  float penwidth = 1.f,
		  QColor color	 = Qt::black,
		  bool bold	 = false) noexcept :
	    m_text(text),
	    m_fontname(fontname), m_fontsize(fontsize), m_penwidth(penwidth),
	    m_color(color), m_bold(bold), RenderObj(coords)
	{
	}

	TextBlock(const std::string& text, const Coords& coords) noexcept :
	    m_text(text), RenderObj(coords)
	{
	}

	TextBlock(const TextBlock& cpy) noexcept = default;
	~TextBlock() noexcept			 = default;

	void render(QPainter& painter) override
	{
		const float offset_y = 10;
		const float offset_x = 0;

		QString str_text = m_text.c_str();
		str_text.replace("\n", "<br>");
		str_text.replace("[x]", "");
		str_text.replace("$", "");

		painter.translate(m_coords_text.dx() + offset_x,
				  m_coords_text.dy() + offset_y);
		painter.setPen(QPen(m_color, m_penwidth));
		painter.setBrush(m_color);

		QTextDocument td;

		QAbstractTextDocumentLayout::PaintContext ctx;
		ctx.clip = QRectF(0, 0, 100, m_coords_text.dh());

		ctx.palette.setColor(QPalette::Text, painter.pen().color());

		td.setHtml(str_text);

		int text_length = td.characterCount();

		// float def_font = 38;
		// float def_line_height = 40;
		float font_size	  = m_fontsize;
		float line_height = m_lineheight;

		if (text_length >= 65) {
			font_size *= 0.95;
			line_height *= 0.95;
		}

		if (text_length >= 80) {
			font_size *= 0.9;
			line_height *= 0.9;
		}

		if (text_length >= 100) {
			font_size *= 0.8;
			line_height *= 0.8;
		}

		if (text_length >= 120) {
			font_size *= 0.62;
			line_height *= 0.8;
		}

		QFont font(m_fontname.c_str());
		font.setBold(false);
		font.setPixelSize(font_size);

		td.setDefaultFont(font);
		td.setTextWidth(m_coords_text.dw());
		QTextOption text_option;
		text_option.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		td.setDefaultTextOption(text_option);

		td.documentLayout()->draw(&painter, ctx);
	}
};
}    // namespace mtk

#endif