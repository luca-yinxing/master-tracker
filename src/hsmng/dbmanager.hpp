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

#ifndef MT_HSMNG_DBMANAGER_H
#define MT_HSMNG_DBMANAGER_H

#include <QSqlDatabase>

#include <hsdata/deck.hpp>
#include <utils/utils.hpp>

#include "settings.hpp"

//! \file

namespace mtk
{
constexpr const char* DATABASE_FILE	    = "mtk.sqlite";
constexpr const size_t DATABASE_STRING_SIZE = 256;

class DBManager
{
      private:
	QSqlDatabase db;

      public:
	DBManager();
	~DBManager();

	execode init(const std::string& path);
	execode get_settings(std::unique_ptr<settings>& dest);
	execode set_settings_default(std::unique_ptr<settings>& dest);
	execode update_hsbuild(std::unique_ptr<settings>& dest,
			       const char* hsbuild);
	execode add_deck(const std::shared_ptr<Deck>& pdeck);
	execode add_match(game_result result,
			  const std::shared_ptr<Deck>& pdeck,
			  const std::string& oppo_name,
			  card_class oppo_class);
	execode get_deck_stat(int64_t deck_id, int result[][3], int sz) const;
};
}    // namespace mtk

#endif