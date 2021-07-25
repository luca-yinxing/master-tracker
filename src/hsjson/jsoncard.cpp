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

#include "jsoncard.hpp"
namespace mtk
{
execode
get_carddb_value(const QJsonObject& obj,
		 const CardQuery& query,
		 const QString& name,
		 std::shared_ptr<Card>& pcard)
{
	if (pcard == nullptr) {
		return EXE_INVALID_ARGS;
	}

	std::string buffer;
	execode retval = EXE_OK;

	if (name == "dbfId") {
		pcard->id(obj.value(name).toInt());
	} else if (name == "id") {
		retval = get_carddb_value_string(obj.value(name), buffer);
		if (retval != EXE_OK) {
			return retval;
		}
		pcard->code(buffer);
	} else if (name == "cost") {
		pcard->cost(obj.value(name).toInt());
	} else if (name == "name") {
		retval = get_carddb_value_language(obj.value(name),
						   query.lang(),
						   buffer);
		if (retval != EXE_OK) {
			return retval;
		}
		pcard->name(buffer);
	} else if (name == "classes") {
		retval = get_carddb_value_classes_array(obj.value(name), pcard);
	} else if (name == "cardClass") {
		retval =
		    get_carddb_value_classes_string(obj.value(name), pcard);
	} else if (name == "mechanics") {
		retval = get_carddb_value_mechanic(obj.value(name), pcard);
	} else if (name == "rarity") {
		retval = get_carddb_value_string(obj.value(name), buffer);
		pcard->rarity(conv_string_rarity(buffer));
	} else if (name == "type") {
		retval = get_carddb_value_string(obj.value(name), buffer);
		pcard->type(conv_string_type(buffer));
	} else if (name == "set") {
		retval = get_carddb_value_string(obj.value(name), buffer);
		pcard->set(conv_string_set(buffer));
	} else if (name == "race") {
		retval = get_carddb_value_string(obj.value(name), buffer);
		pcard->race(conv_string_race(buffer));
	} else if (name == "text") {
		retval = get_carddb_value_language(obj.value(name),
						   query.lang(),
						   buffer);
		if (retval != EXE_OK) {
			return retval;
		}
		pcard->text(buffer);
	} else if (name == "flavor") {
		retval = get_carddb_value_language(obj.value(name),
						   query.lang(),
						   buffer);
		if (retval != EXE_OK) {
			return retval;
		}
		pcard->text(buffer);
	} else if (name == "artist") {
		retval = get_carddb_value_string(obj.value(name), buffer);
		if (retval != EXE_OK) {
			return retval;
		}
		pcard->text(buffer);
	} else if (name == "spellSchool") {
		retval = get_carddb_value_string(obj.value(name), buffer);
		pcard->spellschool(conv_string_spellschool(buffer));
	} else if (name == "faction") {
		retval = get_carddb_value_string(obj.value(name), buffer);
		pcard->faction(conv_string_faction(buffer));
	} else if (name == "collectible") {
		pcard->collectible(obj.value(name).toBool());
	} else if (name == "elite") {
		pcard->elite(obj.value(name).toBool());
	} else if (name == "health") {
		pcard->health(obj.value(name).toInt());
	} else if (name == "attack") {
		pcard->attack(obj.value(name).toInt());
	} else if (name == "overload") {
		pcard->overload(obj.value(name).toInt());
	} else if (name == "durability") {
		pcard->durability(obj.value(name).toInt());
	} else if (name == "spellDamage") {
		pcard->spelldamage(obj.value(name).toInt());
	} else if (name == "armor") {
		pcard->armor(obj.value(name).toInt());
	} else if (name == "referencedTags") {
		return get_carddb_value_tag(obj.value(name), pcard);
	} else if (name == "questReward") {
		retval = get_carddb_value_string(obj.value(name), buffer);
		if (retval != EXE_OK) {
			return retval;
		}
		pcard->questreward(buffer);
	} /* else if (strcmp(name, "battlegroundsPremiumDbfId") == 0) {
	    return EXE_NOT_IMPL;
	} else if (strcmp(name, "battlegroundsNormalDbfId") == 0) {
	    return EXE_NOT_IMPL;
	} else if (strcmp(name, "battlegroundsHero") == 0) {
	    return EXE_NOT_IMPL;
	} else if (strcmp(name, "targetingArrowText") == 0) {
	    return EXE_NOT_IMPL;
	} else if (strcmp(name, "collectionText") == 0) {
	    return EXE_NOT_IMPL;
	} else if (strcmp(name, "howToEarn") == 0) {
	    return EXE_NOT_IMPL;
	} else if (strcmp(name, "howToEarnGolden") == 0) {
	    return EXE_NOT_IMPL;
	} else if (strcmp(name, "techLevel") == 0) {
	    return EXE_NOT_IMPL;
	} else if (strcmp(name, "hideCost") == 0) {
	    return EXE_NOT_IMPL;
	} else if (strcmp(name, "hideStats") == 0) {
	    return EXE_NOT_IMPL;
	} else if (strcmp(name, "puzzleType") == 0) {
	    return EXE_NOT_IMPL;
	} else if (strcmp(name, "multiClassGroup") == 0) {
	    return EXE_NOT_IMPL;
	} */
	else {
		return EXE_NOT_IMPL;
	}

	return retval;
}

uint64_t
get_carddb_where(const CardQuery& query, const std::shared_ptr<Card>& pcard)
{
	if (query.where() == 0 || query.data().size() < 1) {
		return 0;
	}

	uint64_t search_multi = query.where();
	uint64_t search_first = query.where();
	search_first &= ~(CLASSES);
	search_first &= ~(MECHANIC);
	search_first &= ~(TAG);
	search_first &= ~(COLLECTIBLE);
	search_first &= ~(ELITE);
	uint64_t fields = query.where();

	uint64_t cmp = 0;

	for (size_t i = 0; i < query.data().size(); i++) {
		if (i == 0) {
			cmp = pcard->compare(*(query.data()[i]),
					     search_first,
					     false);
		} else {
			cmp = pcard->compare(*(query.data()[i]),
					     search_multi,
					     false);
		}

		fields &= ~cmp;

		if (fields == 0) {
			return 0;
		}
	}

	return fields;
}

execode
get_carddb_select(const CardQuery& query, const std::shared_ptr<Card>& pcard)
{
	if (!(query.select() & CODE)) {
		pcard->clear_code();
	}
	if (!(query.select() & NAME)) {
		pcard->clear_name();
	}
	if (!(query.select() & CLASSES)) {
		pcard->clear_classes();
	}
	if (!(query.select() & MECHANIC)) {
		pcard->clear_mechanic();
	}
	if (!(query.select() & TEXT)) {
		pcard->clear_text();
	}
	if (!(query.select() & ARTIST)) {
		pcard->clear_artist();
	}
	if (!(query.select() & FLAVOR)) {
		pcard->clear_flavor();
	}
	if (!(query.select() & TAG)) {
		pcard->clear_tag();
	}
	if (!(query.select() & QUESTREWARD)) {
		pcard->clear_questreward();
	}

	return EXE_OK;
}

execode
get_json_carddb_array(const std::string& path, QJsonArray& array)
{
	QFile json_file(path.c_str());

	if (!json_file.open(QIODevice::ReadOnly)) {
		return EXE_CANT_OPEN;
	}

	// la lettura incide circa 0.03/0.04 secondi nei benchmark (con cache)
	// ed è circa il 15% del test_hsjson_query_all

	array = QJsonDocument::fromJson(json_file.readAll()).array();
	// il parsing incide circa 0.13/0.16 secondi nei benchmark (con cache)
	// ed è circa il 57% del test_hsjson_query_all

	// la funzione incide circa 0.16/0.18 secondi nei benchmark (con cache)
	// ed è circa il 65% del test_hsjson_query_all

	json_file.close();

	return EXE_OK;
}

execode
get_json_carddb(const std::string& path,
		const CardQuery& query,
		CardMap& carddb) noexcept
{
	QByteArray data;
	QJsonArray json_array;

	QJsonObject tmp_obj;
	std::shared_ptr<Card> pcard;

	if (get_json_carddb_array(path, json_array) != EXE_OK) {
		return EXE_CANT_OPEN;
	}

	// la ricerca incide circa 0.06/0.08 secondi nei benchmark (con cache)
	// ed è circa il 28% del test_hsjson_query_all

	if (query.data().empty() && query.where() != 0x0) {
		return EXE_ERROR;
	}

	for (const QJsonValue& v : json_array) {
		bool not_found = false;
		tmp_obj	       = v.toObject();
		pcard	       = std::make_shared<Card>();

		for (auto field = tmp_obj.constBegin();
		     field != tmp_obj.constEnd();
		     field++) {
			execode code = get_carddb_value(tmp_obj,
							query,
							field.key(),
							pcard);
			if (code == EXE_NOT_IMPL) {
				continue;
			}
			if (code != EXE_OK) {
				not_found = true;
				break;
			}
		}

		uint64_t fields = get_carddb_where(query, pcard);

		if (!not_found && fields == 0) {
			get_carddb_select(query, pcard);
			carddb.add_card(pcard->id(), pcard);
		} else {
			// pcard->clear();
			pcard.reset();
		}
	}

	return EXE_OK;
}

}    // namespace mtk