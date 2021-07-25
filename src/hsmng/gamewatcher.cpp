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

#include "gamewatcher.hpp"

#include <hsdata/conv.hpp>

#include <QTimer>

#include <QDateTime>
#include <QRegularExpressionMatch>

#include "constant.hpp"

#ifdef MTK_DEBUG
#include <QDebug>
#endif

using namespace mtk;

namespace mtk
{
GameWatcher::GameWatcher(QObject* parent) : QObject(parent)
{
	reset();
	match = new QRegularExpressionMatch();
}

GameWatcher::~GameWatcher()
{
	delete match;
}

void
GameWatcher::reset()
{
	powerState	  = noGame;
	arenaState	  = noDeckRead;
	scr_state	  = SS_MENU;
	mulliganEnemyDone = mulliganPlayerDone = false;
	turn = turnReal = 0;
	spectating	= false;
	logSeekCreate	= -1;
	logSeekWon	= -1;
	tied		= true;
}

void
GameWatcher::process_logline(log_component component,
			     const QString& line,
			     qint64 numline,
			     qint64 seek)
{
	switch (component) {
	case POWER:
		process_power(line, numline, seek);
		break;
	case ZONE:
		// processZone(line, numline);
		break;
	case LOADINGSCREEN:
		processLoadingScreen(line, numline);
		break;
	case ARENA:
		// processArena(line, numline);
		break;
	case ASSET:
		// processAsset(line, numline);
		break;
	default:
		// emit pDebug("Unknown log component read.", Warning);
		break;
	}
}

void
GameWatcher::startReadingDeck()
{
	if (arenaState == deckRead)
		return;
	emit needResetDeck();	 // resetDeck
	arenaState = readingDeck;
	// emit pDebug("Start reading deck (arenaState = readingDeck).", 0);
}

void
GameWatcher::endReadingDeck()
{
	if (arenaState != readingDeck)
		return;
	arenaState = deckRead;
	emit arenaDeckRead();	 // completeArenaDeck with draft file
	// emit pDebug("End reading deck (arenaState = deckRead).", 0);
	emit pLog(tr("Log: Active deck read."));
}

void
GameWatcher::setDeckRead(bool value)
{
	if (value) {
		arenaState = deckRead;
		// emit pDebug("SetDeckRead (arenaState = deckRead).", 0);
	} else {
		arenaState = noDeckRead;
		// emit pDebug("SetDeckRead (arenaState = noDeckRead).", 0);
	}
}

void
GameWatcher::processLoadingScreen(const QString& line, qint64 numline)
{
	//[LoadingScreen] LoadingScreen.OnSceneLoaded() - prevMode=HUB
	// currMode=DRAFT
	if (line.contains(
		QRegularExpression(
		    "LoadingScreen\\.OnSceneLoaded\\(\\) *- *prevMode=(\\w+) *currMode=(\\w+)"),
		match)) {
		screen_state prev =
		    conv_string_screen_state(qPrintable(match->captured(1)));
		screen_state curr =
		    conv_string_screen_state(qPrintable(match->captured(2)));

		emit screen_mode_changed(prev, curr);

		// Create result, avoid first run
		if (prev == SS_GAMEPLAY && logSeekCreate != -1 &&
		    logSeekWon != -1) {
			if (spectating || scr_state == SS_MENU || tied) {
				// emit pDebug("CreateGameResult: Avoid
				// spectator/tied game result.", 0);
			} else {
				QString logFileName = createGameLog();
				createGameResult(logFileName);
			}
			spectating = false;
		}

		if (curr == SS_ARENA) {
			scr_state = SS_ARENA;
			// emit pDebug("Entering ARENA (scr_state = arena).",
			// numLine);

			if (prev == SS_GAMEMODE || prev == SS_FRIENDLY) {
				emit enterArena();    // enterArena deckHandler
			}
		} else if (curr == SS_MENU) {
			scr_state = SS_MENU;
			// emit pDebug("Entering MENU (scr_state = menu).",
			// numLine);
		} else if (curr == SS_GAMEMODE) {
			scr_state = SS_GAMEMODE;
			// emit pDebug("Entering GAME_MODE (scr_state =
			// gameMode).", numLine);

			if (prev == SS_ARENA) {
				setDeckRead(false);
				emit leaveArena();    // leaveArena deckHandler
			}
		} else if (curr == SS_RANKED) {
			scr_state = SS_RANKED;
			// emit pDebug("Entering CASUAL/RANKED (scr_state =
			// ranked).", numLine);
		} else if (curr == SS_ADVENTURE) {
			scr_state = SS_ADVENTURE;
			// emit pDebug("Entering ADVENTURE (scr_state =
			// adventure).", numLine);
		} else if (curr == SS_BRAWL) {
			scr_state = SS_BRAWL;
			// emit pDebug("Entering TAVERN (scr_state =
			// tavernBrawl).", numLine);
		} else if (curr == SS_FRIENDLY) {
			scr_state = SS_FRIENDLY;
			// emit pDebug("Entering FRIENDLY (scr_state =
			// friendly).", numLine);

			if (prev == SS_ARENA) {
				setDeckRead(false);
				emit leaveArena();    // leaveArena deckHandler
			}
		}
	}
}

void
GameWatcher::processAsset(const QString& line, qint64 numline)
{
	Q_UNUSED(line);
	Q_UNUSED(numline);

	//    if(powerState != noGame)   return;

	//    //Definimos RANKED solo si venimos de loadScreen TOURNAMENT y
	//    //acabamos de encontrar el WON pero aun no hemos createResult
	//    if(scr_state == casual && logSeekWon != -1 &&
	//    line.contains("assetPath=rank_window"))
	//    {
	//        scr_state = ranked;
	//        // emit pDebug("On RANKED (scr_state = ranked).", numLine);
	//    }
}

// Usar ticket arena/Comprar arena
// D 11:27:13.1732560 DraftManager.OnBegin - Got new draft deck with ID:
// 507495951 D 11:27:13.1736650 SetDraftMode - DRAFTING Volver arena eleccion
// heroes ya comprada D 11:27:47.0196860 DraftManager.OnChoicesAndContents -
// Draft Deck ID: 507495951, Hero Card = D 11:27:47.0197460 SetDraftMode -
// DRAFTING
void
GameWatcher::processArena(const QString& line, qint64 numline)
{
	// NEW ARENA - START DRAFT
	//[Arena] DraftManager.OnChosen(): hero=HERO_02 premium=STANDARD
	if (line.contains(
		QRegularExpression(
		    "DraftManager\\.OnChosen\\(\\): hero=HERO_(\\d+)"),
		match)) {
		QString hero = match->captured(1);
		// emit pDebug("New arena. Heroe: " + hero, numLine);
		emit pLog(tr("Log: New arena."));
		emit newArena(hero);	//(connect)Begin draft
					////(connect)resetDeckDontRead
					//(arenaState = deckRead)
	}
	// DRAFTING PICK CARD
	//[Arena] Client chooses: Profesora violeta (NEW1_026)
	else if (line.contains(
		     QRegularExpression("Client chooses: .* \\((\\w+)\\)"),
		     match)) {
		QString code = match->captured(1);
		if (!code.contains("HERO")) {
			// emit pDebug("Pick card: " + code, numLine);
			emit pickCard(code);
		}
	}
	// START READING DECK
	//[Arena] DraftManager.OnChoicesAndContents - Draft Deck ID: 472720132,
	// Hero Card = HERO_02
	else if (
	    line.contains(
		QRegularExpression(
		    "DraftManager\\.OnChoicesAndContents - Draft Deck ID: \\d+, Hero Card = HERO_(\\d+)"),
		match)) {
		QString hero = match->captured(1);
		// emit pDebug("Found Hero Draft Deck. Heroe: " + hero,
		// numLine);
		startReadingDeck();
		emit heroDraftDeck(hero);
	}
	// END READING DECK
	//[Arena] SetDraftMode - ACTIVE_DRAFT_DECK
	else if (line.contains("SetDraftMode - ACTIVE_DRAFT_DECK")) {
		// emit pDebug("Found ACTIVE_DRAFT_DECK.", numLine);
		endReadingDeck();    // completeArenaDeck with draft file
		emit
		activeDraftDeck();    //(connect)End draft/Show mechanics,
				      // debe estar detras de endReadingDeck
		// para primero completar el deck y luego mostrar la mechanics
		// window del deck completo
		emit arenaDeckRead();	 // completeArenaDeck with draft file,
					 // debemos repetirlo para el caso de
					 // end draft
		// para primero completar el fichero del draft deck (se hace en
		// endDraft) y luego rellenar el deck con lo que le falte por si
		// hemos salido de la arena y vuelto a entrar durante el draft.
	}
	// READ DECK CARD
	//[Arena] DraftManager.OnChoicesAndContents - Draft deck contains card
	// FP1_012
	else if (
	    (arenaState == readingDeck) &&
	    line.contains(
		QRegularExpression(
		    "DraftManager\\.OnChoicesAndContents - Draft deck contains card (\\w+)"),
		match)) {
		QString code = match->captured(1);
		// emit pDebug("Reading deck: " + code, numLine);
		emit newDeckCard(code);
	}
	// COMPRAR ARENA -- VUELTA A SELECCION HEROE
	else if (
	    line.contains(
		QRegularExpression(
		    "DraftManager\\.OnBegin - Got new draft deck with ID: \\d+"),
		match) ||
	    line.contains(
		QRegularExpression(
		    "DraftManager\\.OnChoicesAndContents - Draft Deck ID: \\d+, Hero Card ="),
		match)) {
		// emit pDebug("New arena: choosing heroe.", numLine);
		emit heroDraftDeck();	 // No hero
		emit arenaChoosingHeroe();
	}
	// SetDraftMode - DRAFTING
	else if (line.contains("SetDraftMode - DRAFTING")) {
		// emit pDebug("Found SetDraftMode - DRAFTING.", numLine);
		emit continueDraft();	 //(connect) continueDraft
	}
	// SetDraftMode - IN_REWARDS
	else if (line.contains("SetDraftMode - IN_REWARDS")) {
		// emit pDebug("Found SetDraftMode - IN_REWARDS.", numLine);
		emit heroDraftDeck();	 // No hero
		emit inRewards();	 // Remove mechanics window
	}
}

// Ejemplo spectate
// 11:22:01 - GameWatcher(1): Start Spectator Game.
// 11:22:01 - GameWatcher(3): Found CREATE_GAME (powerState = heroType1State)
// 11:22:04 - GameWatcher(22): LoadingScreen: HUB -> GAMEPLAY
// Juego arena
// 20:58:25 - GameWatcher(27229): Found WON (powerState = noGame): Dappo
// 20:58:35 - GameWatcher(27255): End Spectator Game.
// 20:58:36 - GameWatcher(346): LoadingScreen: GAMEPLAY -> HUB
// 20:58:36 - GameWatcher: CreateGameResult: Avoid spectator/tied game result.
// 20:58:36 - GameWatcher(346): Entering MENU (scr_state = menu).
// Nuevo juego arena
// 20:58:54 - GameWatcher(27256): Start Spectator Game.
// 20:58:56 - GameWatcher(27258): Found CREATE_GAME (powerState =
// heroType1State) 20:58:56 - GameWatcher(376): LoadingScreen: HUB -> GAMEPLAY

void
GameWatcher::process_power(const QString& line, qint64 numline, qint64 seek)
{
	//#ifdef MTK_DEBUG
	//    qDebug() << line;
	//#endif
	//================== End Spectator Game ==================
	if (line.contains("End Spectator Game")) {
		// emit pDebug("End Spectator Game.", numLine);
		//        spectating = false;//Se pondra a false despues de
		//        haberse creado el resultado en LoadingScreen: GAMEPLAY
		//        -> HUB

		if (powerState != noGame) {
			// emit pDebug("WON not found (PowerState = noGame)",
			// 0);
			powerState = noGame;
			emit end_game();
		}
	}

	//================== Begin Spectating 1st player ==================
	//================== Start Spectator Game ==================
	else if (line.contains("Begin Spectating") ||
		 line.contains("Start Spectator Game")) {
		// emit pDebug("Start Spectator Game.", numLine);
		spectating = true;
	}
	// Create game
	// GameState.DebugPrintPower() - CREATE_GAME
	else if (line.contains("GameState.DebugPrintPower() - CREATE_GAME")) {
		if (powerState != noGame) {
			return;
			// emit pDebug("WON not found (PowerState = noGame)",
			// 0);
			// powerState = noGame;
			// emit end_game();
		}

		// emit pDebug("\nFound CREATE_GAME (powerState =
		// heroType1State)", numLine);
		logSeekCreate = seek;
		powerState    = heroType1State;

		mulliganEnemyDone = mulliganPlayerDone = false;
		turn = turnReal = 0;

		hero1.clear();
		hero2.clear();
		name1.clear();
		name2.clear();
		firstPlayer.clear();
		winnerPlayer.clear();
		playerID = 0;
		playerTag.clear();
		secretHero		   = INVALID_CLASS;
		playerMinions		   = 0;
		enemyMinions		   = 0;
		enemyMinionsAliveForAvenge = -1;
		enemyMinionsDeadThisTurn   = 0;
		playerCardsPlayedThisTurn  = 0;
		startGameEpoch = QDateTime::currentMSecsSinceEpoch() / 1000;
		tied = true;	// Si no se encuentra WON no se llamara a
				// createGameResult() pq tried sigue siendo true

		emit specialCardTrigger(
		    "",
		    "",
		    -1,
		    -1);    // Evita Cartas createdBy en el mulligan de practica
		emit start_game();
	}

	if (powerState != noGame) {
		// Win state
		// PowerTaskList.DebugPrintPower() -     TAG_CHANGE Entity=El
		// tabernero tag=PLAYSTATE value=WON
		if (line.contains(
			QRegularExpression(
			    "PowerTaskList\\.DebugPrintPower\\(\\) - *TAG_CHANGE "
			    "Entity=(.+) tag=PLAYSTATE value=(WON|TIED)"),
			match)) {
			// winnerPlayer = match->captured(1);
			// tied = (match->captured(2) == "TIED");
			QString winner_name = match->captured(1);
			tied		    = (match->captured(2) == "TIED");

			powerState = noGame;
			logSeekWon = seek;

			emit end_game(winner_name, tied);
		}
		// Turn
		// PowerTaskList.DebugPrintPower() -     TAG_CHANGE
		// Entity=GameEntity tag=TURN value=12
		else if (
		    line.contains(
			QRegularExpression(
			    "PowerTaskList\\.DebugPrintPower\\(\\) - *TAG_CHANGE "
			    "Entity=GameEntity tag=TURN value=(\\d+)"),
			match)) {
			turn = match->captured(1).toInt();
			emit logTurn();
			// emit pDebug("Found TURN: " + match->captured(1),
			// numLine);

			if (powerState != inGameState && turn > 1) {
				powerState	  = inGameState;
				mulliganEnemyDone = mulliganPlayerDone = true;
				emit clearDrawList();
				// emit pDebug("WARNING: Heroes/Players info
				// missing (powerState = inGameState,
				// mulliganDone = true)", 0, Warning);
			}
		}
	}
	switch (powerState) {
	case noGame:
		break;
	case heroType1State:
	case heroPower1State:
	case heroType2State:
		processPowerHero(line, numline);
		break;
	case mulliganState:
		// processPowerMulligan(line, numline);
		// break;
	case inGameState:
		process_power_game(line, numline);
		break;
	}
}

void
GameWatcher::processPowerHero(const QString& line, qint64 numline)
{
	if (powerState == heroPower1State) {
		if (line.contains(
			QRegularExpression("Creating ID=\\d+ CardID=(\\w+)"),
			match)) {
			powerState  = heroType2State;
			QString hp1 = match->captured(1);
			// emit pDebug("Skip hero power 1: " + hp1 + "
			// (powerState = heroType2State)", numLine);
		}
	} else	  // powerState == heroType1State || powerState ==
		  // heroType2State
	{
		if (line.contains(QRegularExpression(
				      "Creating ID=\\d+ CardID=HERO_(\\d+)"),
				  match)) {
			if (powerState == heroType1State) {
				powerState = heroPower1State;
				hero1	   = match->captured(1);
				// emit pDebug("Found hero 1: " + hero1 + "
				// (powerState = heroPower1State)", numLine);
			} else	  // if(powerState == heroType2State
			{
				powerState = mulliganState;
				hero2	   = match->captured(1);
				// emit pDebug("Found hero 2: " + hero2 + "
				// (powerState = mulliganState)", numLine);
			}
		}
	}
}

void
GameWatcher::process_power_game(const QString& line, qint64 numline)
{
	// Jugador/Enemigo names, playerTag y firstPlayer
	// GameState.DebugPrintEntityChoices() - id=1 Player=UNKNOWN HUMAN
	// PLAYER TaskList= ChoiceType=MULLIGAN CountMin=0 CountMax=3
	// GameState.DebugPrintEntityChoices() - id=2 Player=triodo#2541
	// TaskList= ChoiceType=MULLIGAN CountMin=0 CountMax=5
	if (line.contains(
		QRegularExpression(
		    "GameState\\.DebugPrintEntityChoices\\(\\) - id=(\\d+) Player=(.*) TaskList=\\d* ChoiceType=MULLIGAN CountMin=0 CountMax=(\\d+)"),
		match)) {
		int id	     = match->captured(1).toInt();
		QString name = match->captured(2);

		emit player_name(id, name);
		/*
		QString numCards = match->captured(3);

		if (player.toInt() == 1) {
		    name1 = playerName;

		    // emit pDebug("Found player 1: " + name1, numLine);
		} else if (player.toInt() == 2) {
		    name2 = playerName;
		    // emit pDebug("Found player 2: " + name2, numLine);
		} else
		    // emit pDebug("Read invalid PlayerID value: " + player,
		numLine, DebugLevel::Error);

		    //No se usa. El playerID se calcula (junto al playerTag) al
		cargar el retrato del heroe en processZone. if
		(playerTag.isEmpty() && playerID == player.toInt()) { playerTag
		= (playerID == 1) ? name1 : name2;
		    // emit pDebug("Found playerTag: " + playerTag, numLine);
		}

		if (numCards == "3") {
		    firstPlayer = playerName;
		    // emit pDebug("Found First Player: " + firstPlayer,
		numLine);
		}
		*/
	}

	// MULLIGAN DONE
	// GameState.DebugPrintPower() -     TAG_CHANGE Entity=fayatime
	// tag=MULLIGAN_STATE value=DONE GameState.DebugPrintPower() -
	// TAG_CHANGE Entity=Винсент tag=MULLIGAN_STATE value=DONE
	else if (line.contains(QRegularExpression(
				   "Entity=(.+) tag=MULLIGAN_STATE value=DONE"),
			       match)) {
		// Player mulligan
		if (match->captured(1) == playerTag) {
			if (!mulliganPlayerDone) {
				// emit pDebug("Player mulligan end.", numLine);
				mulliganPlayerDone = true;
				turn		   = 1;
				emit clearDrawList();

				if (mulliganEnemyDone) {
					// turn = 1;
					powerState = inGameState;
					// emit pDebug("Mulligan phase end
					// (powerState = inGameState)",
					// numLine);
				}
			}
		}
		// Enemy mulligan
		else {
			if (!mulliganEnemyDone) {
				if (firstPlayer == playerTag) {
					// Convertir ultima carta en moneda
					// enemiga
					// emit pDebug("Enemy: Coin created.",
					// 0);
					emit lastHandCardIsCoin();
				}
				// emit pDebug("Enemy mulligan end.", numLine);
				mulliganEnemyDone = true;
				turn		  = 1;

				if (mulliganPlayerDone) {
					// turn = 1;
					powerState = inGameState;
					// emit pDebug("Mulligan phase end
					// (powerState = inGameState)",
					// numLine);
				}
			}
		}
	}

	// GameState.DebugPrintEntityChoices() -   Entities[4]=[entityName=La
	// moneda id=68 zone=HAND zonePos=5 cardId=GAME_005 player=2]
	// GameState.DebugPrintEntityChoices() - Entities[4]=[entityName=UNKNOWN
	// ENTITY [cardType=INVALID] id=68 zone=HAND zonePos=5 cardId= player=1]
	else if (
	    line.contains(
		QRegularExpression(
		    "GameState\\.DebugPrintEntityChoices\\(\\) - *"
		    "Entities\\[4\\]=\\[entityName=.* id=(\\d+) zone=HAND zonePos=5 cardId=(.*) player=\\d+\\]"),
		match)) {
		int id	     = match->captured(1).toInt();
		QString code = match->captured(2);
		// emit pDebug("Coin ID: " + id, numLine);
		emit coin_id(id, code);
	}
	// SHOW_ENTITY tag
	// tag=HEALTH value=1
	if (line.contains(
		QRegularExpression("PowerTaskList\\.DebugPrintPower\\(\\) - *"
				   "tag=(\\w+) value=(\\w+)"),
		match)) {
		QString tag   = match->captured(1);
		QString value = match->captured(2);
		if (tag == "DAMAGE" || tag == "ATK" || tag == "HEALTH" ||
		    tag == "EXHAUSTED" || tag == "DIVINE_SHIELD" ||
		    tag == "STEALTH" || tag == "TAUNT" || tag == "CHARGE" ||
		    tag == "RUSH" || tag == "ARMOR" || tag == "FROZEN" ||
		    tag == "WINDFURY" || tag == "SILENCED" ||
		    tag == "CONTROLLER" || tag == "TO_BE_DESTROYED" ||
		    tag == "AURA" || tag == "CANT_BE_DAMAGED" ||
		    tag == "SHOULDEXITCOMBAT" || tag == "ZONE" ||
		    tag == "LINKED_ENTITY" || tag == "DURABILITY" ||
		    tag == "COST") {
			// TODO: use enemy player id!!!
			emit tag_change(last_entity_id,
					"",
					tag,
					value,
					"",
					last_entity_player);
		}
		// if (tag == "ATK" || tag == "HEALTH") {
		// emit pDebug((lastShowEntity.isPlayer ? QString("Player") :
		// QString("Enemy")) + ": SHOW_TAG(" + tag + ")= " + value,
		// numLine);
		// if (lastShowEntity.id == -1)
		// emit pDebug("Show entity id missing.", numLine,
		// DebugLevel::Error);
		// if (lastShowEntity.isPlayer)

		// else
		// emit tag_change(lastShowEntity.id, "", tag, value, "", -1);
		//}
		// En un futuro quizas haya que distinguir entre cambios en zone
		// HAND o PLAY, por ahora son siempre cambios en PLAY

		// Tag avanzados para CHANGE_ENTITY - Updating Entity=
		// No son necesarios ya que al hacer el update entity emit
		// minionCodeChange que cambiara el codigo del minion leyendo
		// del json todos sus atributos correctos.
		//                    || (lastShowEntity.trackAllTags && (tag ==
		//                    "DAMAGE" || /*tag == "EXHAUSTED" ||*/
		//                     tag == "DIVINE_SHIELD" || tag ==
		//                     "STEALTH" || tag == "TAUNT" || tag ==
		//                     "CHARGE" || tag == "RUSH" || tag ==
		//                     "FROZEN" || tag == "WINDFURY" || tag ==
		//                     "AURA"))
	} else {
		// Reiniciamos lastShowEntity
		last_entity_id = -1;

		// TAG_CHANGE jugadores conocido
		// D 10:48:46.1127070 PowerTaskList.DebugPrintPower() -
		// TAG_CHANGE Entity=SerKolobok tag=RESOURCES value=3 D
		// 20:08:22.6854340 PowerTaskList.DebugPrintPower() - TAG_CHANGE
		// Entity=The Innkeeper tag=OVERLOAD_OWED value=2
		if (line.contains(
			QRegularExpression(
			    "PowerTaskList\\.DebugPrintPower\\(\\) - *TAG_CHANGE "
			    "Entity=(.*) tag=(\\w+) value=(\\w+)"),
			match)) {
			QString name  = match->captured(1);
			QString tag   = match->captured(2);
			QString value = match->captured(3);
			bool isPlayer = (name == playerTag);

			if (tag == "RESOURCES" || tag == "RESOURCES_USED" ||
			    tag == "CURRENT_SPELLPOWER" ||
			    tag == "OVERLOAD_OWED") {
				// emit pDebug((isPlayer ? QString("Player") :
				// QString("Enemy")) + ": TAG_CHANGE(" + tag +
				// ")= " + value + " -- Name: " + name,
				// numLine);
				if (!playerTag.isEmpty()) {
					if (isPlayer)
						emit playerTagChange(tag,
								     value);
					else
						emit enemyTagChange(tag, value);
				} else {
					emit unknownTagChange(tag, value);
				}
			}
		}

		// TAG_CHANGE desconocido
		// TAG_CHANGE Entity=[entityName=UNKNOWN ENTITY
		// [cardType=INVALID] id=49 zone=HAND zonePos=3 cardId=
		// player=2] tag=CLASS value=MAGE TAG_CHANGE
		// Entity=[entityName=UNKNOWN ENTITY [cardType=INVALID] id=37
		// zone=HAND zonePos=2 cardId= player=2] tag=CLASS value=MAGE
		if (line.contains(
			QRegularExpression(
			    "PowerTaskList\\.DebugPrintPower\\(\\) - *TAG_CHANGE "
			    "Entity=\\[entityName=UNKNOWN ENTITY \\[cardType=INVALID\\] id=(\\d+) zone=(\\w+) zonePos=\\d+ cardId= player=(\\d+)\\] "
			    "tag=(\\w+) value=(\\w+)"),
			match)) {
			QString id    = match->captured(1);
			QString zone  = match->captured(2);
			int player    = match->captured(3).toInt();
			QString tag   = match->captured(4);
			QString value = match->captured(5);

			if (tag == "CLASS") {
				// emit pDebug((isPlayer ? QString("Player") :
				// QString("Enemy")) + ": Secret hero = " +
				// value + " -- Id: " + id, numLine);
				if (value == "MAGE")
					secretHero = MAGE;
				else if (value == "HUNTER")
					secretHero = HUNTER;
				else if (value == "PALADIN")
					secretHero = PALADIN;
				else if (value == "ROGUE")
					secretHero = ROGUE;
			}
			// Justo antes de jugarse ARMS_DEALING se pone a 0, si
			// no lo evitamos el minion no se actualizara desde la
			// carta
			else if (tag == "ARMS_DEALING" && value.toInt() != 0) {
				// emit pDebug((isPlayer ? QString("Player") :
				// QString("Enemy")) + ": TAG_CHANGE(" + tag +
				// ")= " + value + " -- Id: " + id, numLine);
				emit buffHandCard(id.toInt());
			} else if (tag == "DAMAGE" || tag == "ATK" ||
				   tag == "HEALTH" || tag == "EXHAUSTED" ||
				   tag == "DIVINE_SHIELD" || tag == "STEALTH" ||
				   tag == "TAUNT" || tag == "CHARGE" ||
				   tag == "RUSH" || tag == "ARMOR" ||
				   tag == "FROZEN" || tag == "WINDFURY" ||
				   tag == "SILENCED" || tag == "CONTROLLER" ||
				   tag == "TO_BE_DESTROYED" || tag == "AURA" ||
				   tag == "CANT_BE_DAMAGED" ||
				   tag == "SHOULDEXITCOMBAT" || tag == "ZONE" ||
				   tag == "LINKED_ENTITY" ||
				   tag == "DURABILITY" || tag == "COST") {
				// emit pDebug((isPlayer ? QString("Player") :
				// QString("Enemy")) + ": MINION/CARD
				// TAG_CHANGE(" + tag + ")= " + value + " -- Id:
				// " + id, numLine);
				emit tag_change(id.toInt(),
						"",
						tag,
						value,
						zone,
						player);
			}
		}

		// TAG_CHANGE conocido
		// PowerTaskList aparece segundo pero hay acciones que no tienen
		// GameState, como el damage del maestro del acero herido
		// GameState.DebugPrintPower() -         TAG_CHANGE
		// Entity=[entityName=Déspota del templo id=36 zone=PLAY
		// zonePos=1 cardId=EX1_623 player=2] tag=DAMAGE value=0
		// GameState.DebugPrintPower() -     TAG_CHANGE
		// Entity=[entityName=Déspota del templo id=36 zone=PLAY
		// zonePos=1 cardId=EX1_623 player=2] tag=ATK value=3
		else if (
		    line.contains(
			QRegularExpression(
			    "PowerTaskList\\.DebugPrintPower\\(\\) - *TAG_CHANGE "
			    "Entity=\\[entityName=(.*) id=(\\d+) zone=(\\w+) zonePos=\\d+ cardId=(\\w*) player=(\\d+)\\] "
			    "tag=(\\w+) value=(\\w+)"),
			match)) {
			QString name   = match->captured(1);
			int id	       = match->captured(2).toInt();
			QString zone   = match->captured(3);
			QString cardId = match->captured(4);
			int player     = match->captured(5).toInt();
			QString tag    = match->captured(6);
			QString value  = match->captured(7);

			if (tag == "DAMAGE" || tag == "ATK" ||
			    tag == "HEALTH" || tag == "EXHAUSTED" ||
			    tag == "DIVINE_SHIELD" || tag == "STEALTH" ||
			    tag == "TAUNT" || tag == "CHARGE" ||
			    tag == "RUSH" || tag == "ARMOR" ||
			    tag == "FROZEN" || tag == "WINDFURY" ||
			    tag == "SILENCED" || tag == "CONTROLLER" ||
			    tag == "TO_BE_DESTROYED" || tag == "AURA" ||
			    tag == "CANT_BE_DAMAGED" ||
			    tag == "SHOULDEXITCOMBAT" || tag == "ZONE" ||
			    tag == "LINKED_ENTITY" || tag == "DURABILITY" ||
			    tag == "COST") {
				emit tag_change(id,
						cardId,
						tag,
						value,
						zone,
						player);
			}
		}

		// Enemigo accion desconocida
		// BLOCK_START BlockType=PLAY Entity=[entityName=UNKNOWN ENTITY
		// [cardType=INVALID] id=29 zone=HAND zonePos=2 cardId=
		// player=1]
		// EffectCardId=System.Collections.Generic.List`1[System.String]
		// EffectIndex=0 Target=0 SubOption=-1
		else if (
		    line.contains(
			QRegularExpression(
			    "PowerTaskList\\.DebugPrintPower\\(\\) - BLOCK_START BlockType=(\\w+) "
			    "Entity=\\[entityName=UNKNOWN ENTITY \\[cardType=INVALID\\] id=(\\d+) zone=\\w+ zonePos=\\d+ cardId= player=\\d+\\]"),
			match)) {
			QString blockType = match->captured(1);
			QString id	  = match->captured(2);

			// ULTIMO TRIGGER SPECIAL CARDS, con o sin objetivo
			// emit pDebug("Trigger(" + blockType + ") desconocido.
			// Id: " + id, numLine);
			emit specialCardTrigger("", blockType, id.toInt(), -1);
		}

		// SHOW_ENTITY conocido
		// SHOW_ENTITY - Updating Entity=[entityName=Maestra de secta
		// id=50 zone=DECK zonePos=0 cardId= player=2] CardID=EX1_595
		// SHOW_ENTITY - Updating Entity=[entityName=Turbocerdo con
		// pinchos id=18 zone=DECK zonePos=0 cardId=CFM_688 player=1]
		// CardID=CFM_688
		else if (
		    line.contains(
			QRegularExpression(
			    "PowerTaskList\\.DebugPrintPower\\(\\) - *SHOW_ENTITY - Updating "
			    "Entity=\\[entityName=.* id=(\\d+) zone=\\w+ zonePos=\\d+ cardId=\\w* player=(\\d+)\\] "
			    "CardID=(.*)"),
			match)) {
			int id	      = match->captured(1).toInt();
			int player    = match->captured(2).toInt();
			QString code  = match->captured(3).simplified();
			bool isPlayer = (player == playerID);

			// emit pDebug((isPlayer ? QString("Player") :
			// QString("Enemy")) + ": SHOW_ENTITY -- Id: " + id,
			// numLine);
			last_entity_id	   = id;
			last_entity_player = player;
			emit show_entity(id, player, code);
		}

		// SHOW_ENTITY desconocido
		// SHOW_ENTITY - Updating Entity=[entityName=UNKNOWN ENTITY
		// [cardType=INVALID] id=58 zone=HAND zonePos=3 cardId=
		// player=2] CardID=EX1_011
		else if (
		    line.contains(
			QRegularExpression(
			    "PowerTaskList\\.DebugPrintPower\\(\\) - *SHOW_ENTITY - Updating "
			    "Entity=\\[entityName=UNKNOWN ENTITY \\[cardType=INVALID\\] id=(\\d+) zone=\\w+ zonePos=\\d+ cardId= player=(\\d+)\\] "
			    "CardID=\\w+"),
			match)) {
			int id	   = match->captured(1).toInt();
			int player = match->captured(2).toInt();

			// emit pDebug((isPlayer ? QString("Player") :
			// QString("Enemy")) + ": SHOW_ENTITY -- Id: " + id,
			// numLine);
			last_entity_id	   = id;
			last_entity_player = player;
			// emit showEntity(id, player);
		}

		// FULL_ENTITY conocido
		// FULL_ENTITY - Updating [entityName=Recluta Mano de Plata
		// id=95 zone=PLAY zonePos=3 cardId=CS2_101t player=2]
		// CardID=CS2_101t
		else if (
		    line.contains(
			QRegularExpression(
			    "PowerTaskList\\.DebugPrintPower\\(\\) - *FULL_ENTITY - Updating "
			    "\\[entityName=.* id=(\\d+) zone=\\w+ zonePos=\\d+ cardId=\\w* player=(\\d+)\\] "
			    "CardID=(.*)"),
			match)) {
			int id	     = match->captured(1).toInt();
			int player   = match->captured(2).toInt();
			QString code = match->captured(3).simplified();

			// emit pDebug((isPlayer ? QString("Player") :
			// QString("Enemy")) + ": FULL_ENTITY -- Id: " + id,
			// numLine);
			last_entity_id	   = id;
			last_entity_player = player;

			emit updated_card(player, id, code);
		}

		// FULL_ENTITY known
		// FULL_ENTITY - Creating ID=78 CardID=TRL_012
		else if (
		    line.contains(
			QRegularExpression(
			    "GameState\\.DebugPrintPower\\(\\) - *FULL_ENTITY - Creating ID=(\\d+) CardID=(\\w+)"),
			match)) {
			int id	     = match->captured(1).toInt();
			QString code = match->captured(2);

			emit created_card(id, code);
		}

		// CHANGE_ENTITY conocido
		// CHANGE_ENTITY - Updating Entity=[entityName=Aullavísceras
		// id=53 zone=HAND zonePos=3 cardId=EX1_411 player=2]
		// CardID=OG_031
		else if (
		    line.contains(
			QRegularExpression(
			    "PowerTaskList\\.DebugPrintPower\\(\\) - *CHANGE_ENTITY - Updating Entity="
			    "\\[entityName=.* id=(\\d+) zone=(\\w+) zonePos=\\d+ cardId=\\w* player=(\\d+)\\] "
			    "CardID=(\\w+)"),
			match)) {
			int id	      = match->captured(1).toInt();
			QString zone  = match->captured(2);
			int player    = match->captured(3).toInt();
			QString code  = match->captured(4);
			bool isPlayer = (player == playerID);

			// emit pDebug((isPlayer ? QString("Player") :
			// QString("Enemy")) + ": CHANGE_ENTITY -- Id: " + id +
			// " to Code: " + newCardId + " in Zone: " + zone,
			// numLine);
			last_entity_id	   = id;
			last_entity_player = player;
			/*
			if (zone == "HAND") {
			    if (isPlayer)
				emit playerCardCodeChange(id, code);
			} else if (zone == "PLAY") {
			    emit minionCodeChange(isPlayer, id, code);
			}
	    */
			emit update_entity(id, zone, player, code);
		}

		// Jugador/Enemigo accion con/sin objetivo
		// PowerTaskList.DebugPrintPower() - BLOCK_START
		// BlockType=ATTACK Entity=[entityName=Jinete de lobos id=45
		// zone=PLAY zonePos=1 cardId=CS2_124 player=2] EffectCardId=
		// EffectIndex=-1 Target=[entityName=Jaina Valiente id=64
		// zone=PLAY zonePos=0 cardId=HERO_08 player=1]
		// PowerTaskList.DebugPrintPower() - BLOCK_START
		// BlockType=TRIGGER Entity=[entityName=Trepadora embrujada
		// id=12 zone=GRAVEYARD zonePos=0 cardId=FP1_002 player=1]
		// EffectCardId= EffectIndex=0 Target=0
		// PowerTaskList.DebugPrintPower() - BLOCK_START BlockType=POWER
		// Entity=[entityName=Elemental de Escarcha id=43 zone=PLAY
		// zonePos=1 cardId=EX1_283 player=2] EffectCardId=
		// EffectIndex=-1 Target=[entityName=Trituradora antigua de
		// Sneed id=23 zone=PLAY zonePos=5 cardId=GVG_114 player=1]
		// PowerTaskList.DebugPrintPower() - BLOCK_START
		// BlockType=FATIGUE Entity=[entityName=Malfurion Tempestira
		// id=76 zone=PLAY zonePos=0 cardId=HERO_06 player=1]
		// EffectCardId= EffectIndex=0 Target=0
		else if (
		    line.contains(
			QRegularExpression(
			    "PowerTaskList\\.DebugPrintPower\\(\\) - BLOCK_START BlockType=(\\w+) "
			    "Entity=\\[entityName=(.*) id=(\\d+) zone=(\\w+) zonePos=\\d+ cardId=(\\w+) player=(\\d+)\\] "
			    "EffectCardId=.* EffectIndex=-?\\d+ "
			    "Target=(?:\\[entityName=(.*) id=(\\d+) zone=(\\w+) zonePos=\\d+ cardId=(\\w+) player=\\d+\\])?"),
			match)) {
			QString blockType = match->captured(1);
			QString name1	  = match->captured(2);
			QString id1	  = match->captured(3);
			QString zone	  = match->captured(4);
			QString cardId1	  = match->captured(5);
			QString player1	  = match->captured(6);
			QString name2	  = match->captured(7);
			QString id2	  = match->captured(8);
			QString zone2	  = match->captured(9);
			QString cardId2	  = match->captured(10);
			bool isPlayer	  = (player1.toInt() == playerID);

			// ULTIMO TRIGGER SPECIAL CARDS, con o sin objetivo
			// emit pDebug("Trigger(" + blockType + "): " + name1 +
			// " (" + cardId1 + ")" + (name2.isEmpty() ? "" : " -->
			// " + name2 + " (" + cardId2 + ")"), numLine);
			emit specialCardTrigger(cardId1,
						blockType,
						id1.toInt(),
						id2.isEmpty() ? -1 :
								      id2.toInt());
			// if (isHeroPower(cardId1) && isPlayerTurn &&
			// player1.toInt() == playerID)
			//    emit playerHeroPower();

			// Accion sin objetivo
			if (zone2.isEmpty()) {
				if (blockType == "FATIGUE" && zone == "PLAY") {
					// emit pDebug((isPlayer ?
					// QString("Player") : QString("Enemy"))
					// + ": Fatigue damage.", numLine);
					if (advanceTurn(isPlayer))
						emit new_turn(isPlayerTurn,
							      turnReal);
				}
			}

			// Accion con objetivo en PLAY
			else if (zone2 == "PLAY") {
				// Jugador juega carta con objetivo en PLAY, No
				// enemigo pq BlockType=PLAY es de entity
				// desconocida para el enemigo
				if (blockType == "PLAY" && zone == "HAND") {
					emit player_action_played();
					/*
					card_type type;
					collection->get_card_type(qPrintable(cardId2),
					type);

					if (type == SPELL) {
					    // emit pDebug((isPlayer ?
					QString("Player") : QString("Enemy")) +
					": Spell obj played: " + name1 + " on
					target " + name2, numLine); if (cardId2
					== MAD_SCIENTIST) {
						// emit pDebug("Skip spell obj
					testing (Mad Scientist died).", 0); }
					else if (isPlayer && isPlayerTurn) { if
					(cardId2.startsWith("HERO")) emit
					playerSpellObjHeroPlayed(); else emit
					playerSpellObjMinionPlayed();
					    }
					} else {
					    // emit pDebug((isPlayer ?
					QString("Player") : QString("Enemy")) +
					": Minion/weapon obj played: " + name1 +
					" target " + name2, numLine); if
					(isPlayer && isPlayerTurn) { if
					(cardId2.startsWith("HERO")) emit
					playerBattlecryObjHeroPlayed();
					//Secreto Evasion
						// else emit
					playerBattlecryObjMinionPlayed();//No se
					usa aun
					    }
					}
					*/
				}

				// Jugador/enemigo causa accion con objetivo en
				// PLAY
				else if (blockType == "POWER") {
					if (isPlayer)
						emit playerCardObjPlayed(
						    cardId1,
						    id1.toInt(),
						    id2.toInt());
					else
						emit enemyCardObjPlayed(
						    cardId1,
						    id1.toInt(),
						    id2.toInt());
				}

				// Jugador/Enemigo ataca (esbirro/heroe VS
				// esbirro/heroe)
				else if (blockType == "ATTACK" &&
					 zone == "PLAY") {
					emit zonePlayAttack(cardId1,
							    id1.toInt(),
							    id2.toInt());

					if (cardId1.contains("HERO")) {
						if (cardId2.contains("HERO")) {
							// emit pDebug((isPlayer
							// ? QString("Player") :
							// QString("Enemy")) +
							// ": Attack: " + name1
							// + " (heroe)vs(heroe)
							// " + name2, numLine);
							if (isPlayer &&
							    isPlayerTurn)
								emit playerAttack(
								    true,
								    true,
								    playerMinions,
								    id1.toInt(),
								    id2.toInt());
						} else {
							// emit pDebug((isPlayer
							// ? QString("Player") :
							// QString("Enemy")) +
							// ": Attack: " + name1
							// + " (heroe)vs(minion)
							// " + name2, numLine);
							/*if(match->captured(5)
							== MAD_SCIENTIST) //Son
							comprobaciones now de
							secretos
							{
							    // emit
							pDebug("Saltamos
							comprobacion de
							secretos";
							}
							else */
							if (isPlayer &&
							    isPlayerTurn)
								emit playerAttack(
								    true,
								    false,
								    playerMinions,
								    id1.toInt(),
								    id2.toInt());
						}
					} else {
						if (cardId2.contains("HERO")) {
							// emit pDebug((isPlayer
							// ? QString("Player") :
							// QString("Enemy")) +
							// ": Attack: " + name1
							// + " (minion)vs(heroe)
							// " + name2, numLine);
							if (isPlayer &&
							    isPlayerTurn)
								emit playerAttack(
								    false,
								    true,
								    playerMinions,
								    id1.toInt(),
								    id2.toInt());
						} else {
							// emit pDebug((isPlayer
							// ? QString("Player") :
							// QString("Enemy")) +
							// ": Attack: " + name1
							// + "
							// (minion)vs(minion) "
							// + name2, numLine);
							/*if(match->captured(5)
							== MAD_SCIENTIST) //Son
							comprobaciones now de
							secretos
							{
							    // emit
							pDebug("Saltamos
							comprobacion de
							secretos";
							}
							else */
							if (isPlayer &&
							    isPlayerTurn)
								emit playerAttack(
								    false,
								    false,
								    playerMinions,
								    id1.toInt(),
								    id2.toInt());
						}
					}
				}
			}
		}
	}
}

void
GameWatcher::processZone(const QString& line, qint64 numline)
{
	if (powerState == noGame)
		return;

	// Carta desconocida
	//[entityName=UNKNOWN ENTITY [cardType=INVALID] id=69 zone=HAND
	// zonePos=0 cardId= player=2] zone from OPPOSING DECK -> OPPOSING HAND
	if (line.contains(
		QRegularExpression(
		    "\\[entityName=UNKNOWN ENTITY \\[cardType=INVALID\\] id=(\\d+) zone=\\w+ zonePos=\\d+ cardId= player=\\d+\\] zone from "
		    "(\\w+ \\w+(?: \\(Weapon\\))?)? -> (\\w+ \\w+(?: \\((?:Weapon|Hero|Hero Power)\\))?)?"),
		match)) {
		QString id	 = match->captured(1);
		QString zoneFrom = match->captured(2);
		QString zoneTo	 = match->captured(3);

		// Enemigo juega secreto
		if (zoneTo == "OPPOSING SECRET") {
			// emit pDebug("Enemy: Secret played. ID: " + id,
			// numLine);
			emit enemySecretPlayed(id.toInt(),
					       secretHero,
					       scr_state);
		}

		// Enemigo roba carta desconocida
		else if (zoneTo == "OPPOSING HAND") {
			if (mulliganEnemyDone) {
				// Enemigo roba carta de deck
				if (zoneFrom == "OPPOSING DECK") {
					bool advance = advanceTurn(false);
					// emit pDebug("Enemy: Card drawn. ID: "
					// + id, numLine);
					if (advance && turnReal == 1)
						emit new_turn(isPlayerTurn,
							      turnReal);
					emit enemy_carddraw(id.toInt(),
							    turnReal);
					if (advance && turnReal != 1)
						emit new_turn(isPlayerTurn,
							      turnReal);
				}
				// Enemigo roba carta especial del vacio
				else if (zoneFrom.isEmpty()) {
					// emit pDebug("Enemy: Special card
					// drawn. ID: " + id, numLine);
					emit enemy_carddraw(id.toInt(),
							    turnReal,
							    true);
				}
			} else {
				// Enemigo roba starting card
				// emit pDebug("Enemy: Starting card drawn. ID:
				// " + id, numLine);
				emit enemy_carddraw(id.toInt());
			}
		}

		// Jugador, OUTSIDER desconocido a deck
		else if (zoneTo == "FRIENDLY DECK") {
			if (mulliganPlayerDone) {
				// emit pDebug("Player: Outsider unknown card to
				// deck. ID: " + id, numLine);
				emit card_return_deck("", id.toInt());
			}
		}

		// Enemigo juega carta desconocida
		if (zoneFrom == "OPPOSING HAND") {
			// Carta devuelta al mazo en Mulligan
			if (zoneTo == "OPPOSING DECK") {
				// emit pDebug("Enemy: Starting card returned.
				// ID: " + id, numLine);
				emit card_played(1, id.toInt(), "", true);
			} else {
				// emit pDebug("Enemy: Unknown card played. ID:
				// " + id, numLine);
				emit card_played(1, id.toInt());
			}
		}

		// Jugador, elimina OUTSIDER desconocido de deck
		else if (zoneFrom == "FRIENDLY DECK") {
			// emit pDebug("Player: Unknown card removed from deck.
			// ID: " + id, numLine);
			emit player_carddraw("", id.toInt());
		}
	}

	// Carta conocida
	//[entityName=Shellshifter id=32 zone=HAND zonePos=0 cardId=UNG_101
	// player=1] zone from FRIENDLY DECK -> FRIENDLY HAND
	//[entityName=Shellshifter id=32 zone=DECK zonePos=0 cardId= player=1]
	// zone from FRIENDLY HAND -> FRIENDLY DECK
	else if (
	    line.contains(
		QRegularExpression(
		    "\\[entityName=(.*) id=(\\d+) zone=\\w+ zonePos=(\\d+) cardId=(\\w*) player=(\\d+)\\] zone from "
		    "(\\w+ \\w+(?: \\(Weapon\\))?)? -> (\\w+ \\w+(?: \\((?:Weapon|Hero|Hero Power)\\))?)?"),
		match)) {
		QString name	 = match->captured(1);
		QString id	 = match->captured(2);
		QString zonePos	 = match->captured(3);
		QString cardId	 = match->captured(4);
		QString player	 = match->captured(5);
		QString zoneFrom = match->captured(6);
		QString zoneTo	 = match->captured(7);
		/*
		if (cardId.isEmpty()) {
		    card buffer;

		    if (collection->find_by_name(qPrintable(name), buffer) !=
		EXE_OK) {
			//return false;
		    }

		    cardId = buffer.code;

		    free_card(buffer);
		}
		*/
		// Jugador juega secreto
		if (zoneTo == "FRIENDLY SECRET" &&
		    zoneFrom != "OPPOSING SECRET") {
			// emit pDebug("Player: Secret played: " + name + " ID:
			// " + id, numLine);
			emit playerSecretPlayed(id.toInt(), cardId);
		}

		// Enemigo roba carta conocida
		else if (zoneTo == "OPPOSING HAND" &&
			 zoneFrom != "OPPOSING HAND") {
			if (mulliganEnemyDone) {
				// Mostramos todas las cartas marcadas como
				// desveladas en el log y establecemos
				// excepciones (isCheatingCard)
				bool advance	  = false;
				bool cheatingCard = isCheatingCard(cardId);
				// if (cheatingCard)
				// emit pDebug("Enemy: Known card to hand from
				// deck (Hidden to avoid cheating): " + name + "
				// ID: " + id, numLine);
				// else
				// emit pDebug("Enemy: Known card to hand: " +
				// name + " ID: " + id, numLine);
				if (zoneFrom == "OPPOSING DECK") {
					advance = advanceTurn(false);
					if (!cheatingCard)
						emit enemyKnownCardDraw(
						    id.toInt(),
						    cardId);
				}
				if (advance && turnReal == 1)
					emit new_turn(isPlayerTurn, turnReal);
				if (cheatingCard)
					emit enemy_carddraw(id.toInt(),
							    turnReal);
				else
					emit enemy_carddraw(id.toInt(),
							    turnReal,
							    false,
							    cardId);
				if (advance && turnReal != 1)
					emit new_turn(isPlayerTurn, turnReal);
			} else {
				// Enemigo roba starting card
				// emit pDebug("Enemy: Starting card drawn
				// (Hidden to avoid cheating). ID: " + id,
				// numLine);
				emit enemy_carddraw(id.toInt());
			}
		}

		// Jugador roba carta conocida
		else if (zoneTo == "FRIENDLY HAND") {
			if (mulliganPlayerDone)	   // Evita que las cartas
						   // iniciales creen un nuevo
						   // Board en PlanHandler al
						   // ser robadas
			{
				// emit pDebug("Player: Known card to hand: " +
				// name + " ID: " + id, numLine);
				bool advance = false;
				if (zoneFrom == "FRIENDLY DECK") {
					advance = advanceTurn(true);
				}
				if (advance && turnReal == 1)
					emit new_turn(isPlayerTurn, turnReal);
				emit playerCardToHand(id.toInt(),
						      cardId,
						      turnReal);
				if (advance && turnReal != 1)
					emit new_turn(isPlayerTurn, turnReal);
			} else {
				// emit pDebug("Player: Starting card to hand: "
				// + name + " ID: " + id, numLine);
				emit playerCardToHand(id.toInt(), cardId, 0);
			}
		}

		// Jugador, OUTSIDER a deck
		else if (zoneTo == "FRIENDLY DECK" &&
			 zoneFrom != "FRIENDLY DECK" &&
			 zoneFrom != "FRIENDLY HAND") {
			// emit pDebug("Player: Outsider card to deck: " + name
			// + " ID: " + id, numLine);
			emit card_return_deck(cardId, id.toInt());
		}

		// Enemigo, nuevo minion en PLAY
		else if (zoneTo == "OPPOSING PLAY" &&
			 zoneFrom != "OPPOSING PLAY") {
			enemyMinions++;
			// emit pDebug("Enemy: Minion moved to OPPOSING PLAY: "
			// + name + " ID: " + id + " Minions: " +
			// QString::number(enemyMinions), numLine);
			if (zoneFrom == "FRIENDLY PLAY")
				emit playerMinionZonePlaySteal(id.toInt(),
							       zonePos.toInt());
			else if (zoneFrom.isEmpty())
				emit enemyMinionZonePlayAddTriggered(
				    cardId,
				    id.toInt(),
				    zonePos.toInt());
			else
				emit enemyMinionZonePlayAdd(cardId,
							    id.toInt(),
							    zonePos.toInt());
		}

		// Jugador, nuevo minion en PLAY
		else if (zoneTo == "FRIENDLY PLAY" &&
			 zoneFrom != "FRIENDLY PLAY") {
			playerMinions++;
			// emit pDebug("Player: Minion moved to FRIENDLY PLAY: "
			// + name + " ID: " + id + " Minions: " +
			// QString::number(playerMinions), numLine);
			if (zoneFrom == "OPPOSING PLAY")
				emit enemyMinionZonePlaySteal(id.toInt(),
							      zonePos.toInt());
			else if (zoneFrom.isEmpty())
				emit playerMinionZonePlayAddTriggered(
				    cardId,
				    id.toInt(),
				    zonePos.toInt());
			else
				emit playerMinionZonePlayAdd(cardId,
							     id.toInt(),
							     zonePos.toInt());
		}

		// Enemigo, carga heroe
		else if (zoneTo == "OPPOSING PLAY (Hero)") {
			// emit pDebug("Enemy: Hero moved to OPPOSING PLAY
			// (Hero): " + name + " ID: " + id, numLine);
			emit enemyHeroZonePlayAdd(cardId, id.toInt());
		}

		// Jugador, carga heroe
		else if (zoneTo == "FRIENDLY PLAY (Hero)") {
			// emit pDebug("Player: Hero moved to FRIENDLY PLAY
			// (Hero): " + name + " ID: " + id, numLine);
			if (playerID == 0) {
				playerID = player.toInt();
				emit enemyHero((playerID == 1) ? hero2 : hero1);
				// emit pDebug("Found playerID: " + player,
				// numLine);

				secretHero = conv_string_number_class(
				    (playerID == 1) ? qPrintable(hero1) :
							    qPrintable(hero2));

				if (playerTag.isEmpty()) {
					playerTag =
					    (playerID == 1) ? name1 : name2;
					// if (!playerTag.isEmpty())
					// emit pDebug("Found playerTag: " +
					// playerTag, numLine);
				}
			}
			emit playerHeroZonePlayAdd(cardId, id.toInt());
		}

		// Enemigo, carga hero power
		else if (zoneTo == "OPPOSING PLAY (Hero Power)") {
			// emit pDebug("Enemy: Hero Power moved to OPPOSING PLAY
			// (Hero Power): " + name + " ID: " + id, numLine);
			emit enemyHeroPowerZonePlayAdd(cardId, id.toInt());
		}

		// Jugador, carga hero power
		else if (zoneTo == "FRIENDLY PLAY (Hero Power)") {
			// emit pDebug("Player: Hero Power moved to FRIENDLY
			// PLAY (Hero Power): " + name + " ID: " + id, numLine);
			emit playerHeroPowerZonePlayAdd(cardId, id.toInt());
		}

		// Enemigo, equipa arma
		else if (zoneTo == "OPPOSING PLAY (Weapon)" &&
			 zoneFrom !=
			     "OPPOSING GRAVEYARD")    // Al reemplazar un arma
						      // por otra, la antigua
						      // va, vuelve y va a
						      // graveyard.
		{
			// emit pDebug("Enemy: Weapon moved to OPPOSING PLAY
			// (Weapon): " + name + " ID: " + id, numLine);
			emit enemyWeaponZonePlayAdd(cardId, id.toInt());
		}

		// Jugador, equipa arma
		else if (zoneTo == "FRIENDLY PLAY (Weapon)" &&
			 zoneFrom != "FRIENDLY GRAVEYARD") {
			// emit pDebug("Player: Weapon moved to FRIENDLY PLAY
			// (Weapon): " + name + " ID: " + id, numLine);
			emit playerWeaponZonePlayAdd(cardId, id.toInt());
		}

		if (zoneFrom == "FRIENDLY SECRET") {
			// Enemigo roba secreto (kezan mystic)
			if (zoneTo == "OPPOSING SECRET") {
				// emit pDebug("Enemy: Secret stolen: " + name +
				// " ID: " + id, numLine);
				emit enemySecretStolen(id.toInt(),
						       cardId,
						       scr_state);
			}
			// Jugador secreto desvelado
			else {
				// emit pDebug("Player: Secret revealed: " +
				// name + " ID: " + id, numLine);
				emit playerSecretRevealed(id.toInt(), cardId);
			}
		}

		else if (zoneFrom == "OPPOSING SECRET") {
			// Jugador roba secreto (kezan mystic)
			if (zoneTo == "FRIENDLY SECRET") {
				// emit pDebug("Player: Secret stolen: " + name
				// + " ID: " + id, numLine);
				emit playerSecretStolen(id.toInt(), cardId);
			}
			// Enemigo secreto desvelado
			else if (zoneTo != "OPPOSING SECRET") {
				// emit pDebug("Enemy: Secret revealed: " + name
				// + " ID: " + id, numLine);
				emit enemySecretRevealed(id.toInt(), cardId);
			}
		}

		// Enemigo juega carta conocida
		else if (zoneFrom == "OPPOSING HAND" &&
			 zoneTo != "OPPOSING HAND") {
			bool discard = false;

			// Enemigo juega hechizo
			if (zoneTo.isEmpty() || zoneTo == "OPPOSING SECRET") {
				// emit pDebug("Enemy: Spell played: " + name +
				// " ID: " + id, numLine);
			}
			// Enemigo juega esbirro
			else if (zoneTo == "OPPOSING PLAY") {
				// emit pDebug("Enemy: Minion played: " + name +
				// " ID: " + id + " Minions: " +
				// QString::number(enemyMinions), numLine);
			}
			// Enemigo juega arma
			else if (zoneTo == "OPPOSING PLAY (Weapon)") {
				// emit pDebug("Enemy: Weapon played: " + name +
				// " ID: " + id, numLine);
			}
			// Enemigo juega heroe
			else if (zoneTo == "OPPOSING PLAY (Hero)") {
				// emit pDebug("Enemy: Hero played: " + name + "
				// ID: " + id, numLine);
			}
			// Enemigo descarta carta
			else if (zoneTo == "OPPOSING GRAVEYARD") {
				// emit pDebug("Enemy: Card discarded: " + name
				// + " ID: " + id, numLine);
				discard = true;
			}
			// Carta devuelta al mazo en Mulligan, Leviatan de
			// llamas
			else if (zoneTo == "OPPOSING DECK") {
				// emit pDebug("Enemy: Starting card returned: "
				// + name + " ID: " + id, numLine);
				discard = true;
			} else {
				// emit pDebug("Enemy: Card moved from hand: " +
				// name + " ID: " + id, numLine, Warning);
				discard = true;
			}

			emit card_played(1, id.toInt(), cardId, discard);
		}

		// Enemigo roba carta overdraw/recruit
		else if (zoneFrom == "OPPOSING DECK" &&
			 (zoneTo == "OPPOSING GRAVEYARD" ||
			  zoneTo == "OPPOSING PLAY")) {
			bool advance = advanceTurn(false);
			// emit pDebug("Enemy: Card from deck skipped hand
			// (overdraw/recruit): " + name + " ID: " + id,
			// numLine);
			emit enemyKnownCardDraw(id.toInt(), cardId);
			if (advance)
				emit new_turn(isPlayerTurn, turnReal);
		}

		// Jugador roba carta conocida
		// Maldición ancestral hace un FRIENDLY DECK --> EMPTY justo
		// despues de meterse en el mazo. CONTRA Joust mechanics mueven
		// la carta mostrada a EMPTY. CONTRA. En deckHandler se evita
		// que la carta se incluya en el mazo (al ser un outsider)
		// Dragon's Fury se mete como outsider en el mazo EMPTY -->
		// FRIENDLY DECK y sale despues de hacer el joust FRIENDLY DECK
		// --> EMPTY. Puesto que AT lo mete en el deck es necesario
		// sacarlo. PRO El mono de Elise vacia el deck a empty y lo
		// rellena de legendarias. PRO Explorar un'goro vacia el deck a
		// empty y lo rellena de explorar un'goro. PRO
		else if (zoneFrom == "FRIENDLY DECK" &&
			 zoneTo != "FRIENDLY DECK") {
			// emit pDebug("Player: Card drawn: " + name + " ID: " +
			// id, numLine);
			// El avance de turno ocurre generalmente en (zoneTo ==
			// "FRIENDLY HAND") pero en el caso de overdraw ocurrira
			// aqui.
			if (mulliganPlayerDone)	   // Evita que las cartas
						   // iniciales creen un nuevo
						   // Board en PlanHandler al
						   // ser robadas
			{
				bool advance = advanceTurn(true);
				if (advance)
					emit new_turn(isPlayerTurn, turnReal);
			}
			emit player_carddraw(cardId, id.toInt());
		}

		// Jugador juega carta conocida
		else if (zoneFrom == "FRIENDLY HAND") {
			bool discard  = false;
			bool mulligan = false;

			// Jugador juega hechizo
			if (zoneTo.isEmpty() ||
			    zoneTo == "FRIENDLY SECRET")    // En spectator mode
							    // los secretos van
							    // FRIENDLY HAND -->
							    // FRIENDLY SECRET
			{
				// emit pDebug("Player: Spell played: " + name +
				// " ID: " + id, numLine);
				if (isPlayerTurn)
					emit playerSpellPlayed(cardId);
			}
			// Jugador juega esbirro
			else if (zoneTo == "FRIENDLY PLAY") {
				// emit pDebug("Player: Minion played: " + name
				// + " ID: " + id + " Minions: " +
				// QString::number(playerMinions), numLine);
				if (isPlayerTurn)
					emit playerMinionPlayed(cardId,
								id.toInt(),
								playerMinions);
			}
			// Jugador juega arma
			else if (zoneTo == "FRIENDLY PLAY (Weapon)") {
				// emit pDebug("Player: Weapon played: " + name
				// + " ID: " + id, numLine);
			}
			// Jugador juega heroe
			else if (zoneTo == "FRIENDLY PLAY (Hero)") {
				// emit pDebug("Player: Hero played: " + name +
				// " ID: " + id, numLine);
			}
			// Jugador descarta carta
			else if (zoneTo == "FRIENDLY GRAVEYARD") {
				// emit pDebug("Player: Card discarded: " + name
				// + " ID: " + id, numLine);
				discard = true;
			}
			// Carta devuelta al mazo en Mulligan
			//(Quizas sea util verificar que estamos en el turno 1 o
			// puede dar problemas en el futuro)
			else if (zoneTo == "FRIENDLY DECK") {
				// emit pDebug("Player: Starting card returned:
				// " + name + " ID: " + id, numLine);
				emit card_return_deck(cardId, id.toInt());
				mulligan = true;
				discard	 = true;
			} else {
				// emit pDebug("Player: Card moved from hand: "
				// + name + " ID: " + id, numLine, Warning);
				discard = true;
			}
			if (!mulligan) {
				emit card_played(0,
						 id.toInt(),
						 cardId,
						 discard);
			}
			if (isPlayerTurn && !discard) {
				// Rat trap/Hidden wisdom control
				playerCardsPlayedThisTurn++;
				if (playerCardsPlayedThisTurn > 2) {
					// emit pDebug("Rat trap/Hidden wisdom
					// tested: This turn cards played: " +
					// QString::number(playerCardsPlayedThisTurn),
					// 0);
					emit _3CardsPlayedTested();
				}
			}
		}

		// Enemigo esbirro muere
		else if (zoneFrom == "OPPOSING PLAY" &&
			 zoneTo != "OPPOSING PLAY") {
			if (enemyMinions > 0)
				enemyMinions--;
			// emit pDebug("Enemy: Minion removed from OPPOSING
			// PLAY: " + name + " ID: " + id + " Minions: " +
			// QString::number(enemyMinions), numLine);
			if (zoneTo != "FRIENDLY PLAY")
				emit enemyMinionZonePlayRemove(id.toInt());

			if (zoneTo == "OPPOSING GRAVEYARD") {
				emit enemyMinionGraveyard(id.toInt(),
							  cardId,
							  isPlayerTurn);
				if (isPlayerTurn) {
					// Avenge control
					if (enemyMinionsAliveForAvenge == -1) {
						if (cardId == MAD_SCIENTIST) {
							// emit pDebug("Skip
							// avenge testing (Mad
							// Scientist died).",
							// 0);
						} else {
							enemyMinionsAliveForAvenge =
							    enemyMinions;
							QTimer::singleShot(
							    1000,
							    this,
							    SLOT(
								checkAvenge()));
						}
					} else
						enemyMinionsAliveForAvenge--;

					// Hand of salvation control
					enemyMinionsDeadThisTurn++;
					if (enemyMinionsDeadThisTurn > 1) {
						if (cardId == MAD_SCIENTIST) {
							// emit pDebug("Skip
							// Hand of salvation
							// testing (Mad
							// Scientist died).",
							// 0);
						} else {
							// emit pDebug("Hand of
							// salvation tested:
							// This turn died: " +
							// QString::number(enemyMinionsDeadThisTurn),
							// 0);
							emit
							handOfSalvationTested();
						}
					}
				}
			}
		}

		// Jugador esbirro muere
		else if (zoneFrom == "FRIENDLY PLAY" &&
			 zoneTo != "FRIENDLY PLAY") {
			if (playerMinions > 0)
				playerMinions--;
			// emit pDebug("Player: Minion removed from FRIENDLY
			// PLAY: " + name + " ID: " + id + " Minions: " +
			// QString::number(playerMinions), numLine);
			if (zoneTo != "OPPOSING PLAY")
				emit playerMinionZonePlayRemove(id.toInt());

			if (zoneTo == "FRIENDLY GRAVEYARD")
				emit playerMinionGraveyard(id.toInt(), cardId);
		}

		// Enemigo, deshecha arma
		else if (zoneFrom == "OPPOSING PLAY (Weapon)") {
			// emit pDebug("Enemy: Weapon moved from OPPOSING PLAY
			// (Weapon): " + name + " ID: " + id, numLine);
			emit enemyWeaponZonePlayRemove(id.toInt());

			if (zoneTo == "OPPOSING GRAVEYARD")
				emit enemyWeaponGraveyard(id.toInt(), cardId);
		}

		// Jugador, deshecha arma
		else if (zoneFrom == "FRIENDLY PLAY (Weapon)") {
			// emit pDebug("Player: Weapon moved from FRIENDLY PLAY
			// (Weapon): " + name + " ID: " + id, numLine);
			emit playerWeaponZonePlayRemove(id.toInt());

			if (zoneTo == "FRIENDLY GRAVEYARD")
				emit playerWeaponGraveyard(id.toInt(), cardId);
		}
	}

	// Jugador/Enemigo esbirro cambia pos
	// No podemos usar zonePos= porque para los esbirros del jugador que
	// pasan a una posicion mayor muestra su posicion origen Toddo comentado
	// porque esta forma de contar el numero de esbirros puede producir
	// errores. Ej: Si un esbirro con deathrattle produce otro esbirro.
	// Primero se cambia la pos de los esbirros a la dcha y despues se
	// genran los esbirros de deathrattle causando una suma erronea. id=7
	// local=True [entityName=Ingeniera novata id=25 zone=HAND zonePos=5
	// cardId=EX1_015 player=1] pos from 5 -> 3
	else if (
	    line.contains(
		QRegularExpression(
		    "\\[entityName=(.*) id=(\\d+) zone=(?:HAND|PLAY) zonePos=\\d+ cardId=\\w+ player=(\\d+)\\] pos from \\d+ -> (\\d+)"),
		match)) {
		QString name	= match->captured(1);
		QString id	= match->captured(2);
		QString player	= match->captured(3);
		QString zonePos = match->captured(4);
		int zonePosInt	= zonePos.toInt();

		if (zonePosInt > 0)    // En patch 8.2.0 los esbirros se mueven
				       // siempre a 0 y luego a su posicion
		{
			// Jugador esbirro cambia pos
			if (player.toInt() == playerID) {
				//            // emit pDebug("Player: New minion
				//            pos: " +
				//                        name + " >> " +
				//                        zonePos + " Minions: "
				//                        +
				//                        QString::number(playerMinions),
				//                        numLine);
				emit playerMinionPosChange(id.toInt(),
							   zonePosInt);
			}
			// Enemigo esbirro cambia pos
			else {
				//            // emit pDebug("Enemy: New minion
				//            pos: " +
				//                        name + " >> " +
				//                        zonePos + " Minions: "
				//                        +
				//                        QString::number(enemyMinions),
				//                        numLine);
				emit enemyMinionPosChange(id.toInt(),
							  zonePosInt);
			}
		}
		Q_UNUSED(name);
	}
}

void
GameWatcher::checkAvenge()
{
	if (enemyMinionsAliveForAvenge > 0) {
		emit avengeTested();
		// emit pDebug("Avenge tested: Survivors: " +
		// QString::number(enemyMinionsAliveForAvenge), 0);
	} else
		// emit pDebug("Avenge not tested: Survivors: " +
		// QString::number(enemyMinionsAliveForAvenge), 0);
		enemyMinionsAliveForAvenge = -1;
}

void
GameWatcher::createGameResult(QString logFileName)
{
	GameResult gameResult;

	if (playerID == 1) {
		gameResult.playerHero = hero1;
		gameResult.enemyHero  = hero2;
		gameResult.enemyName  = name2;
	} else if (playerID == 2) {
		gameResult.playerHero = hero2;
		gameResult.enemyHero  = hero1;
		gameResult.enemyName  = name1;
	} else {
		// emit pDebug("CreateGameResult: PlayerID wasn't defined in the
		// game.", 0, DebugLevel::Error);
		return;
	}

	gameResult.isFirst  = (firstPlayer == playerTag);
	gameResult.isWinner = (winnerPlayer == playerTag);

	emit newGameResult(gameResult, scr_state, logFileName, startGameEpoch);
}

QString
GameWatcher::createGameLog()
{
	if (!copyGameLogs) {
		// emit pDebug("Game log copy disabled.", 0);
		return "";
	}
	if (logSeekCreate == -1) {
		// emit pDebug("Cannot create match log. Not found CREATE_GAME",
		// 0);
		return "";
	}

	if (logSeekWon == -1) {
		// emit pDebug("Cannot create match log. Not found WON ", 0);
		return "";
	}

	QString timeStamp =
	    QDateTime::currentDateTime().toString("MMMM-d hh-mm");
	QString win	 = (winnerPlayer == playerTag) ? "WIN" : "LOSE";
	QString coin	 = (firstPlayer == playerTag) ? "FIRST" : "COIN";
	QString gameMode = conv_screen_state_string(scr_state);
	QString playerHero, enemyHero;
	if (playerID == 1) {
		playerHero = conv_string_number_class(qPrintable(hero1));
		enemyHero  = conv_string_number_class(qPrintable(hero2));
	} else {
		playerHero = conv_string_number_class(qPrintable(hero2));
		enemyHero  = conv_string_number_class(qPrintable(hero1));
	}
	QString fileName = gameMode + " " + timeStamp + " " + playerHero +
			   "vs" + enemyHero + " " + win + " " + coin +
			   ".arenatracker";

	// emit pDebug("Game log ready to be copied.", 0);
	emit gameLogComplete(logSeekCreate, logSeekWon, fileName);
	logSeekCreate = -1;
	logSeekWon    = -1;

	return fileName;
}

bool
GameWatcher::advanceTurn(bool playerDraw)
{
	if (turnReal == turn)
		return false;

	// Al jugar contra la maquina puede que se lea antes el fin de turno que
	// el robo del inicio del turno if(turn > turnReal+1)   turn =
	// turnReal+1;

	bool playerTurn;
	if ((firstPlayer == playerTag && turn % 2 == 1) ||
	    (firstPlayer != playerTag && turn % 2 == 0))
		playerTurn = true;
	else
		playerTurn = false;

	if (firstPlayer.isEmpty() || playerTag.isEmpty())
		playerTurn = playerDraw;

	// Al turno 1 dejamos que pase cualquiera asi dejamos el turno 0 para
	// indicar cartas de mulligan Solo avanza de turno al robar carta el
	// jugador que le corresponde
	bool advance = (turn == 1 || playerDraw == playerTurn || playerID == 0);
	if (advance) {
		turnReal = turn;
		// emit pDebug("\nTurn: " + QString::number(turn) + " " +
		// (playerTurn ? "Player" : "Enemy"), 0);

		isPlayerTurn = playerTurn;

		if (playerDraw)
			emit clearDrawList();

		emit specialCardTrigger(
		    "",
		    "",
		    -1,
		    -1);    // Evita Cartas createdBy en las cartas recien
			    // robadas al empezar el turno

		// Secret CSpirit test
		if (!isPlayerTurn && enemyMinions > 0) {
			// emit pDebug("CSpirit tested. Minions: " +
			// QString::number(enemyMinions), 0);
			emit cSpiritTested();
		}

		enemyMinionsDeadThisTurn = 0;	 // Hand of salvation testing
		playerCardsPlayedThisTurn =
		    0;	  // Rat trap/Hidden wisdom testing
	}
	return advance;
}

screen_state
GameWatcher::getLoadingScreen()
{
	return this->scr_state;
}

void
GameWatcher::setCopyGameLogs(bool value)
{
	this->copyGameLogs = value;
}

bool
GameWatcher::isCheatingCard(const QString& code)
{
	if (code == PRINCE_MALCHEZAAR)
		return true;
	else if (code == BODY_OF_CTHUN)
		return true;
	else if (code == EYE_OF_CTHUN)
		return true;
	else if (code == HEART_OF_CTHUN)
		return true;
	else if (code == MAW_OF_CTHUN)
		return true;
	return false;
}
}    // namespace mtk