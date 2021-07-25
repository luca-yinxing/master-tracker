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

#include "enemywindow.hpp"

#include "ui_enemywindow.h"

#include <hsdata/deck.hpp>

using namespace mtk;

namespace mtk
{
EnemyWindow::EnemyWindow(QWidget* parent) :
    QMainWindow(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint),
    ui(new Ui_EnemyWindow)

{
	ui->setupUi(this);
}

EnemyWindow::~EnemyWindow() { }

void
EnemyWindow::init(const std::shared_ptr<GameManager>& gm)
{
	if (gm == nullptr) {
		return;
	}
	gamemanager = gm;

	connect(gamemanager.get(),
		&GameManager::oppo_changed,
		this,
		&EnemyWindow::enemy_cards_changed);

	return;
}

void
EnemyWindow::enemy_cards_changed(const std::shared_ptr<Player>& p)
{
	if (p == nullptr) {
		ui->tab_deck->clear_cards();
		ui->tab_hand->clear_cards();
		ui->tab_graveyard->clear_cards();
		ui->tab_board->clear_cards();
		return;
	}
	/*
		int count_deck	    = 0;
		int count_hand	    = 0;
		int count_graveyard = 0;
		int count_board	    = 0;

		for (int i = 0; i < items; i++) {
			if (cards[i].zone == DECK) {
				count_deck++;
			} else if (cards[i].zone == HAND) {
				count_hand++;
			} else if (cards[i].zone == GRAVEYARD) {
				count_graveyard++;
			} else if (cards[i].zone == PLAY) {
				count_board++;
			}
		}

		const card* cards_deck[count_deck];
		const card* cards_hand[count_hand];
		const card* cards_graveyard[count_graveyard];
		const card* cards_board[count_board];
		int index_deck	    = 0;
		int index_hand	    = 0;
		int index_graveyard = 0;
		int index_board	    = 0;
		int duplicated_deck[count_deck];
		int turn_hand[count_hand];
		int turn_graveyard[count_graveyard];
		int turn_board[count_board];
		bool is_duplicated = false;

		for (int i = 0; i < items; i++) {
			if (cards[i].zone == DECK) {
				for (int dupl = 0; dupl < index_deck; dupl++) {
					if (cards[i].ptr == cards_deck[dupl]) {
						duplicated_deck[dupl]++;
						is_duplicated = true;
						break;
					}
				}
				if (is_duplicated) {
					is_duplicated = false;
				} else {
					duplicated_deck[index_deck] = 1;
					cards_deck[index_deck]	    =
	   cards[i].ptr; index_deck++;
				}
			} else if (cards[i].zone == HAND) {
				turn_hand[index_hand]  = cards[i].turn;
				cards_hand[index_hand] = cards[i].ptr;
				index_hand++;
			} else if (cards[i].zone == GRAVEYARD) {
				turn_graveyard[index_graveyard]	 =
	   cards[i].turn; cards_graveyard[index_graveyard] = cards[i].ptr;
				index_graveyard++;
			} else if (cards[i].zone == PLAY) {
				turn_board[index_board]	 = cards[i].turn;
				cards_board[index_board] = cards[i].ptr;
				index_board++;
			}
		}

		ui->tab_deck->reload_cards(cards_deck, duplicated_deck,
	   index_deck); ui->tab_hand->reload_cards(cards_hand, turn_hand,
	   index_hand); ui->tab_graveyard->reload_cards(cards_graveyard,
						turn_graveyard,
						index_graveyard);
		ui->tab_board->reload_cards(cards_board, turn_board,
	   index_board);
		*/
}

}    // namespace mtk