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

#include "deckwidget.hpp"

#include <QClipboard>

#include "ui_deckwidget.h"

#include <hsdata/deckstring.hpp>
#include <hsmng/gamemanager.hpp>

namespace mtk
{
DeckWidget::DeckWidget(QWidget* parent) : QWidget(parent), ui(new Ui_DeckWidget)
{
	ui->setupUi(this);

	connect(ui->btn_copy,
		&QPushButton::released,
		this,
		&DeckWidget::btn_copy_deck);
}

DeckWidget::~DeckWidget()
{
	delete ui;
}

void
DeckWidget::deck_changed(const std::shared_ptr<Deck>& pdeck)
{
	this->pdeck = pdeck;

	if (pdeck == nullptr) {
		return;
	}

	ui->label_name->setText(pdeck->name().c_str());
	/*
		std::vector<card> cards_deck[pdeck->cards_num];
		int cards_count[pdeck->cards_num];

		for (int i = 0; i < pdeck->cards_num; i++) {
			cards_deck[i]  = pdeck->cards[i].ptr;
			cards_count[i] = pdeck->cards[i].count;
		}

		ui->cardlist->reload_cards(cards_deck, cards_count,
	   pdeck->cards_num);
		*/
}

void
DeckWidget::clear_cards()
{
	ui->cardlist->clear_cards();
}

void
DeckWidget::reload_cards(const PlayCard* cards[], int count[], int items)
{
	if (cards == nullptr) {
		return;
	}
}

void
DeckWidget::btn_copy_deck()
{
	QClipboard* clipboard = QApplication::clipboard();
	std::string strdeck;
	write_deckstring(*pdeck, strdeck);
	clipboard->setText(strdeck.c_str());

	ui->btn_copy->setText("Deck copied!");
}

}    // namespace mtk