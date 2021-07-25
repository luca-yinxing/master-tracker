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

#ifndef MT_HSMNG_GAMEMANAGER_H
#define MT_HSMNG_GAMEMANAGER_H

#include "dbmanager.hpp"
#include "gamewatcher.hpp"
#include "logworker.hpp"

#include <QObject>
#include <QTimer>

#include <hsdata/cardmap.hpp>
#include <hsdata/player.hpp>

#include <hsms/player.hpp>

//! \file

namespace mtk
{
const log_component LOG_COMPONENT_ENABLED[] = {
	LOADINGSCREEN,
	POWER,
	//    ZONE,
};

constexpr const size_t LOG_COMPONENT_ENABLED_NUM =
    sizeof(LOG_COMPONENT_ENABLED) / sizeof(log_component);

constexpr const size_t LOG_COMPONENT_TIMER_NUM = 250;

constexpr const size_t DECK_PICK_TIMER_NUM = 500;

constexpr const int CARD_HAND_SIZE = 16;

class GameManager : public QObject
{
	Q_OBJECT
      private:
	std::shared_ptr<Player> m_player_user;
	std::shared_ptr<Player> m_player_oppo;

	DBManager* db = nullptr;

	language lang = INVALID_LANGUAGE;

	GameWatcher* watcher = nullptr;

	CardMap carddb;

	LogWorker logworker_list[LOG_COMPONENT_ENABLED_NUM];
	QTimer* log_timer = nullptr;

	int64_t deck_pick	= 0;
	QTimer* deck_pick_timer = nullptr;

	uint32_t monospace_addr = ADDR_ZERO;

	std::vector<std::shared_ptr<Deck>> m_decklist;

	bool game_started = false;
	int current_turn  = -1;

#ifdef MTK_DEBUG
	QFile* debug_log;
#endif

      public:
	static std::unique_ptr<settings> app_settings;

	GameManager(QObject* parent = nullptr);
	~GameManager();

	execode init();

	const DBManager* get_db() const noexcept { return db; }

	const std::vector<std::shared_ptr<Deck>>& decklist() const noexcept
	{
		return m_decklist;
	}

	std::shared_ptr<Deck> get_deck(size_t i) const noexcept
	{
		return m_decklist[i];
	}

      private:
	execode init_settings();
	execode init_database();
	execode init_monospace();
	execode init_logworker_list();
	execode init_carddb(bool update);
	execode init_gamewatcher();
	execode init_log_timer();
	execode init_deck_pick_timer();

      public slots:
#ifdef MTK_DEBUG
	void debug_log_line(log_component component,
			    const QString& line,
			    qint64 numline,
			    qint64 seek);
#endif
	void update_log();
	void update_deck_pick();

	void start_game();
	void player_name();
	void screen_mode_changed(screen_state prev, screen_state curr);
	void end_game(QString winner_name, bool is_tied);

	void coin_id(int id, QString code);
	void new_turn(bool is_player_turn, int num);

	void player_carddraw(QString code, int id);
	void enemy_carddraw(int id, int turn, bool special, QString code);

	void card_return_deck(QString code, int id);

	void card_played(int player, int id, QString code, bool discard);

	void created_card(int id, QString code);
	void updated_card(int player_id, int id, QString code);

	void show_entity(int id, int player_id, QString code);
	void update_entity(int id, QString zone, int player_id, QString code);

	void tag_change(int id,
			QString code,
			QString tag,
			QString value,
			QString zone,
			int player);

      signals:
	void user_changed(const std::shared_ptr<Player>& puser);
	void oppo_changed(const std::shared_ptr<Player>& poppo);
	void game_start();
	void game_end();
};

}    // namespace mtk

#endif