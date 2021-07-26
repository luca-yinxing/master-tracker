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

// Copyright (C) 2021 Luca Gasperini <luca.gasperini@xsoftware.it>
//
// This file is part of master-tracker.
//
// master-tracker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// master-tracker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with master-tracker.  If not, see <http://www.gnu.org/licenses/>.

#include <hsdata/card.hpp>
#include <hsdata/conv.hpp>

#include <hsjson/jsoncard.hpp>
#include <hsjson/jsondl.hpp>

#include <QCommandLineParser>
#include <QGuiApplication>

#include <hsrender/context.hpp>
#include <hsrender/scenecard.hpp>
#include <hsrender/scenecardtile.hpp>

#ifdef MTK_DEBUG
#include <QDebug>
#endif

#include <chrono>
#include <iostream>

using namespace mtk;

#define CONTEXT_I18N "hsrender_app"

void
render_tile(const std::shared_ptr<Card>& pcard,
	    const std::string& assets_folder,
	    const std::string& output_file,
	    const std::string& texture_file,
	    int width)
{
	std::unique_ptr<SceneCardTile> tile_scene =
	    std::make_unique<SceneCardTile>();

	tile_scene->init();

	Context context;

	context.base_dir(assets_folder);
	uint32_t tile_id = context.add(std::move(tile_scene));

	context.render(tile_id);
	context.save(tile_id, output_file);
}

void
render_card(const std::shared_ptr<Card>& pcard,
	    const std::string& assets_folder,
	    const std::string& output_file,
	    const std::string& texture_file,
	    int width)
{
	std::unique_ptr<SceneCard> card_scene =
	    std::make_unique<SceneCard>(pcard, texture_file);

	card_scene->init();

	Context context;

	context.base_dir(assets_folder);
	uint32_t card_id = context.add(std::move(card_scene));

	context.render(card_id);
	context.scene(card_id)->scale(width, 0, false);
	context.save(card_id, output_file);
}

void
print_card_details(const std::shared_ptr<Card>& pcard)
{
	std::cout << "Card Details:\n";
	std::cout << "ID: " << pcard->id() << "\n";
	std::cout << "Code ID: " << pcard->code() << "\n";
	std::cout << "Name: " << pcard->name() << "\n";
	std::cout << "Cost: " << pcard->cost() << "\n";
	std::cout << "Rarity: " << conv_rarity_string(pcard->rarity()) << "\n";
	std::cout << "Type: " << conv_type_string(pcard->type()) << "\n";
	std::cout << "Classes: ";
	for (auto klass : pcard->classes()) {
		std::cout << conv_class_string(klass);
	}
	std::cout << "\n";
	std::cout << "Race: " << conv_race_string(pcard->race()) << "\n";
	std::cout << "Set: " << conv_set_string(pcard->set()) << "\n";
	std::cout << "Spellschool: "
		  << conv_spellschool_string(pcard->spellschool()) << "\n";
	std::cout << "Faction: " << conv_faction_string(pcard->faction())
		  << "\n";
	std::cout << "Collectible: "
		  << (pcard->collectible() ? "TRUE" : "FALSE") << "\n";
	std::cout << "Elite: " << (pcard->elite() ? "TRUE" : "FALSE") << "\n";
	std::cout << "Mechanics: ";
	for (auto mechanic : pcard->mechanic()) {
		std::cout << conv_mechanic_string(mechanic);
	}
	std::cout << "\n";
	std::cout << "Text: " << pcard->text() << "\n";
	std::cout << "Artist: " << pcard->artist() << "\n";
	std::cout << "Flavor: " << pcard->flavor() << "\n";
	std::cout << "Attack: " << pcard->attack() << "\n";
	std::cout << "Health: " << pcard->health() << "\n";
	std::cout << "Overload: " << pcard->overload() << "\n";
	std::cout << "Durability: " << pcard->durability() << "\n";
	std::cout << "Spelldamage: " << pcard->spelldamage() << "\n";
	std::cout << "Armor: " << pcard->armor() << "\n";
	std::cout << "Tags: ";
	for (auto tag : pcard->tag()) {
		std::cout << conv_tag_string(tag);
	}
	std::cout << "\n";
	std::cout << "Questreward: " << pcard->questreward() << "\n";

	std::cout << std::endl;
}

