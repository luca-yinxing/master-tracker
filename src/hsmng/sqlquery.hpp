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

#ifndef MTK_HSMNG_SQLQUERY_H
#define MTK_HSMNG_SQLQUERY_H

namespace mtk
{
constexpr const char* SQL_SETTINGS_TABLE_CREATE = "CREATE TABLE settings "
						  "(id integer primary key, "
						  "hsexe_dir varchar(256),"
						  "hsuser_dir varchar(256),"
						  "local_dir varchar(256), "
						  "jsoncards varchar(256),"
						  "hsbuild varchar(16),"
						  "img_dir varchar(256),"
						  "hslog_dir varchar(256),"
						  "lang_dir varchar(256),"
						  "hsconfig varchar(256),"
						  "asset_dir varchar(256)"
						  ")";

constexpr const char* SQL_DECK_TABLE_CREATE = "CREATE TABLE deck "
					      "(id integer primary key, "
					      "name varchar(256),"
					      "format integer(1),"
					      "class integer(1),"
					      "code varchar(256)"
					      ")";

constexpr const char* SQL_MATCH_TABLE_CREATE = "CREATE TABLE match "
					       "(id integer primary key, "
					       "id_deck integer,"
					       "format char(1),"
					       "oppo_name varchar(256),"
					       "oppo_class integer(1),"
					       "result integer(1),"
					       "datetime integer"
					       ")";

constexpr const char* SQL_SETTINGS_SELECT_DATA =
    "SELECT "
    "hsexe_dir, hsuser_dir, local_dir, jsoncards, hsbuild, img_dir, hslog_dir, lang_dir, hsconfig, asset_dir"
    " FROM settings WHERE id=1";

constexpr const char* SQL_SETTINGS_INSERT_ALL =
    "INSERT INTO settings"
    "(id, hsexe_dir, hsuser_dir, local_dir, jsoncards, hsbuild, img_dir, hslog_dir, lang_dir, hsconfig, asset_dir)"
    "VALUES (?,?,?,?,?,?,?,?,?,?,?)";

constexpr const char* SQL_SETTINGS_UPDATE_HSBUILD =
    "UPDATE settings SET hsbuild = ? WHERE id = 1";

constexpr const char* SQL_DECK_SELECT_EXISTS = "SELECT 1 from deck WHERE id=?";

constexpr const char* SQL_DECK_INSERT_ALL = "INSERT INTO deck"
					    "(id, name, format, class, code)"
					    "VALUES (?,?,?,?,?)";

constexpr const char* SQL_MATCH_INSERT_DATA =
    "INSERT INTO match"
    "(id_deck, format, oppo_name, oppo_class, result, datetime)"
    "VALUES (?,?,?,?,?,?)";

constexpr const char* SQL_STAT_SELECT_DATA =
    "SELECT oppo_class, "
    "SUM(CASE WHEN result=2 THEN 1 ELSE 0 END) AS 'win', "
    "SUM(CASE WHEN result=1 THEN 1 ELSE 0 END) AS 'lost' "
    "FROM match WHERE id_deck = ? GROUP BY oppo_class";
}    // namespace mtk

#endif