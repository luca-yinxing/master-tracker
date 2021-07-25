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

#ifndef MTK_GUI_MAINWINDOW_H
#define MTK_GUI_MAINWINDOW_H

#include "deckwindow.hpp"
#include "enemywindow.hpp"
#include "playwindow.hpp"
#include "widgets/deckwidget.hpp"

#include <QMainWindow>

#include <hsmng/gamemanager.hpp>
#include <hsmng/gamewatcher.hpp>
#include <hsmng/logworker.hpp>
//! \file

class Ui_MainWindow;

namespace mtk
{
class MainWindow : public QMainWindow
{
	Q_OBJECT
      private:
	Ui_MainWindow* ui;
	std::unique_ptr<PlayWindow> playwindow;
	std::unique_ptr<DeckWindow> deckwindow;
	std::unique_ptr<EnemyWindow> enemywindow;

	std::shared_ptr<GameManager> gamemanager;

      public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() Q_DECL_OVERRIDE;
	void init();

      public slots:
	void close_app();
	void deck_selected(const std::shared_ptr<Deck>& pdeck);
	void deck_doubleclick(const std::shared_ptr<Deck>& pdeck);
	void show_gameui();
	void hide_gameui();
};

}    // namespace mtk

#endif	  // MAINWINDOW_H
