#include <hsdata/deckstring.hpp>
#include <hsjson/jsoncard.hpp>

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFile>

#include <iostream>

#define CONTEXT_I18N "deckstring_app"

using namespace mtk;

enum output_type
{
	HSPAGE = 0,
	CSV    = 1,
};

int
main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv, false);

	QCoreApplication::setApplicationName("Hearthstone DeckString App");
	QCoreApplication::setApplicationVersion(VERSION_DECKSTRING_APP);

	QCommandLineParser parser;
	parser.setApplicationDescription("Hearthstone DeckString App");
	parser.addHelpOption();
	parser.addVersionOption();

	QCommandLineOption opt_json(
	    QStringList() << "j"
			  << "json",
	    QCoreApplication::translate(CONTEXT_I18N, "Json file."),
	    "json",
	    "");
	parser.addOption(opt_json);

	QCommandLineOption opt_deckstring(
	    QStringList() << "d"
			  << "deckstring",
	    QCoreApplication::translate(CONTEXT_I18N,
					"Hearthstone DeckString."),
	    "deckstring",
	    "");
	parser.addOption(opt_deckstring);

	QCommandLineOption opt_format(
	    QStringList() << "f"
			  << "format",
	    QCoreApplication::translate(CONTEXT_I18N, "Format type."),
	    "format",
	    "0");
	parser.addOption(opt_format);

	QCommandLineOption opt_output(
	    QStringList() << "o"
			  << "output",
	    QCoreApplication::translate(CONTEXT_I18N, "Output file."),
	    "output",
	    "");
	parser.addOption(opt_output);

	parser.process(app);

	std::string inputcode;
	std::string jsonfile;
	std::string outputfile;
	output_type ot = HSPAGE;
	Deck deck;
	CardMap carddb;
	CardQuery query;

	if (parser.isSet(opt_deckstring)) {
		inputcode = qPrintable(parser.value(opt_deckstring));
	} else {
		return 1;
	}

	if (parser.isSet(opt_json)) {
		jsonfile = qPrintable(parser.value(opt_json));
	}

	if (parser.isSet(opt_format)) {
		ot = (output_type)parser.value(opt_format).toInt();
	}

	if (parser.isSet(opt_output)) {
		outputfile = qPrintable(parser.value(opt_output));
	}

	query.lang(IT_IT);

	if (read_deckstring(inputcode.c_str(), deck) != EXE_OK) {
		return 1;
	}

	if (get_json_carddb(jsonfile, query, carddb) != EXE_OK) {
		return 1;
	}

	deck.complete_info(carddb);

	deck.sort_cost();

	std::string tmp_output;

	switch (ot) {
	case HSPAGE:
		if (write_deckstring_page(deck, tmp_output) != EXE_OK) {
			return 1;
		}
		break;
	case CSV:
		if (write_deckstring_csv(deck, tmp_output) != EXE_OK) {
			return 1;
		}
		break;
	default:
		return 1;
	}

	if (outputfile.empty()) {
		std::cout << tmp_output << std::endl;
	} else {
		QFile file(outputfile.c_str());
		file.open(QIODevice::WriteOnly);
		file.write(tmp_output.c_str(), tmp_output.length());
	}

	return 0;
}