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

#include "decklistwidget.hpp"

#include "styleitemdelegate.hpp"

#include "ui_decklistwidget.h"

#include "carddateitem.hpp"
#include "cardiconitem.hpp"
#include "cardnumitem.hpp"
#include "decknameitem.hpp"

#include <hsmng/gamemanager.hpp>

//#include <hsrender/render.hpp>

#include <QDebug>

using namespace mtk;

namespace mtk
{
DeckListWidget::DeckListWidget(QWidget* parent) :
    QWidget(parent), ui(new Ui_DeckListWidget)
{
	ui->setupUi(this);

	connect(ui->table_widget,
		&QTableWidget::cellDoubleClicked,
		this,
		&DeckListWidget::cell_doubleclicked);
	connect(ui->table_widget->selectionModel(),
		&QItemSelectionModel::currentRowChanged,
		this,
		&DeckListWidget::deck_selected);
	connect(ui->filter_widget,
		&FilterWidget::itemClicked,
		this,
		&DeckListWidget::filter_changed);

	ui->table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->table_widget->setItemDelegate(new StyleItemDelegate());
	ui->table_widget->horizontalHeader()->setSectionResizeMode(
	    QHeaderView::Fixed);
	/*
		ui->table_widget->setColumnWidth(0, REF_TILE_HEIGHT);
		ui->table_widget->setColumnWidth(1, REF_TILE_HEIGHT);
		ui->table_widget->setColumnWidth(2, REF_TILE_WIDTH);
		ui->table_widget->setColumnWidth(3, 150);
	*/
	for (size_t i = 0; i < conv_array_class_len; i++) {
		filter_class.insert(conv_array_class[i]);
	}
}

DeckListWidget::~DeckListWidget()
{
	clear_decks();
	QAbstractItemDelegate* tmp = ui->table_widget->itemDelegate();
	ui->table_widget->setItemDelegate(nullptr);
	delete tmp;
	delete ui;
}

void
DeckListWidget::clear_decks()
{
	ui->table_widget->setRowCount(0);
}

void
DeckListWidget::reload_decks(const std::vector<std::shared_ptr<Deck>>& decks)
{
	clear_decks();

	// TODO: Globalize those strings building in non GUI library
	const char* STR_EXT_PNG = ".png";

	std::string str_format;

	for (size_t i = 0; i < conv_array_class_len; i++) {
		/*
		if (strok(conv_array_class_string_number[i])) {
			ui->filter_widget->add_item(
			    conv_array_class[i],
			    GameManager::app_settings->theme_dir() +
				STR_OS_DIR_SEP + "icon_" +
				conv_array_class_string_number[i] +
				STR_EXT_PNG);
		}
		*/
	}

	CardIconItem* item_format;
	QTableWidgetItem* item_class;
	DeckNameItem* item_name;
	CardDateItem* item_create_date;

	QFont font;
	font.setBold(true);
	font.setKerning(true);
	font.setLetterSpacing(QFont::AbsoluteSpacing, -1);
	font.setPixelSize(20);

	int i = 0;
	/*
		options opt;
		opt.asset_folder(GameManager::app_settings->asset_dir());
	*/
	QPixmap class_pixmap;
	QPixmap format_pixmap;

	for (const std::shared_ptr<Deck>& tmp : decks) {
		if (filter_class.find(tmp->hero_class()) ==
		    filter_class.end()) {
			continue;
		}

		item_format	 = new CardIconItem();
		item_class	 = new QTableWidgetItem();
		item_name	 = new DeckNameItem();
		item_create_date = new CardDateItem();
		/*
				render_format_icon(tmp->format(),
						   format_pixmap,
						   opt,
						   REF_TILE_HEIGHT);
				item_format->setBackground(format_pixmap);

						render_class_icon(tmp->hero_class(),
								  class_pixmap,
								  opt,
								  REF_TILE_HEIGHT);
				*/
		item_class->setBackground(class_pixmap);

		// Little hack for sort purpose
		item_name->init(GameManager::app_settings->img_dir() +
				    STR_OS_DIR_SEP + tmp->herocard()->code() +
				    STR_EXT_PNG,
				tmp);

		item_create_date->init(tmp->create_date());

		ui->table_widget->insertRow(i);
		ui->table_widget->setRowHeight(i, 35);
		ui->table_widget->setItem(i, 0, item_format);
		ui->table_widget->setItem(i, 1, item_class);
		ui->table_widget->setItem(i, 2, item_name);
		ui->table_widget->setItem(i, 3, item_create_date);
		// ui->table_widget->setRowHeight(i, REF_TILE_HEIGHT);

		i++;
	}
}

void
DeckListWidget::deck_selected(const QModelIndex& current,
			      const QModelIndex& previous)
{
	QTableWidgetItem* item = ui->table_widget->item(current.row(), 2);
	if (item == nullptr) {
		return;
	}
	/*
		CardNameItem* tmp = dynamic_cast<CardNameItem*>(item);
		const deck* ptr	  = nullptr;

		if (tmp == nullptr) {
			return;
		}

			ptr = (const deck*)tmp->ptr();
			if (ptr == nullptr) {
				return;
			}

			emit deck_changed(ptr);
			*/
}

void
DeckListWidget::filter_changed(QListWidgetItem* item)
{
	filter_class.clear();
	QList<QListWidgetItem*> items = ui->filter_widget->selectedItems();

	for (int i = 0; i < items.count(); i++) {
		filter_class.insert(
		    (card_class)items[i]->data(Qt::UserRole).toInt());
	}
	/*
		QTableWidgetItem* tmp;

		for (int i = 0; i < ui->table_widget->rowCount(); i++) {
			tmp = ui->table_widget->item(i, 1);
			if
	   (filter_deck((card_class)tmp->data(Qt::UserRole).toInt()) ||
			    items.count() == 0) {
				ui->table_widget->showRow(i);
			} else {
				ui->table_widget->hideRow(i);
			}
		}
		*/
}

void
DeckListWidget::cell_doubleclicked(int row, int column)
{
	QTableWidgetItem* item = ui->table_widget->item(row, 2);

	if (item == nullptr) {
		return;
	}

	/*
	CardNameItem* tmp = dynamic_cast<CardNameItem*>(item);
	const deck* ptr	  = nullptr;

	if (tmp == nullptr) {
		return;
	}

		ptr = (const deck*)tmp->get_ptr();
		if (ptr == nullptr) {
			return;
		}

		emit deck_doubleclick(ptr);
		*/
}

}    // namespace mtk