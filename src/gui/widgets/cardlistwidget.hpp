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

#ifndef MTK_GUI_CARDLISTWIDGET_H
#define MTK_GUI_CARDLISTWIDGET_H

#include "carddetailwidget.hpp"

#include <QWidget>
#include <hsdata/playcard.hpp>

//! \file

class Ui_CardListWidget;

namespace mtk
{
class CardListWidget : public QWidget
{
	Q_OBJECT
      private:
	Ui_CardListWidget* ui;

      public:
	CardListWidget(QWidget* parent = nullptr);
	~CardListWidget();

	void clear_cards();

	execode draw_card_tile(QPixmap& canvas, const PlayCard* c);

	void set_theme();
	void reload_cards(const std::vector<std::shared_ptr<Card>>& cards);
	// void reload_cards(const deckcard cards[], int items);

      public slots:
	void card_selected(const QModelIndex& current,
			   const QModelIndex& previous);
	void toggle_detail();
};

}    // namespace mtk
#endif	  // DECKWIDGET_H
