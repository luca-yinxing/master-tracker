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

#ifndef HSJSON_JSONCARD_H
#define HSJSON_JSONCARD_H

#include <hsdata/card.hpp>
#include <hsdata/cardmap.hpp>
#include <hsdata/conv.hpp>

#include <stdio.h>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <map>
#include <memory>

namespace mtk
{
/**
 * \brief Data structure for Card database query
 *
 */
class CardQuery
{
      private:
	/**
	 * \brief value to store list of field to fetch
	 *
	 */
	uint64_t m_select = ~(0);
	/**
	 * \brief value to store list of field to compare
	 *
	 */
	uint64_t m_where = 0x0;
	/**
	 * \brief array of Card pointers as data to compare with where
	 *
	 */
	std::vector<std::shared_ptr<Card>> m_data;
	/**
	 * \brief using Hearthstone localization for strings
	 *
	 */
	language m_lang = EN_US;

      public:
	CardQuery() noexcept			 = default;
	CardQuery(const CardQuery& cpy) noexcept = default;
	~CardQuery() noexcept			 = default;

	void clear() noexcept { *this = CardQuery(); }

	uint64_t select() const noexcept { return m_select; }

	uint64_t where() const noexcept { return m_where; }

	const std::vector<std::shared_ptr<Card>>& data() const noexcept
	{
		return m_data;
	}

	language lang() const noexcept { return m_lang; }

	void select(uint64_t v) noexcept { m_select = v; }

	void where(uint64_t v) noexcept { m_where = v; }

	void data(const std::vector<std::shared_ptr<Card>>& v) noexcept
	{
		m_data = v;
	}

	void data(const std::vector<Card>& v) noexcept
	{
		m_data.clear();
		for (const Card& c : v) {
			m_data.push_back(std::make_shared<Card>(c));
		}
	}

