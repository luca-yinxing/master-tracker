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

#ifndef MTK_GUI_DECKWIDGET_H
#define MTK_GUI_DECKWIDGET_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include <hsdata/deck.hpp>
#include <hsdata/playcard.hpp>

#include "cardlistwidget.hpp"

//! \file

class Ui_DeckWidget;

namespace mtk
{
class DeckWidget : public QWidget
{
	Q_OBJECT
      private:
	Ui_DeckWidget* ui;

	std::shared_ptr<Deck> pdeck;

      public:
	DeckWidget(QWidget* parent = nullptr);
	~DeckWidget();

	void clear_cards();
      public slots:
	void deck_changed(const std::shared_ptr<Deck>& pdeck);
	void btn_copy_deck();

      public:
	void set_theme();
	void reload_cards(const PlayCard* cards[], int count[], int items);

      signals:
	void deckChanged();
};

}    // namespace mtk
#endif	  // DECKWIDGET_H
