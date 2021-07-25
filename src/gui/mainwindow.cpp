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

#include "mainwindow.hpp"

#include "overlay.hpp"
#include "styleitemdelegate.hpp"

#include "ui_mainwindow.h"

#include <hsdata/deck.hpp>

#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>

#ifdef MTK_DEBUG
#include <QDebug>
#endif

using namespace mtk;

namespace mtk
{
MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui_MainWindow)
{
	ui->setupUi(this);

	playwindow  = std::make_unique<PlayWindow>();
	enemywindow = std::make_unique<EnemyWindow>();

	connect(ui->decklist_widget,
		&DeckListWidget::deck_changed,
		this,
		&MainWindow::deck_selected);
	connect(ui->decklist_widget,
		&DeckListWidget::deck_doubleclick,
		this,
		&MainWindow::deck_doubleclick);

	ui->deck_widget->setVisible(false);

	QApplication::setWheelScrollLines(1);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void
MainWindow::init()
{
	gamemanager	    = std::make_shared<GameManager>();
	execode init_result = gamemanager->init();

	if (init_result == EXE_FALSE) {
		QString tmp_hsexe, tmp_hsuser;

		do {
			tmp_hsexe = QFileDialog::getExistingDirectory(
			    nullptr,
			    "Select Hearthstone Executable directory");
		} while (!tmp_hsexe.endsWith("Hearthstone"));

		do {
			tmp_hsuser = QFileDialog::getExistingDirectory(
			    nullptr,
			    "Select Hearthstone User directory");
		} while (!tmp_hsuser.endsWith("Blizzard/Hearthstone"));

		GameManager::app_settings->hsexe_dir(qPrintable(tmp_hsexe));
		GameManager::app_settings->hsuser_dir(qPrintable(tmp_hsuser));

		init_result = gamemanager->init();
	}

	if (init_result == EXE_ERROR) {
		QMessageBox::critical(this,
				      "Database Error",
				      "Cannot load settings from database!");
		return;
	} else if (init_result == EXE_ERROR) {
		QMessageBox::critical(this,
				      "JSON Cards Error",
				      "Cannot load JSON card database!");
		return;
	} else if (init_result == EXE_NOT_FOUND) {
		QMessageBox::critical(
		    this,
		    "Monospace Error",
		    "Cannot load Monospace, please start Hearthstone first!");
		return;
	}

	std::vector<std::shared_ptr<Deck>> decks = gamemanager->decklist();

	ui->decklist_widget->reload_decks(decks);

	playwindow->init(gamemanager);
	enemywindow->init(gamemanager);

	connect(gamemanager.get(),
		&GameManager::game_start,
		this,
		&MainWindow::show_gameui);
	connect(gamemanager.get(),
		&GameManager::game_end,
		this,
		&MainWindow::hide_gameui);

	return;
}

void
MainWindow::close_app()
{
	this->close();
}

void
MainWindow::deck_selected(const std::shared_ptr<Deck>& pdeck)
{
	ui->deck_widget->setVisible(true);
	if (pdeck != nullptr) {
		ui->deck_widget->deck_changed(pdeck);
	}
}

void
MainWindow::deck_doubleclick(const std::shared_ptr<Deck>& pdeck)
{
	deckwindow = std::make_unique<DeckWindow>(this);

	deckwindow->init(gamemanager, pdeck);
	deckwindow->show();
}

void
MainWindow::show_gameui()
{
	Rect win_geometry;

	execode result = get_window_geometry("Hearthstone", &win_geometry);

	if (result == EXE_OK) {
		playwindow->setGeometry(win_geometry.w - win_geometry.x - 320,
					120,
					320,
					750);
	}

	playwindow->show();

	if (result == EXE_OK) {
		enemywindow->setGeometry(win_geometry.x, 120, 320, 750);
	}

	enemywindow->show();
}

void
MainWindow::hide_gameui()
{
	playwindow->hide();
	enemywindow->hide();
}

}    // namespace mtk