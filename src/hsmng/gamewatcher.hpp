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

#ifndef MT_HSMNG_GAMEWATCHER_H
#define MT_HSMNG_GAMEWATCHER_H

#include <QObject>
#include <QString>

#include <hsdata/card.hpp>
#include <hsdata/types.hpp>

namespace mtk
{
class GameResult
{
      public:
	bool isFirst, isWinner;
	QString playerHero, enemyHero, enemyName;
};

class ArenaRewards
{
      public:
	ArenaRewards()
	{
		gold	   = 0;
		dust	   = 0;
		packs	   = 0;
		plainCards = 0;
		goldCards  = 0;
	}
	int gold, dust, packs, plainCards, goldCards;
};

class ArenaResult
{
      public:
	QString playerHero;
	QList<GameResult> gameResultList;
	ArenaRewards arenaRewards;
};

class GameWatcher : public QObject
{
	Q_OBJECT
      public:
	GameWatcher(QObject* parent = nullptr);
	~GameWatcher();

      private:
	enum PowerState
	{
		noGame,
		heroType1State,
		heroPower1State,
		heroType2State,
		mulliganState,
		inGameState
	};
	enum ArenaState
	{
		noDeckRead,
		deckRead,
		readingDeck
	};

	// Variables
      private:
	QString playerTag;
	PowerState powerState;
	ArenaState arenaState;
	screen_state scr_state;
	QString hero1, hero2, name1, name2, firstPlayer, winnerPlayer;
	int playerID;
	card_class secretHero;
	int enemyMinions, enemyMinionsAliveForAvenge;	 // Avenge control
	int enemyMinionsDeadThisTurn;	 // Hand of Salvation control (exclusive
					 // arena paladin secret)
	int playerCardsPlayedThisTurn;	  // Rat trap/Hidden wisdom control
					  // (wichwood secrets)
	int playerMinions;
	bool isPlayerTurn;
	QRegularExpressionMatch* match;
	// TurnReal avanza a turn cuando robamos carta, nos aseguramos de que
	// animaciones atrasadas no aparezcan como parte del nuevo turno
	int turn, turnReal;
	bool mulliganEnemyDone, mulliganPlayerDone;
	qint64 logSeekCreate, logSeekWon;
	bool copyGameLogs;
	bool spectating, tied;

	int last_entity_id;
	int last_entity_player;

	qint64 startGameEpoch;

	// Metodos
      private:
	void createGameResult(QString logFileName);
	void processLoadingScreen(const QString& line, qint64 numline);
	void processAsset(const QString& line, qint64 numline);
	void processArena(const QString& line, qint64 numline);
	void process_power(const QString& line, qint64 numline, qint64 seek);
	void processPowerHero(const QString& line, qint64 numline);
	void processPowerMulligan(const QString& line, qint64 numline);
	void process_power_game(const QString& line, qint64 numline);
	void processZone(const QString& line, qint64 numline);
	bool advanceTurn(bool playerDraw);
	void startReadingDeck();
	void endReadingDeck();
	bool isHeroPower(QString code);
	QString createGameLog();
	bool isCheatingCard(const QString& code);

      public:
	void reset();
	screen_state getLoadingScreen();
	void setCopyGameLogs(bool value);

