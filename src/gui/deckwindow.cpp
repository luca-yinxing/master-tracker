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

#include "deckwindow.hpp"

#include "styleitemdelegate.hpp"

#include "ui_deckwindow.h"

#include <hsdata/deck.hpp>

#include <QDateTime>
#include <QMessageBox>

using namespace mtk;

namespace mtk
{
DeckWindow::DeckWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui_DeckWindow)
{
	ui->setupUi(this);

	ui->table_widget->setIconSize(QSize(40, 40));
	ui->table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

DeckWindow::~DeckWindow() { }

void
DeckWindow::init(const std::shared_ptr<GameManager>& gm,
		 const std::shared_ptr<Deck>& pdeck)
{
	gamemanager = gm;

	int result[conv_array_class_len][3];

	for (size_t i = 0; i < conv_array_class_len; i++) {
		result[i][0] = 0;
		result[i][1] = 0;
		result[i][2] = 0;
	}

	gamemanager->get_db()->get_deck_stat(pdeck->id(),
					     result,
					     conv_array_class_len);

	QString dir = GameManager::app_settings->local_dir().c_str();
	QTableWidgetItem* tmp;
	QTableWidgetItem* win_item;
	QTableWidgetItem* lost_item;
	QTableWidgetItem* winrate_item;

	ui->table_widget->setColumnCount(conv_array_class_len - 2);
	float lost    = 0;
	float win     = 0;
	float winrate = 0;

	// Removing INVALID_CLASS (first element) and NEUTRAL (last element) in
	// this search
	for (size_t i = 1; i < conv_array_class_len - 1; i++) {
		lost	= 0;
		win	= 0;
		winrate = 0;

		if (strok(conv_array_class_string_number[i])) {
			tmp = new QTableWidgetItem();
			tmp->setIcon(QIcon(dir + "/theme/icon_" +
					   conv_array_class_string_number[i] +
					   ".png"));
			ui->table_widget->setHorizontalHeaderItem(i - 1, tmp);
		}

		for (size_t c = 0; c < conv_array_class_len; c++) {
			if (conv_array_class[i] == (card_class)result[c][0]) {
				win  = result[c][1];
				lost = result[c][2];
				break;
			}
		}

		win_item     = new QTableWidgetItem();
		lost_item    = new QTableWidgetItem();
		winrate_item = new QTableWidgetItem();

		win_item->setText(QString::number(win));
		lost_item->setText(QString::number(lost));
		if (win == 0) {
			winrate = 0;
		} else if (win != 0 && lost == 0) {
			winrate = win;
		} else {
			winrate = win / lost;
		}
		winrate_item->setText(QString::number(winrate));

		ui->table_widget->setItem(0, i - 1, win_item);
		ui->table_widget->setItem(1, i - 1, lost_item);
		ui->table_widget->setItem(2, i - 1, winrate_item);
	}
	ui->table_widget->resizeColumnsToContents();
	return;
}

}    // namespace mtk