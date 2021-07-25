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

#include "gamemanager.hpp"

#include <hsdata/deckstring.hpp>
#include <hsdata/playcard.hpp>
#include <hsjson/locale.hpp>

#include <hsjson/jsoncard.hpp>
#include <hsjson/jsondl.hpp>

#include <sys/stat.h>

#ifdef MTK_DEBUG
#include <QDebug>
#endif

using namespace mtk;

namespace mtk
{
std::unique_ptr<settings> GameManager::app_settings = nullptr;

GameManager::GameManager(QObject* parent) : QObject(parent)
{
	db	= nullptr;
	watcher = nullptr;
}

GameManager::~GameManager()
{
#ifdef MTK_DEBUG
	debug_log->close();
	delete debug_log;
#endif
	delete watcher;
};

execode
GameManager::init()
{
	init_settings();

	if (db == nullptr) {
		if (init_database() != EXE_OK) {
			return EXE_ERROR;
		}
	}

	execode retval_settings = db->get_settings(app_settings);

	if (retval_settings == EXE_NOT_FOUND) {
		return EXE_FALSE;
	}

	if (get_locale_language(app_settings->lang_dir(), this->lang) !=
	    EXE_OK) {
		this->lang = EN_US;
	}

#ifdef MTK_DEBUG
	qDebug() << "AUTOMATIC FETCH CLIENT LANG:"
		 << conv_language_string(this->lang);
#endif

	bool update = db->update_hsbuild(app_settings, MTK_HSBUILD) == EXE_OK;

	if (init_carddb(update) != EXE_OK) {
		return EXE_CANT_OPEN;
	}

	if (init_monospace() != EXE_OK) {
		return EXE_NOT_FOUND;
	}

	init_gamewatcher();

	init_logworker_list();
	init_log_timer();
	init_deck_pick_timer();

	return EXE_OK;
}

execode
GameManager::init_settings()
{
	if (app_settings == nullptr) {
		app_settings = std::make_unique<settings>();
	}

	return db->set_settings_default(app_settings);
}

execode
GameManager::init_database()
{
	db = new DBManager;

	return db->init(app_settings->local_dir());
}

execode
GameManager::init_monospace()
{
	if (init_hsms(monospace_addr) != EXE_OK) {
		return EXE_NOT_FOUND;
	}

	get_player_deck_list(monospace_addr, m_decklist);

	std::string herocard_code;
	std::shared_ptr<Card> ptr_herocard;
	std::vector<DeckCard> tmp_cardlist;
	std::shared_ptr<Card> ptr_card;

	for (std::shared_ptr<Deck>& tmp_deck : m_decklist) {
		herocard_code = tmp_deck->herocard()->code();
		ptr_herocard  = carddb.find_by_code(herocard_code);

		if (ptr_herocard == nullptr) {
#ifdef MTK_DEBUG
			qDebug() << "ERROR: Cannot find herocard at code"
				 << herocard_code.c_str();
#endif
		} else {
			tmp_deck->herocard(ptr_herocard);
		}
		tmp_cardlist.reserve(tmp_deck->cards().size());
		for (DeckCard& tmp_card : tmp_deck->cards()) {
			ptr_card = carddb.find_by_code(tmp_card.code());

			if (ptr_card == nullptr) {
#ifdef MTK_DEBUG
				qDebug() << "ERROR: Cannot find card at code"
					 << tmp_card.code().c_str();
#endif
			} else {
				tmp_card.ptr(ptr_card);
			}
		}

		db->add_deck(tmp_deck);
	}

	return EXE_OK;
}

execode
GameManager::init_logworker_list()
{
#ifdef MTK_DEBUG
	std::string str_file_log = app_settings->local_dir() + "/debug.log";
	debug_log		 = new QFile(str_file_log.c_str());
	debug_log->open(QIODevice::WriteOnly);
#endif

	for (size_t i = 0; i < LOG_COMPONENT_ENABLED_NUM; i++) {
		logworker_list[i].init(app_settings->hslog_dir(),
				       LOG_COMPONENT_ENABLED[i]);

		connect(&logworker_list[i],
			&LogWorker::new_line,
			watcher,
			&GameWatcher::process_logline);
#ifdef MTK_DEBUG
		connect(&logworker_list[i],
			&LogWorker::new_line,
			this,
			&GameManager::debug_log_line);
#endif
	}

	return EXE_OK;
}

#ifdef MTK_DEBUG
void
GameManager::debug_log_line(log_component component,
			    const QString& line,
			    qint64 numline,
			    qint64 seek)
{
	if (debug_log->isOpen()) {
		QTextStream stream(debug_log);
		stream << line;
	}
}

#endif

execode
GameManager::init_carddb(bool update)
{
	void* curl_handle = nullptr;

	if (update) {
		curl_handle = init_json_curl();

		download_json_cards(curl_handle,
				    app_settings->jsoncards(),
				    app_settings->hsbuild(),
				    "all",
				    true);
	}

	CardQuery query;
	query.lang(this->lang);

	if (get_json_carddb(app_settings->jsoncards(), query, carddb) !=
	    EXE_OK) {
		return EXE_ERROR;
	}

	if (curl_handle && update) {
		QString dir_tile;

		std::vector<std::string> code_list;
		carddb.to_codelist(code_list);

		if (os_mkdir(app_settings->img_dir()) != EXE_OK) {
			return EXE_ERROR_OS;
		}

		download_json_art(curl_handle,
				  app_settings->img_dir(),
				  code_list);

		free_json_curl(curl_handle);
	}

	return EXE_OK;
}

execode
GameManager::init_gamewatcher()
{
	watcher = new GameWatcher(this);

	connect(watcher,
		&GameWatcher::start_game,
		this,
		&GameManager::start_game);
	// connect(watcher, &GameWatcher::player_name, this,
	// &GameManager::player_name);
	connect(watcher,
		&GameWatcher::screen_mode_changed,
		this,
		&GameManager::screen_mode_changed);
	connect(watcher, &GameWatcher::end_game, this, &GameManager::end_game);
	connect(watcher, &GameWatcher::coin_id, this, &GameManager::coin_id);
	connect(watcher, &GameWatcher::new_turn, this, &GameManager::new_turn);
	connect(watcher,
		&GameWatcher::player_carddraw,
		this,
		&GameManager::player_carddraw);
	connect(watcher,
		&GameWatcher::enemy_carddraw,
		this,
		&GameManager::enemy_carddraw);
	connect(watcher,
		&GameWatcher::card_return_deck,
		this,
		&GameManager::card_return_deck);
	connect(watcher,
		&GameWatcher::card_played,
		this,
		&GameManager::card_played);
	connect(watcher,
		&GameWatcher::created_card,
		this,
		&GameManager::created_card);
	connect(watcher,
		&GameWatcher::updated_card,
		this,
		&GameManager::updated_card);
	connect(watcher,
		&GameWatcher::show_entity,
		this,
		&GameManager::show_entity);
	connect(watcher,
		&GameWatcher::update_entity,
		this,
		&GameManager::update_entity);
	connect(watcher,
		&GameWatcher::tag_change,
		this,
		&GameManager::tag_change);

	return EXE_OK;
}

execode
GameManager::init_log_timer()
{
	log_timer = new QTimer(this);
	connect(log_timer, &QTimer::timeout, this, &GameManager::update_log);
	log_timer->start(LOG_COMPONENT_TIMER_NUM);

	return EXE_OK;
}

execode
GameManager::init_deck_pick_timer()
{
	deck_pick_timer = new QTimer(this);
	connect(deck_pick_timer,
		&QTimer::timeout,
		this,
		&GameManager::update_deck_pick);

	return EXE_OK;
}

void
GameManager::update_log()
{
	for (size_t i = 0; i < LOG_COMPONENT_ENABLED_NUM; i++) {
		if (logworker_list[i].open() == EXE_OK) {
			logworker_list[i].read_log();
		}
	}
}

void
GameManager::update_deck_pick()
{
	int64_t tmp_deck_pick = 0;
	if (get_player_deck_pick_id(monospace_addr, tmp_deck_pick) != EXE_OK) {
		return;
	}
	if (tmp_deck_pick == deck_pick) {
		return;
	}

	deck_pick = tmp_deck_pick;

#ifdef MTK_DEBUG
	std::shared_ptr<Deck> tmp_deck = nullptr;
	for (const std::shared_ptr<Deck>& pdeck : m_decklist) {
		if (pdeck->id() == deck_pick) {
			tmp_deck = pdeck;
			break;
		}
	}

	qDebug() << "CURRENT DECK PICK NAME:" << tmp_deck->name().c_str();
#endif
}

void
GameManager::start_game()
{
	m_player_user = std::make_shared<Player>();
	m_player_oppo = std::make_shared<Player>();

	for (const std::shared_ptr<Deck>& pdeck : m_decklist) {
		if (pdeck->id() == deck_pick) {
			m_player_user->pdeck(pdeck);
			break;
		}
	}

	game_started = true;
	current_turn = 0;

	std::vector<std::shared_ptr<PlayerInfo>> pinfo_list;

	get_player_info(monospace_addr, pinfo_list);

	if (pinfo_list[0]->side == FRIENDLY) {
		m_player_user->info(pinfo_list[0]);
		m_player_oppo->info(pinfo_list[1]);
	} else if (pinfo_list[0]->side == OPPOSING) {
		m_player_user->info(pinfo_list[1]);
		m_player_oppo->info(pinfo_list[0]);
	}

	m_player_user->id(1);
	m_player_oppo->id(2);

	QTimer::singleShot(200, this, &GameManager::player_name);

	emit user_changed(m_player_user);
	emit game_start();

#ifdef MTK_DEBUG
	qDebug() << "Game is started!";
#endif
}

void
GameManager::player_name()
{
	std::vector<std::shared_ptr<PlayerInfo>> pinfo_list;

	get_player_info(monospace_addr, pinfo_list);

	if (pinfo_list[0]->side == FRIENDLY) {
		m_player_user->info(pinfo_list[0]);
		m_player_oppo->info(pinfo_list[1]);
	} else if (pinfo_list[0]->side == OPPOSING) {
		m_player_user->info(pinfo_list[1]);
		m_player_oppo->info(pinfo_list[0]);
	}
#ifdef MTK_DEBUG
	qDebug() << "Update names!";
	qDebug() << "User Player:"
		 << QString::fromUtf16(m_player_user->info()->name) << "id"
		 << m_player_user->id();
	qDebug() << "Oppo Player:"
		 << QString::fromUtf16(m_player_oppo->info()->name) << "id"
		 << m_player_user->id();
#endif
}

void
GameManager::screen_mode_changed(screen_state prev, screen_state curr)
{
	if (curr == SS_RANKED || curr == SS_FRIENDLY || curr == SS_ADVENTURE) {
		deck_pick_timer->start(DECK_PICK_TIMER_NUM);
	} else {
		deck_pick_timer->stop();
	}

#ifdef MTK_DEBUG
	qDebug() << "SCREEN STATE PREV:" << conv_screen_state_string(prev);
	qDebug() << "SCREEN STATE CURR:" << conv_screen_state_string(curr);
#endif
}

void
GameManager::end_game(QString winner_name, bool is_tied)
{
	if (!game_started) {
		return;
	}

	int pos	    = winner_name.lastIndexOf(QChar('#'));
	winner_name = winner_name.left(pos);

	char buffer_oppo_name[DEF_BUFFER_STR];
	to_utf8(m_player_oppo->info()->name, buffer_oppo_name, DEF_BUFFER_STR);
	char buffer_user_name[DEF_BUFFER_STR];
	to_utf8(m_player_user->info()->name, buffer_user_name, DEF_BUFFER_STR);

	game_result result = TIED;

	// FIXME: result is not working while cannot take properly the player
	// name
	if (winner_name == buffer_user_name) {
		result = WIN;
	} else if (winner_name == buffer_oppo_name) {
		result = LOST;
	}
	card_class oppo_class = m_player_oppo->pdeck() == nullptr ?
					  NEUTRAL :
					  m_player_oppo->pdeck()->hero_class();
	db->add_match(result,
		      m_player_user->pdeck(),
		      buffer_oppo_name,
		      oppo_class);
#ifdef MTK_DEBUG
	if (result == WIN) {
		qDebug() << buffer_user_name << "won the match!";
	} else if (result == LOST) {
		qDebug() << buffer_oppo_name << "won the match!";
	} else {
		qDebug() << "Tied match!" << winner_name;
	}
#endif

	m_player_user.reset();
	m_player_oppo.reset();

	emit user_changed(nullptr);
	emit oppo_changed(nullptr);
	emit game_end();

	game_started = false;
}

void
GameManager::coin_id(int id, QString code)
{
#ifdef MTK_DEBUG
	if (code.isEmpty()) {
		qDebug() << "Added coin to oppo:" << id;
	} else {
		qDebug() << "Added coin to user:" << id << code;
	}
#endif
	/*
	PlayCard tmp;
	set_playcard(&tmp);

	tmp.id = id;
	tmp.turn = 0;
	tmp.zone = HAND;

	if (code.isEmpty()) {
	    add_player_cards(oppo, &tmp);
	} else {
	    const char* code_str = qPrintable(code);
	    set_playcard_code(&tmp, code_str);
	    tmp.ptr = find_map_card_by_code(carddb, code_str);

	    add_player_cards(user, &tmp);
	}

	emit player_cards_changed(user->cards, user->cards_num);
	emit enemy_cards_changed(oppo->cards, oppo->cards_num);
    */
}

void
GameManager::new_turn(bool is_player_turn, int num)
{
	current_turn = num / 2 + (num % 2 != 0);

#ifdef MTK_DEBUG
	qDebug() << "New turn:" << current_turn << num << is_player_turn;
#endif
}

void
GameManager::player_carddraw(QString code, int id)
{
#ifdef MTK_DEBUG
	qDebug() << "Added card to user:" << id << code;
#endif
	/*
	PlayCard* tmp = nullptr;
	for (int i = 0; i < user->cards_num; i++) {
	    if (strcmp(user->cards[i].code, qPrintable(code)) == 0 &&
	user->cards[i].id == -1) { tmp = &user->cards[i];
	    }
	}

	if (tmp == nullptr) {
	    return;
	}

	tmp->turn = current_turn;
	tmp->id = id;
	tmp->zone = HAND;

	emit player_cards_changed(user->cards, user->cards_num);
	//TODO: Error
	return;
	*/
}

void
GameManager::enemy_carddraw(int id, int turn, bool special, QString code)
{
#ifdef MTK_DEBUG
	qDebug() << "Added card to oppo:" << id << code;
#endif

	PlayCard tmp;
	tmp.id(id);
	tmp.turn(current_turn);
	tmp.zone(HAND);

	m_player_oppo->cards_add(tmp);

	emit oppo_changed(m_player_oppo);
	return;
}

void
GameManager::card_return_deck(QString code, int id)
{
	// REMEMBER: code "CORE_EX1_567" != code "EX1_567"
	// not using code parameter

	m_player_user->cards_change_zone(DECK, id);

	emit user_changed(m_player_user);
	// TODO: Error
	return;
}

void
GameManager::card_played(int player, int id, QString code, bool discard)
{
	/*
    #ifdef MTK_DEBUG
	qDebug() << "Played card:" << player << id << code << discard;
    #endif
	PlayCard* tmp = nullptr;
	if (player == 0) {
	    tmp = get_player_card(user, id);
	} else {
	    tmp = get_player_card(oppo, id);
	}

	if (tmp == nullptr) {
	    return;
	}

	tmp->zone = PLAY;
	*/
}

void
GameManager::created_card(int id, QString code)
{
#ifdef MTK_DEBUG
	qDebug() << "Created card user:" << id << code;
#endif
	// Skipping enemy heropower
	// Created card user: 67 "HERO_08bp"
	if (code.startsWith("HERO_")) {
		return;
	}

	const char* code_str = qPrintable(code);
	PlayCard tmp;
	tmp.id(id);
	tmp.code(code_str);
	tmp.turn(current_turn);
	tmp.ptr(carddb.find_by_code(code_str));
	// Put the coin in hand directly
	if (code.contains("COIN")) {
		tmp.zone(HAND);
	} else {
		tmp.zone(SETASIDE);
	}
	// tmp.created_by = true;

	m_player_user->cards_add(tmp);

	emit user_changed(m_player_user);
}

void
GameManager::updated_card(int player, int id, QString code)
{
#ifdef MTK_DEBUG
	qDebug() << "Updated card user:" << id << code << player;
#endif
	if (player == m_player_oppo->id()) {
		if (code.startsWith("HERO_")) {
			Deck tmp;
			const std::shared_ptr<mtk::Card> tmp_herocard =
			    carddb.find_by_code(qPrintable(code));
			tmp.herocard(tmp_herocard);
			m_player_oppo->pdeck(tmp);
#ifdef MTK_DEBUG
			qDebug() << "Oppo class:"
				 << conv_class_string(
					m_player_oppo->pdeck()->hero_class());
#endif
			return;
		}

		PlayCard tmp;
		tmp.turn(current_turn);
		tmp.id(id);

		m_player_oppo->cards_add(tmp);
	}
}

void
GameManager::show_entity(int id, int player_id, QString code)
{
#ifdef MTK_DEBUG
	qDebug() << "Show entity:" << id << code << player_id;
#endif
	if (player_id == m_player_user->id()) {
		// TODO: Ok or not with shared?
		std::shared_ptr<PlayCard> tmp = nullptr;
		for (std::shared_ptr<PlayCard>& c : m_player_user->cards()) {
			if (c->code() == qPrintable(code) && c->id() == 0) {
				tmp = c;
				break;
			}
		}

		if (tmp == nullptr) {
			return;
		}

		tmp->turn(current_turn);
		tmp->id(id);

		emit user_changed(m_player_user);
	} else if (player_id == m_player_oppo->id()) {
		std::shared_ptr<PlayCard> tmp = nullptr;
		for (std::shared_ptr<PlayCard>& c : m_player_oppo->cards()) {
			if (c->code() == qPrintable(code) && c->id() == 0) {
				tmp = c;
				break;
			}
		}

		if (tmp == nullptr) {
			return;
		}

		tmp->ptr(carddb.find_by_code(qPrintable(code)));
		tmp->code(qPrintable(code));

		emit oppo_changed(m_player_oppo);
	}
}

void
GameManager::update_entity(int id, QString zone, int player_id, QString code)
{
#ifdef MTK_DEBUG
	qDebug() << "Update entity:" << id << code << zone << player_id;
#endif
	std::shared_ptr<Player> p     = nullptr;
	std::shared_ptr<PlayCard> tmp = nullptr;

	if (player_id == m_player_user->id()) {
		p = m_player_user;
	} else if (player_id == m_player_oppo->id()) {
		p = m_player_oppo;
	} else {
		return;
	}

	// TODO: Test if is working for polyforms
	if (zone == "PLAY") {
		tmp = p->cards_at(id);
		tmp->code(qPrintable(code));
	}
}

void
GameManager::tag_change(int id,
			QString code,
			QString tag,
			QString value,
			QString zone,
			int player_id)
{
#ifdef MTK_DEBUG
	qDebug() << "Card Tag change:" << id << code << tag << value << zone
		 << player_id;
#endif
	std::shared_ptr<PlayCard> tmp = nullptr;
	if (player_id == m_player_user->id()) {
		tmp = m_player_user->cards_at(id);
	} else if (player_id == m_player_oppo->id()) {
		// TODO: Generate the card first!!!
		tmp = m_player_oppo->cards_at(id);
	} else {
		return;
	}

	if (tmp == nullptr) {
		return;
	}

	// TODO: Add support for secret zone?
	if (tag == "ZONE") {
		tmp->zone(conv_string_zone(qPrintable(value)));
		tmp->turn(current_turn);
	}

	if (player_id == m_player_user->id()) {
		emit user_changed(m_player_user);
	} else if (player_id == m_player_oppo->id()) {
		emit oppo_changed(m_player_oppo);
	}
}
}    // namespace mtk