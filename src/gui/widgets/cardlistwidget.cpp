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

#include "cardlistwidget.hpp"

#include "styleitemdelegate.hpp"

#include "ui_cardlistwidget.h"

#include "cardnameitem.hpp"
#include "cardnumitem.hpp"

#include <hsmng/gamemanager.hpp>

using namespace mtk;

namespace mtk
{
CardListWidget::CardListWidget(QWidget* parent) :
    QWidget(parent), ui(new Ui_CardListWidget)
{
	ui->setupUi(this);

	ui->card_detail->setVisible(false);

	connect(ui->table_widget->selectionModel(),
		&QItemSelectionModel::currentRowChanged,
		this,
		&CardListWidget::card_selected);
	connect(ui->btn_detail,
		&QPushButton::released,
		this,
		&CardListWidget::toggle_detail);

	ui->table_widget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->table_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->table_widget->setMouseTracking(true);
	ui->table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->table_widget->setItemDelegate(new StyleItemDelegate());
	ui->table_widget->horizontalHeader()->setSectionResizeMode(
	    QHeaderView::Fixed);

	ui->table_widget->setColumnWidth(0, 35);
	ui->table_widget->setColumnWidth(1, 218 - 35);
	ui->table_widget->setColumnWidth(2, 35);

	set_theme();
}

CardListWidget::~CardListWidget()
{
	clear_cards();
	QAbstractItemDelegate* tmp = ui->table_widget->itemDelegate();
	ui->table_widget->setItemDelegate(nullptr);
	delete tmp;
	delete ui;
}

void
CardListWidget::set_theme()
{
	QFont font("Luckiest Guy");
	font.setPixelSize(20);
	ui->table_widget->setFont(font);
	/*
	ui->table_widget->setFrameShape(QFrame::NoFrame);
	ui->table_widget->setIconSize(10 * CARD_SIZE);
	ui->table_widget->setSizePolicy(QSizePolicy::Expanding,
	QSizePolicy::Expanding);
	ui->table_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->table_widget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
	ui->table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->table_widget->setFocusPolicy(Qt::NoFocus);
	/*
	table_widget->setStyleSheet(
	    "QListView{background-color: transparent; outline: 0;}"
	    "QListView::item{padding: 0px;}"
	    "QListView::item:selected {background: "
	    + ThemeHandler::bgSelectedItemListColor() + "; "
							"color: "
	    + ThemeHandler::fgSelectedItemListColor() + ";}");
	    */
}

void
CardListWidget::clear_cards()
{
	ui->table_widget->setRowCount(0);
}

void
CardListWidget::reload_cards(const std::vector<std::shared_ptr<Card>>& cards)
{
	clear_cards();
	/*
		QString dir = GameManager::app_settings->img_dir;

		CardNumItem* item_mana;
		CardNameItem* item_name;
		CardNumItem* item_num;

		QFont font;
		font.setBold(true);
		font.setKerning(true);
		font.setLetterSpacing(QFont::AbsoluteSpacing, -1);
		font.setPixelSize(20);

		char buffer_img[DEF_BUFFER_STR_LONG];
		const char* IMG_EXT = ".jpg";

		const char* TMP_STR_LIST[] = {
	   GameManager::app_settings->img_dir, STR_OS_DIR_SEP, nullptr, IMG_EXT
	   };

		char buffer_fallback[DEF_BUFFER_STR_LONG];
		strconcat(buffer_fallback,
			  GameManager::app_settings->theme_dir,
			  "/unknown.png");
		// TODO: global and translate?
		const char* UNKNOWN_CARD_NAME = "Unknown card";

		for (int i = 0; i < items; i++) {
			item_mana = new CardNumItem();
			item_name = new CardNameItem();
			item_num  = new CardNumItem();

			if (cards[i] != nullptr) {
				item_mana->init(cards[i]->cost,
						COMMON,
						QColor(46, 82, 119));

				card_class c = INVALID_CLASS;

				if (cards[i]->classes_num == 1) {
					c = cards[i]->classes[0];
				}

				TMP_STR_LIST[2] = cards[i]->code;
				strconcat(buffer_img,
					  TMP_STR_LIST,
					  sizeof(TMP_STR_LIST) / sizeof(char*));
				// Little hack for sort purpose
				item_name->init(buffer_img, cards[i]);

				if (count != nullptr) {
					item_num->init(count[i],
						       cards[i]->rarity,
						       QColor(47, 47, 47));
				}
			} else {
				item_mana->init(0, COMMON, QColor(46, 82, 119));
				item_name->init(buffer_fallback, nullptr);
				item_num->init(count[i], COMMON, QColor(47, 47,
	   47));
			}

			ui->table_widget->insertRow(i);
			ui->table_widget->setRowHeight(i, 35);
			ui->table_widget->setItem(i, 0, item_mana);
			ui->table_widget->setItem(i, 1, item_name);
			ui->table_widget->setItem(i, 2, item_num);
		}
		*/
}

void
CardListWidget::card_selected(const QModelIndex& current,
			      const QModelIndex& previous)
{
	QTableWidgetItem* item = ui->table_widget->item(current.row(), 1);
	if (item == nullptr) {
		return;
	}
	CardNameItem* tmp	  = dynamic_cast<CardNameItem*>(item);
	std::shared_ptr<Card> ptr = nullptr;

	if (tmp == nullptr) {
		return;
	}

	ptr = tmp->ptr();
	if (ptr == nullptr) {
		return;
	}

	ui->card_detail->init(ptr);
}

void
CardListWidget::toggle_detail()
{
	if (ui->card_detail->isVisible()) {
		ui->card_detail->setVisible(false);
		ui->btn_detail->setText("Show details");
	} else {
		ui->card_detail->setVisible(true);
		ui->btn_detail->setText("Hide details");
	}
}
}    // namespace mtk