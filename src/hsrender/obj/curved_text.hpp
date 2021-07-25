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

#ifndef MTK_HSRENDER_CURVEDTEXT_H
#define MTK_HSRENDER_CURVEDTEXT_H

/*
#include "../renderobj.hpp"

namespace mtk
{
class CurvedText : public RenderObj
{
      private:
	inline static const std::vector<QPoint> MINION_CURVE = {
		{ 0, 88 },
		{ 98, 112 },
		{ 294, 13 },
		{ 460, 80 },
	};

	inline static const float MINION_MIDDLE = 0.55f;
	inline static const int MINION_MAXWIDTH = 450;

	inline static const std::vector<QPoint> SPELL_CURVE = {
		{ 10, 78 },
		{ 170, 36 },
		{ 294, 36 },
		{ 450, 80 },
	};

	inline static const float SPELL_MIDDLE = 0.49f;
	inline static const int SPELL_MAXWIDTH = 450;

	inline static const std::vector<QPoint> HERO_CURVE = {
		{ 24, 98 },
		{ 170, 36 },
		{ 294, 36 },
		{ 438, 96 },
	};

	inline static const float HERO_MIDDLE = 0.50f;
	inline static const int HERO_MAXWIDTH = 420;

	inline static const std::vector<QPoint> WEAPON_CURVE = {
		{ 18, 56 },
		{ 66, 56 },
		{ 400, 56 },
		{ 456, 56 },
	};

	inline static const float WEAPON_MIDDLE = 0.56f;
	inline static const int WEAPON_MAXWIDTH = 450;

	inline static const std::vector<QPoint> HP_CURVE = {
		{ 10, 37 },
		{ 110, 37 },
		{ 350, 37 },
		{ 450, 37 },
	};

	inline static const float HP_MIDDLE = 0.54f;
	inline static const int HP_MAXWIDTH = 440;

	int m_maxwidth = 0;
	float m_middle = 0;
	std::vector<QPoint> m_curve;
	Coords m_coords_text;

	std::string m_text;

      public:
	CurvedText(const std::string& text,
		   card_type type,
		   const Coords& coords_text) noexcept
	{
		switch (type) {
		case MINION:
			m_maxwidth = MINION_MAXWIDTH;
			m_middle   = MINION_MIDDLE;
			m_curve	   = MINION_CURVE;
			break;
		case SPELL:
			m_maxwidth = SPELL_MAXWIDTH;
			m_middle   = SPELL_MIDDLE;
			m_curve	   = SPELL_CURVE;
			break;
		case HERO:
			m_maxwidth = HERO_MAXWIDTH;
			m_middle   = HERO_MIDDLE;
			m_curve	   = HERO_CURVE;
			break;
		case WEAPON:
			m_maxwidth = WEAPON_MAXWIDTH;
			m_middle   = WEAPON_MIDDLE;
			m_curve	   = WEAPON_CURVE;
			break;
		default:
			return;
		}
		m_coords_text = coords_text;
		m_text	      = text;
	};
	CurvedText(const Coords& coords) noexcept : RenderObj(coords) { }

	CurvedText(const CurvedText& cpy) noexcept = default;
	~CurvedText() noexcept			   = default;

	void render(QPainter& painter) override
	{
		painter.device()->width();
		QPixmap card_name_pixmap(box_dim_w * 2, box_dim_h);
		card_name_pixmap.fill(Qt::transparent);
		QPainter card_name_painter;
		card_name_painter.begin(&card_name_pixmap);

		card_name_painter.setRenderHint(QPainter::Antialiasing);
		card_name_painter.setRenderHint(
		    QPainter::SmoothPixmapTransform);
		card_name_painter.setRenderHint(QPainter::TextAntialiasing);
		size_t name_length = m_text.length();

		card_name_painter.setPen(QPen(QColor(255, 255, 255)));
		card_name_painter.setBrush(QColor(255, 255, 255));

		QFont font("Belwe Bd BT");

		float font_size = 45;
		font.setPixelSize(font_size);

		render_char render_text[name_length];

		for (size_t i = 0; i < name_length; i++) {
			render_text[i].c = m_text[i];
		}

		QPainterPath path;

		float render_text_width = 0.0f;

		do {
			font_size -= 1;
			font.setPixelSize(font_size);
			QFontMetrics fm(font);
			render_text_width = 0.0f;
			calc_chars_width(render_text, name_length, fm);
			for (size_t i = 0; i < name_length; i++) {
				render_text_width += render_text[i].w;
			}
		} while (render_text_width > m_maxwidth && font_size > 10);

		float left_pos = 0;
		float w	       = render_text_width / m_maxwidth;
		float begin    = m_middle - (w / 2);
		float steps    = w / name_length;

		float px = 0, py = 0, pr = 0, t = 0;

		QPen pen(Qt::black);
		pen.setWidth(10);
		pen.setJoinStyle(Qt::RoundJoin);
		pen.setCapStyle(Qt::RoundCap);

		for (size_t i = 0; i < name_length; i++) {
			if (left_pos == 0) {
				t = begin + (steps * i);
				point_curve(m_curve, t, px, py, pr);
				left_pos = px;
			} else {
				t += 0.01f;
				point_curve(m_curve, t, px, py, pr);

				while (px < left_pos) {
					t += 0.001f;
					point_curve(m_curve, t, px, py, pr);
				}
			}
			left_pos += render_text[i].w;

			path.setFillRule(Qt::WindingFill);
			path.addText(0, 0, font, QString(render_text[i].c));

			card_name_painter.save();
			card_name_painter.translate(px, py);
			card_name_painter.rotate(pr);
			pen.setWidthF(9.0f * (font_size / 50));

			card_name_painter.strokePath(path, pen);

			card_name_painter.fillPath(path, QBrush(Qt::white));

			card_name_painter.restore();
			path.clear();
		}

		card_name_painter.end();

		painter.drawPixmap(m_coords_text.dx(),
				   m_coords_text.dy(),
				   m_coords_text.dw(),
				   m_coords_text.dh(),
				   card_name_pixmap,
				   m_coords_text.sx(),
				   m_coords_text.sy(),
				   m_coords_text.sw(),
				   m_coords_text.sh());
	}
};
}    // namespace mtk
*/
#endif