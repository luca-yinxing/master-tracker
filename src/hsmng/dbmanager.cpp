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

#include "dbmanager.hpp"

#include "sqlquery.hpp"

#include <utils/utils.hpp>

#include <hsdata/conv.hpp>
#include <hsdata/deckstring.hpp>

#include <QDir>
#include <QSqlQuery>

#include <QDateTime>

#ifdef MTK_DEBUG
#include <QDebug>
#include <QSqlError>
#endif

namespace mtk
{
DBManager::DBManager() { }

DBManager::~DBManager()
{
	db.close();
}

execode
DBManager::init(const std::string& path)
{
	if (path.empty()) {
		return EXE_INVALID_ARGS;
	}

	std::string dbfile = path + STR_OS_DIR_SEP + DATABASE_FILE;

	bool exists = file_exists(dbfile);

	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(dbfile.c_str());

	if (!db.open()) {
		return EXE_CANT_OPEN;
	}

	if (!exists) {
		QSqlQuery query;
		if (!query.exec(SQL_SETTINGS_TABLE_CREATE)) {
#ifdef MTK_DEBUG
			qDebug() << query.lastError();
#endif
		}
		if (!query.exec(SQL_DECK_TABLE_CREATE)) {
#ifdef MTK_DEBUG
			qDebug() << query.lastError();
#endif
		}
		if (!query.exec(SQL_MATCH_TABLE_CREATE)) {
#ifdef MTK_DEBUG
			qDebug() << query.lastError();
#endif
		}
	}

	return EXE_OK;
}

execode
DBManager::get_settings(std::unique_ptr<settings>& dest)
{
	QSqlQuery query;

	if (!query.exec(SQL_SETTINGS_SELECT_DATA)) {
#ifdef MTK_DEBUG
		qDebug() << query.lastError();
#endif
		return EXE_ERROR;
	}

	bool exists = query.next();
	if (exists) {
		dest->hsexe_dir(qPrintable(query.value(0).toString()));
		dest->hsuser_dir(qPrintable(query.value(1).toString()));
		dest->local_dir(qPrintable(query.value(2).toString()));
		dest->jsoncards(qPrintable(query.value(3).toString()));
		dest->hsbuild(qPrintable(query.value(4).toString()));
		dest->img_dir(qPrintable(query.value(5).toString()));
		dest->hslog_dir(qPrintable(query.value(6).toString()));
		dest->lang_dir(qPrintable(query.value(7).toString()));
		dest->hsconfig(qPrintable(query.value(8).toString()));
		dest->asset_dir(qPrintable(query.value(9).toString()));
	} else {
		if (dest->hsexe_dir().empty() || dest->hsuser_dir().empty()) {
			return EXE_NOT_FOUND;
		}

		query.prepare(SQL_SETTINGS_INSERT_ALL);
		query.bindValue(0, 1);
		query.bindValue(1, dest->hsexe_dir().c_str());
		query.bindValue(2, dest->hsuser_dir().c_str());
		query.bindValue(3, dest->local_dir().c_str());
		query.bindValue(4, dest->jsoncards().c_str());
		query.bindValue(5, dest->hsbuild().c_str());
		query.bindValue(6, dest->img_dir().c_str());
		query.bindValue(7, dest->hslog_dir().c_str());
		query.bindValue(8, dest->lang_dir().c_str());
		query.bindValue(9, dest->hsconfig().c_str());
		query.bindValue(10, dest->asset_dir().c_str());

		if (!query.exec()) {
#ifdef MTK_DEBUG
			qDebug() << query.lastError();
#endif
			return EXE_ERROR;
		}
	}

	return EXE_OK;
}

execode
DBManager::set_settings_default(std::unique_ptr<settings>& dest)
{
	if (dest->hsbuild().empty()) {
		dest->hsbuild(MTK_HSBUILD);
	}
#ifdef Q_OS_WIN
	dest.local_dir(QDir::homePath().toStdString() +
		       "/AppData/Local/mastertracker");
	dest.hsexe_dir("C:/Program Files (x86)/Hearthstone");
	dest.hsuser_dir(
	    QDir::homePath().toStdString() +
	    "/Local Settings/Application Data/Blizzard/Hearthstone");
#endif
#ifdef Q_OS_MAC
	dest.local_dir(QDir::homePath().toStdString() +
		       "/Library/Application Support/mastertracker");
	dest.hsexe_dir("/Applications/Hearthstone");
	dest.hsuser_dir(QDir::homePath().toStdString() +
			"/Library/Preferences/Blizzard/Hearthstone");
#endif
#ifdef Q_OS_LINUX
	if (dest->local_dir().empty()) {
		dest->local_dir(QDir::homePath().toStdString() +
				"/.local/share/mastertracker");
	}

	if (dest->hsexe_dir().empty() || dest->hsuser_dir().empty()) {
		return EXE_NOT_IMPL;
	}

#endif
	if (dest->jsoncards().empty()) {
		dest->jsoncards(dest->local_dir() + "/cards.json");
	}
	if (dest->img_dir().empty()) {
		dest->img_dir(dest->local_dir() + "/img");
	}
	if (dest->asset_dir().empty()) {
		dest->asset_dir(dest->local_dir() + "/assets");
	}

	if (dest->hsconfig().empty()) {
		dest->hsconfig(dest->hsuser_dir() + "/log.config");
	}
	if (dest->lang_dir().empty()) {
		dest->lang_dir(dest->hsuser_dir() + "/Cache/UberText");
	}

	if (dest->hslog_dir().empty()) {
		dest->hslog_dir(dest->hsexe_dir() + "/Logs");
	}

	return EXE_OK;
}

execode
DBManager::update_hsbuild(std::unique_ptr<settings>& dest, const char* hsbuild)
{
	if (hsbuild == nullptr || dest == nullptr || dest->hsbuild().empty()) {
		return EXE_INVALID_ARGS;
	}

	if (dest->hsbuild() == hsbuild) {
		return EXE_FALSE;
	}

	dest->hsbuild(hsbuild);

	QSqlQuery query;

	query.prepare(SQL_SETTINGS_UPDATE_HSBUILD);
	query.bindValue(0, hsbuild);
	if (!query.exec()) {
#ifdef MTK_DEBUG
		qDebug() << query.lastError();
#endif
		return EXE_ERROR;
	}

	return EXE_OK;
}

execode
DBManager::add_deck(const std::shared_ptr<Deck>& pdeck)
{
	QSqlQuery query;

	query.prepare(SQL_DECK_SELECT_EXISTS);
	// TODO: bad cast
	query.bindValue(0, (int)pdeck->id());

	if (!query.exec()) {
#ifdef MTK_DEBUG
		qDebug() << query.lastError();
#endif
		return EXE_ERROR;
	}

	bool exists = query.next();
	if (!exists) {
		query.prepare(SQL_DECK_INSERT_ALL);
		// TODO: bad cast
		query.bindValue(0, (int)pdeck->id());
		query.bindValue(1, pdeck->name().c_str());
		query.bindValue(2, (unsigned char)pdeck->format());
		query.bindValue(3, (unsigned char)pdeck->hero_class());
		std::string buffer;
		write_deckstring(*pdeck, buffer);
		query.bindValue(4, buffer.c_str());
	}
	if (!query.exec()) {
#ifdef MTK_DEBUG
		qDebug() << query.lastError();
#endif
		return EXE_ERROR;
	}

	return EXE_OK;
}

execode
DBManager::add_match(game_result result,
		     const std::shared_ptr<Deck>& pdeck,
		     const std::string& oppo_name,
		     card_class oppo_class)
{
	QSqlQuery query;

	query.prepare(SQL_MATCH_INSERT_DATA);
	// TODO: bad cast
	query.bindValue(0, (int)pdeck->id());
	query.bindValue(1, (unsigned char)pdeck->format());
	query.bindValue(2, oppo_name.c_str());
	query.bindValue(3, (unsigned char)oppo_class);
	query.bindValue(4, (unsigned char)result);
	QDateTime current = QDateTime::currentDateTime();
	query.bindValue(5, current.toSecsSinceEpoch());

	if (!query.exec()) {
#ifdef MTK_DEBUG
		qDebug() << query.lastError();
#endif
		return EXE_ERROR;
	}

	return EXE_OK;
}

execode
DBManager::get_deck_stat(int64_t deck_id, int result[][3], int sz) const
{
	if (result == nullptr) {
		return EXE_INVALID_ARGS;
	}

	QSqlQuery query;

	query.prepare(SQL_STAT_SELECT_DATA);
	// TODO: bad cast
	query.bindValue(0, (int)deck_id);

	if (!query.exec()) {
#ifdef MTK_DEBUG
		qDebug() << query.lastError();
#endif
		return EXE_ERROR;
	}

	int i = 0;

	while (query.next()) {
		if (result[i] == nullptr) {
			return EXE_INVALID_ARGS;
		}
		if (i > sz) {
			return EXE_OVERFLOW;
		}
		result[i][0] = query.value(0).toInt();
		result[i][1] = query.value(1).toInt();
		result[i][2] = query.value(2).toInt();
		i++;
	}

	return EXE_OK;
}
}    // namespace mtk