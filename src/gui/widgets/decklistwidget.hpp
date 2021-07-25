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

#ifndef MTK_GUI_DECKLISTWIDGET_H
#define MTK_GUI_DECKLISTWIDGET_H

#include "carddetailwidget.hpp"

#include <QListWidgetItem>
#include <QWidget>

#include <hsdata/playcard.hpp>

#include <set>

//! \file

class Ui_DeckListWidget;

namespace mtk
{
class DeckListWidget : public QWidget
{
	Q_OBJECT
      private:
	Ui_DeckListWidget* ui;
	std::set<card_class> filter_class;

      public:
	DeckListWidget(QWidget* parent = nullptr);
	~DeckListWidget();

	void clear_decks();

	void reload_decks(const std::vector<std::shared_ptr<Deck>>& decks);

      public slots:
	void deck_selected(const QModelIndex& current,
			   const QModelIndex& previous);
	void filter_changed(QListWidgetItem* item);
	void cell_doubleclicked(int row, int column);
      signals:
	void deck_changed(const std::shared_ptr<Deck>& pdeck);
	void deck_doubleclick(const std::shared_ptr<Deck>& pdeck);
};

}    // namespace mtk
#endif	  // DECKWIDGET_H