int
main(int argc, char** argv)
{
	char str_buffer[DEF_BUFFER_STR_LONG];

	std::string output_file;
	std::string assets_folder;
	std::string texture_file;
	int width		    = 0;
	std::shared_ptr<Card> pcard = std::make_shared<Card>();

	QGuiApplication app(argc, argv, false);

	QGuiApplication::setApplicationName("Hearthstone Render App");
	QGuiApplication::setApplicationVersion(VERSION_HSRENDER_APP);

	QCommandLineParser parser;
	parser.setApplicationDescription("Hearthstone Render App");
	parser.addHelpOption();
	parser.addVersionOption();

	QCommandLineOption opt_texture(
	    QStringList() << "T"
			  << "texture",
	    QGuiApplication::translate(CONTEXT_I18N, "Texture file."),
	    "texture",
	    "");
	parser.addOption(opt_texture);

	QCommandLineOption opt_output(
	    QStringList() << "O"
			  << "output",
	    QGuiApplication::translate(CONTEXT_I18N, "Output file."),
	    "output",
	    "output_card.png");
	parser.addOption(opt_output);

	QCommandLineOption opt_asset(
	    QStringList() << "A"
			  << "asset",
	    QGuiApplication::translate(CONTEXT_I18N, "Asset directory."),
	    "asset",
	    "");
	parser.addOption(opt_asset);

	QCommandLineOption opt_width(
	    QStringList() << "W"
			  << "width",
	    QGuiApplication::translate(CONTEXT_I18N, "Output file width."),
	    "width",
	    "512");
	parser.addOption(opt_width);

	QCommandLineOption opt_tile(
	    QStringList() << "I"
			  << "tile",
	    QGuiApplication::translate(CONTEXT_I18N, "Draw a tile."));
	parser.addOption(opt_tile);

	QCommandLineOption opt_json(
	    QStringList() << "J"
			  << "json",
	    QGuiApplication::translate(CONTEXT_I18N, "Json file."),
	    "json",
	    "");
	parser.addOption(opt_json);

	QCommandLineOption opt_id(
	    QStringList() << "i"
			  << "id",
	    QGuiApplication::translate(CONTEXT_I18N, "Card code id."),
	    "id",
	    "");
	parser.addOption(opt_id);

	strlists(str_buffer,
		 conv_array_language_string,
		 conv_array_language_len,
		 1);

	QCommandLineOption opt_lang(
	    QStringList() << "L"
			  << "lang",
	    QGuiApplication::translate(CONTEXT_I18N, "Card language. (") +
		str_buffer + ")",
	    "lang",
	    "enUS");
	parser.addOption(opt_lang);

	QCommandLineOption opt_name(
	    QStringList() << "n"
			  << "name",
	    QGuiApplication::translate(CONTEXT_I18N, "Card name."),
	    "name",
	    "Empty Name");
	parser.addOption(opt_name);

	QCommandLineOption opt_text(
	    QStringList() << "t"
			  << "text",
	    QGuiApplication::translate(CONTEXT_I18N, "Card text."),
	    "text",
	    "Empty Text");
	parser.addOption(opt_text);

	QCommandLineOption opt_attack(
	    QStringList() << "a"
			  << "attack",
	    QGuiApplication::translate(CONTEXT_I18N, "Card attack."),
	    "attack",
	    "0");
	parser.addOption(opt_attack);

	QCommandLineOption opt_health(
	    QStringList() << "H"
			  << "health",
	    QGuiApplication::translate(CONTEXT_I18N, "Card health."),
	    "health",
	    "0");
	parser.addOption(opt_health);

	QCommandLineOption opt_cost(
	    QStringList() << "c"
			  << "cost",
	    QGuiApplication::translate(CONTEXT_I18N, "Card cost."),
	    "cost",
	    "0");
	parser.addOption(opt_cost);

	QCommandLineOption opt_armor(
	    QStringList() << "m"
			  << "armor",
	    QGuiApplication::translate(CONTEXT_I18N, "Card armor."),
	    "armor",
	    "0");
	parser.addOption(opt_armor);

	QCommandLineOption opt_durability(
	    QStringList() << "d"
			  << "durability",
	    QGuiApplication::translate(CONTEXT_I18N, "Card durability."),
	    "durability",
	    "0");
	parser.addOption(opt_durability);

	strlists(str_buffer, conv_array_type_string, conv_array_type_len, 1);

	QCommandLineOption opt_type(
	    QStringList() << "y"
			  << "type",
	    QGuiApplication::translate(CONTEXT_I18N, "Card type. (") +
		str_buffer + ")",
	    "type",
	    "MINION");
	parser.addOption(opt_type);

	strlists(str_buffer, conv_array_set_string, conv_array_set_len, 1);

	QCommandLineOption opt_set(
	    QStringList() << "s"
			  << "set",
	    QGuiApplication::translate(CONTEXT_I18N, "Card set. (") +
		str_buffer + ")",
	    "set",
	    "CORE");
	parser.addOption(opt_set);

	strlists(str_buffer,
		 conv_array_rarity_string,
		 conv_array_rarity_len,
		 1);

	QCommandLineOption opt_rarity(
	    QStringList() << "r"
			  << "rarity",
	    QGuiApplication::translate(CONTEXT_I18N, "Card rarity. (") +
		str_buffer + ")",
	    "rarity",
	    "COMMON");
	parser.addOption(opt_rarity);

	strlists(str_buffer, conv_array_race_string, conv_array_race_len, 1);

	QCommandLineOption opt_race(
	    QStringList() << "R"
			  << "race",
	    QGuiApplication::translate(CONTEXT_I18N, "Card race. (") +
		str_buffer + ")",
	    "rarity",
	    "");
	parser.addOption(opt_race);

	strlists(str_buffer,
		 conv_array_spellschool_string,
		 conv_array_spellschool_len,
		 1);

	QCommandLineOption opt_spellschool(
	    QStringList() << "S"
			  << "spellschool",
	    QGuiApplication::translate(CONTEXT_I18N, "Card spellschool. (") +
		str_buffer + ")",
	    "rarity",
	    "");
	parser.addOption(opt_spellschool);

	strlists(str_buffer, conv_array_class_string, conv_array_class_len, 1);

	QCommandLineOption opt_class(
	    QStringList() << "C"
			  << "class",
	    QGuiApplication::translate(CONTEXT_I18N, "Card class. (") +
		str_buffer + ")",
	    "class",
	    "NEUTRAL");
	parser.addOption(opt_class);

	QCommandLineOption opt_elite(
	    QStringList() << "e"
			  << "elite",
	    QGuiApplication::translate(CONTEXT_I18N, "Card is elite. (0/1)"),
	    "elite",
	    "1");
	parser.addOption(opt_elite);

	QCommandLineOption opt_download(
	    QStringList() << "D"
			  << "download",
	    QGuiApplication::translate(CONTEXT_I18N, "Download art."),
	    "download-dir",
	    "");

	parser.addOption(opt_download);

	parser.process(app);

	if (parser.isSet(opt_json) && parser.isSet(opt_id)) {
		std::string file_json = qPrintable(parser.value(opt_json));
		std::string code_id   = qPrintable(parser.value(opt_id));

		if (file_json.empty() || code_id.empty()) {
			return 1;
		}

		CardMap carddb;
		CardQuery query;
		std::vector<Card> card_query;
		card_query.push_back(Card(code_id));
		language lang =
		    conv_string_language(qPrintable(parser.value(opt_lang)));
		query.lang(lang == INVALID_LANGUAGE ? EN_US : lang);
		query.data(card_query);
		query.where(CODE);

		if (get_json_carddb(file_json, query, carddb) != EXE_OK) {
			return 1;
		}

		std::shared_ptr<Card> tmp = carddb.find_by_code(code_id);

		if (tmp == nullptr) {
			return 1;
		}

		*pcard = *tmp;
	}
	if (parser.isSet(opt_name)) {
		pcard->name(qPrintable(parser.value(opt_name)));
	}
	if (parser.isSet(opt_text)) {
		pcard->text(qPrintable(parser.value(opt_text)));
	}
	if (parser.isSet(opt_attack)) {
		pcard->attack(parser.value(opt_attack).toInt());
	}
	if (parser.isSet(opt_health)) {
		pcard->health(parser.value(opt_health).toInt());
	}
	if (parser.isSet(opt_cost)) {
		pcard->cost(parser.value(opt_cost).toInt());
	}
	if (parser.isSet(opt_type)) {
		pcard->type(
		    conv_string_type(qPrintable(parser.value(opt_type))));
	}
	if (parser.isSet(opt_set)) {
		pcard->set(conv_string_set(qPrintable(parser.value(opt_set))));
	}
	if (parser.isSet(opt_rarity)) {
		pcard->rarity(
		    conv_string_rarity(qPrintable(parser.value(opt_rarity))));
	}
	if (parser.isSet(opt_race)) {
		pcard->race(
		    conv_string_race(qPrintable(parser.value(opt_race))));
	}
	if (parser.isSet(opt_spellschool)) {
		pcard->spellschool(conv_string_spellschool(
		    qPrintable(parser.value(opt_spellschool))));
	}
	if (parser.isSet(opt_elite)) {
		pcard->elite(parser.value(opt_elite) == "1");
	}
	if (parser.isSet(opt_armor)) {
		pcard->armor(parser.value(opt_armor).toInt());
	}
	if (parser.isSet(opt_durability)) {
		pcard->durability(parser.value(opt_durability).toInt());
	}
	if (parser.isSet(opt_class)) {
		pcard->classes(
		    conv_string_class(qPrintable(parser.value(opt_class))));
	}

	output_file = qPrintable(parser.value(opt_output));

	if (parser.isSet(opt_texture)) {
		texture_file = qPrintable(parser.value(opt_texture));
	} else if (parser.isSet(opt_download) && parser.isSet(opt_id)) {
		std::string code_id = qPrintable(parser.value(opt_id));
		std::string download_dir =
		    qPrintable(parser.value(opt_download));
		const char* STR_IMAGE_EXT_PNG = ".png";

		if (code_id.empty() || download_dir.empty()) {
			return 1;
		}

		texture_file =
		    download_dir + STR_OS_DIR_SEP + code_id + STR_IMAGE_EXT_PNG;
	} else {
		return 1;
	}
	assets_folder = qPrintable(parser.value(opt_asset));

	width = parser.value(opt_width).toInt();

	if (parser.isSet(opt_json) && parser.isSet(opt_download)) {
		std::string file_json = qPrintable(parser.value(opt_json));
		std::string download_dir =
		    qPrintable(parser.value(opt_download));

		CardMap carddb;
		CardQuery query;
		language lang =
		    conv_string_language(qPrintable(parser.value(opt_lang)));
		query.lang(lang == INVALID_LANGUAGE ? EN_US : lang);

		if (get_json_carddb(file_json, query, carddb) != EXE_OK) {
			return 1;
		}
		std::vector<std::string> codelist;
		carddb.to_codelist(codelist);

		if (os_mkdir(download_dir) != EXE_OK) {
			return 1;
		}

		void* curl = init_json_curl();
		if (download_json_art(curl, download_dir, codelist) !=
		    codelist.size()) {
			return 1;
		}

		free_json_curl(curl);
	}

	print_card_details(pcard);

	auto start = std::chrono::steady_clock::now();

	// FIXME: Assets folder wants ending /
	// TODO: Assets folder != "-D" dir?

	if (parser.isSet(opt_tile)) {
		render_tile(pcard,
			    assets_folder,
			    output_file,
			    texture_file,
			    width);
	} else {
		render_card(pcard,
			    assets_folder,
			    output_file,
			    texture_file,
			    width);
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Rendering time: " << elapsed_seconds.count() << "s\n";

	return 0;
}