	void lang(language v) noexcept { m_lang = v; }
};

/**
 * \brief Get the carddb value string object
 *
 * \param val json value object
 * \param str buffer string as return value
 * \param maxlen length of the string
 * \return EXE_INVALID_ARGS if str is nullptr
 * \return EXE_INVALID_TYPE if value is undefined or is not a string
 * \return EXE_OK if success
 */
inline execode
get_carddb_value_string(const QJsonValue& val, std::string& str)
{
	if (val.isUndefined() || !val.isString()) {
		return EXE_INVALID_TYPE;
	}

	str = val.toString().toStdString();

	return EXE_OK;
}

/**
 * \brief Get the carddb value string localized
 *
 * \param val json parent value object
 * \param lang localization enum
 * \param str buffer string as return value
 * \param maxlen length of the string
 * \return EXE_INVALID_ARGS if str is nullptr
 * \return EXE_INVALID_TYPE if value is undefined or is not a string or the
 * parent in not a object
 * \return EXE_ERROR if lang string is empty
 * \return EXE_OVERFLOW if cant copy all data in str
 * \return EXE_OK if success
 */
inline execode
get_carddb_value_language(const QJsonValue& val,
			  language lang,
			  std::string& str)
{
	if (val.isUndefined() || !val.isObject()) {
		return EXE_INVALID_TYPE;
	}

	const char* str_lang = conv_language_string(lang);

	if (strcmp(str_lang, STR_NULL) == 0) {
		return EXE_ERROR;
	}

	QJsonValue json_lang = val.toObject().value(str_lang);

	if (json_lang.isUndefined() || !json_lang.isString()) {
		return EXE_INVALID_TYPE;
	}

	str = json_lang.toString().toStdString();

	return EXE_OK;
}

/**
 * \brief Get the carddb value classes array object
 *
 * \param val json parent value object
 * \param pcard pointer of Card structure
 * \return EXE_INVALID_TYPE if value is undefined or in not an array
 * \return EXE_ERROR if child value is not a string
 * \return EXE_OK if success
 */
inline execode
get_carddb_value_classes_array(const QJsonValue& val,
			       std::shared_ptr<Card>& pcard)
{
	if (val.isUndefined() || !val.isArray()) {
		return EXE_INVALID_TYPE;
	}

	QJsonArray json_array = val.toArray();
	int tmp_count	      = json_array.count();
	std::vector<card_class> tmp_array;
	tmp_array.reserve(tmp_count);

	for (int i = 0; i < tmp_count; i++) {
		if (json_array[i].isString()) {
			tmp_array.push_back(conv_string_class(
			    qPrintable(json_array[i].toString())));
		} else {
			return EXE_ERROR;
		}
	}

	pcard->classes(tmp_array);

	return EXE_OK;
}

/**
 * \brief Get the carddb value classes string object
 *
 * \param val json value object
 * \param pcard pointer of Card structure
 * \return EXE_INVALID_TYPE if value is undefined or in not a string
 * \return EXE_OK if success
 */
inline execode
get_carddb_value_classes_string(const QJsonValue& val,
				std::shared_ptr<Card>& pcard)
{
	if (val.isUndefined() || !val.isString()) {
		return EXE_INVALID_TYPE;
	}

	pcard->classes(conv_string_class(qPrintable(val.toString())));

	return EXE_OK;
}

/**
 * \brief Get the carddb value mechanic object
 *
 * \param val json parent value object
 * \param pcard pointer of Card structure
 * \return EXE_INVALID_TYPE if value is undefined or in not an array
 * \return EXE_ERROR if child value is not a string
 * \return EXE_OK if success
 */
inline execode
get_carddb_value_mechanic(const QJsonValue& val, std::shared_ptr<Card>& pcard)
{
	if (val.isUndefined() || !val.isArray()) {
		return EXE_INVALID_TYPE;
	}

	QJsonArray json_array = val.toArray();
	int tmp_count	      = json_array.count();
	std::vector<card_mechanic> tmp_array;
	tmp_array.reserve(tmp_count);

	for (int i = 0; i < tmp_count; i++) {
		if (json_array[i].isString()) {
			tmp_array.push_back(conv_string_mechanic(
			    qPrintable(json_array[i].toString())));
		} else {
			return EXE_ERROR;
		}
	}

	pcard->mechanic(tmp_array);

	return EXE_OK;
}

/**
 * \brief Get the carddb value tag object
 *
 * \param val json parent value object
 * \param pcard pointer of Card structure
 * \return EXE_INVALID_TYPE if value is undefined or in not an array
 * \return EXE_ERROR if child value is not a string
 * \return EXE_OK if success
 */
inline execode
get_carddb_value_tag(const QJsonValue& val, std::shared_ptr<Card>& pcard)
{
	if (val.isUndefined() || !val.isArray()) {
		return EXE_INVALID_TYPE;
	}

	QJsonArray json_array = val.toArray();
	int tmp_count	      = json_array.count();

	std::vector<card_tag> tmp_array;
	tmp_array.reserve(tmp_count);

	for (int i = 0; i < tmp_count; i++) {
		if (json_array[i].isString()) {
			tmp_array.push_back(conv_string_tag(
			    qPrintable(json_array[i].toString())));
		} else {
			return EXE_ERROR;
		}
	}

	pcard->tag(tmp_array);

	return EXE_OK;
}

/**
 * \brief Get the carddb value object by Hearthstone database column name
 *
 * \param obj json row object
 * \param query structure of database query
 * \param name string with Hearthstone database column name
 * \param pcard pointer to Card structure
 * \return EXE_INVALID_ARGS if name is nullptr or pcard is nullptr
 * \return EXE_NOT_IMPL if column name is not managed and implemented
 * \return EXE_INVALID_TYPE if value is bad managed
 * \return EXE_OVERFLOW if cant fetch all data
 * \return EXE_ERROR if generic error occured
 * \return EXE_OK if success
 */
execode get_carddb_value(const QJsonObject& obj,
			 const CardQuery& query,
			 const QString& name,
			 std::shared_ptr<Card>& pcard);

/**
 * \brief Execute the where contest of the query, compare the request fields
 *
 * \param query structure of database query
 * \param pcard pointer to Card structure
 * \return EXE_NOT_FOUND if the Card dont match the query
 * \return EXE_OK if the Card match the query
 */
uint64_t get_carddb_where(const CardQuery& query,
			  const std::shared_ptr<Card>& pcard);

/**
 * \brief Execute the select contest of the query, remove not request fields
 *
 * \param query structure of database query
 * \param pcard pointer to Card structure
 * \return EXE_OK if success
 */
execode get_carddb_select(const CardQuery& query,
			  const std::shared_ptr<Card>& pcard);

/**
 * \brief Load json Card database array from file
 *
 * \param path file path of Card database
 * \param array json array object
 * \return EXE_CANT_OPEN if cannot open the file
 * \return EXE_INVALID_ARGS if path is nullptr
 * \return EXE_OK if success
 */
execode get_json_carddb_array(const std::string& path, QJsonArray& array);

/**
 * \brief Get the json carddb object
 *
 * \param path file path of Card database
 * \param query structure of database query
 * \param carddb map object where save Card records
 * \return EXE_INVALID_ARGS if path is nullptr
 * \return EXE_ERROR if query "where" is invalid
 * \return EXE_OK if success
 */
execode get_json_carddb(const std::string& path,
			const CardQuery& query,
			CardMap& carddb) noexcept;

}    // namespace mtk

#endif