      signals:
	void player_action_played();
	void newGameResult(GameResult gameResult,
			   screen_state loadingScreen,
			   QString logFileName,
			   qint64 startGameEpoch);
	void newArena(QString hero);
	void continueDraft();
	void arenaChoosingHeroe();
	void inRewards();
	void newDeckCard(QString card);
	void player_name(int id, QString name);
	void start_game();
	void screen_mode_changed(screen_state prev, screen_state curr);
	void end_game(QString winner_name = "", bool is_tied = false);
	void enemyHero(QString hero);
	void enterArena();
	void leaveArena();
	void player_carddraw(QString code, int id);
	void created_card(int id, QString code);
	void updated_card(int player, int id, QString code);
	void show_entity(int id, int player, QString code);
	void update_entity(int id, QString zone, int player, QString code);
	void card_return_deck(QString code, int id = 0);
	void enemyKnownCardDraw(int id, QString code);
	void enemy_carddraw(int id,
			    int turn	 = 0,
			    bool special = false,
			    QString code = "");
	void playerCardToHand(int id, QString code, int turn);
	void card_played(int player,
			 int id,
			 QString code = "",
			 bool discard = false);
	void lastHandCardIsCoin();
	void enemySecretPlayed(int id,
			       card_class hero,
			       screen_state loadingScreenState);
	void playerSecretPlayed(int id, QString code);
	void enemySecretStolen(int id,
			       QString code,
			       screen_state loadingScreenState);
	void playerSecretStolen(int id, QString code);
	void enemySecretRevealed(int id, QString code);
	void playerSecretRevealed(int id, QString code);
	void playerSpellPlayed(QString code);
	void playerMinionPlayed(QString code, int id, int playerMinions);
	void playerMinionZonePlayAdd(QString code, int id, int pos);
	void enemyMinionZonePlayAdd(QString code, int id, int pos);
	void playerMinionZonePlayAddTriggered(QString code, int id, int pos);
	void enemyMinionZonePlayAddTriggered(QString code, int id, int pos);
	void playerMinionZonePlayRemove(int id);
	void enemyMinionZonePlayRemove(int id);
	void playerMinionZonePlaySteal(int id, int pos);
	void enemyMinionZonePlaySteal(int id, int pos);
	void playerHeroZonePlayAdd(QString code, int id);
	void enemyHeroZonePlayAdd(QString code, int id);
	void playerHeroPowerZonePlayAdd(QString code, int id);
	void enemyHeroPowerZonePlayAdd(QString code, int id);
	void playerWeaponZonePlayAdd(QString code, int id);
	void enemyWeaponZonePlayAdd(QString code, int id);
	void playerWeaponZonePlayRemove(int id);
	void enemyWeaponZonePlayRemove(int id);
	void playerMinionPosChange(int id, int pos);
	void enemyMinionPosChange(int id, int pos);
	void tag_change(int id,
			QString code,
			QString tag,
			QString value,
			QString zone,
			int player);
	void unknownTagChange(QString tag, QString value);
	void playerTagChange(QString tag, QString value);
	void enemyTagChange(QString tag, QString value);
	void playerCardCodeChange(int id, QString newCode);
	void minionCodeChange(bool friendly, int id, QString newCode);
	void playerMinionGraveyard(int id, QString code);
	void enemyMinionGraveyard(int id, QString code, bool isPlayerTurn);
	void playerWeaponGraveyard(int id, QString code);
	void enemyWeaponGraveyard(int id, QString code);
	void playerAttack(bool isHeroFrom,
			  bool isHeroTo,
			  int playerMinions,
			  int id1,
			  int id2);
	void zonePlayAttack(QString code, int id1, int id2);
	void playerSpellObjHeroPlayed();
	void playerSpellObjMinionPlayed();
	void playerBattlecryObjHeroPlayed();
	void playerCardObjPlayed(QString code, int id1, int id2);
	void enemyCardObjPlayed(QString code, int id1, int id2);
	void playerHeroPower();
	void avengeTested();
	void handOfSalvationTested();
	void _3CardsPlayedTested();
	void cSpiritTested();
	void clearDrawList();
	void new_turn(bool is_player_turn, int num);
	void logTurn();
	void needResetDeck();
	void heroDraftDeck(QString hero = "");
	void activeDraftDeck();
	void pickCard(QString code);
	void
	specialCardTrigger(QString code, QString blockType, int id1, int id2);
	void gameLogComplete(qint64 logSeekCreate,
			     qint64 logSeekWon,
			     QString fileName);
	void arenaDeckRead();
	void buffHandCard(int id);
	void coin_id(int id, QString code);
	void pLog(QString line);

      private slots:
	void checkAvenge();

      public slots:
	void process_logline(log_component component,
			     const QString& line,
			     qint64 numline,
			     qint64 seek);
	void setDeckRead(bool value = true);
};

}    // namespace mtk

#endif	  // GAMEWATCHER_H
