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

#include "cardnameitem.hpp"

//#include <hsrender/render.hpp>

namespace mtk
{
void
CardNameItem::init(const std::string& texture,
		   const std::shared_ptr<Card>& pcard)
{
	m_ptr = pcard;

	if (pcard == nullptr) {
		// TODO: Draw fallback card!
		return;
	}

	setData(Qt::UserRole, QString(pcard->name().c_str()));

	QPixmap canvas;
	/*
	options opt;
	opt.texture(texture);
	render_card_tile(*pcard, canvas, opt);
*/
	setBackground(canvas);
}

}    // namespace mtk