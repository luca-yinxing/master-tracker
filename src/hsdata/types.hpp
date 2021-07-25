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

#ifndef HSDATA_TYPES_H
#define HSDATA_TYPES_H

namespace mtk
{
enum player_side
{
	INVALID_SIDE = 0,
	FRIENDLY     = 1,
	OPPOSING     = 2
};

enum game_result
{
	TIED,
	LOST,
	WIN,
};

enum card_field
{
	INVALID_FIELD = 0x0,
	ID	      = 1 << 0,
	CODE	      = 1 << 1,
	NAME	      = 1 << 2,
	RARITY	      = 1 << 3,
	TYPE	      = 1 << 4,
	CLASSES	      = 1 << 5,
	MECHANIC      = 1 << 6,
	RACE	      = 1 << 7,
	SET	      = 1 << 8,
	SPELLSCHOOL   = 1 << 9,
	FACTION	      = 1 << 10,
	COST	      = 1 << 11,
	COLLECTIBLE   = 1 << 12,
	ELITE	      = 1 << 13,
	TEXT	      = 1 << 14,
	ARTIST	      = 1 << 15,
	FLAVOR	      = 1 << 16,
	ATTACK	      = 1 << 17,
	HEALTH	      = 1 << 18,
	OVERLOAD      = 1 << 19,
	DURABILITY    = 1 << 20,
	SPELLDAMAGE   = 1 << 21,
	ARMOR	      = 1 << 22,
	TAG	      = 1 << 23,
	QUESTREWARD   = 1 << 24
};

enum card_zone
{
	INVALID_ZONE	= 0,
	PLAY		= 1,
	DECK		= 2,
	HAND		= 3,
	GRAVEYARD	= 4,
	REMOVEDFROMGAME = 5,
	SETASIDE	= 6,
	SECRET		= 7,
};

enum league
{
	INVALID_LEAGUE = 0,
	DIAMOND	       = 1,
	PLATINUM       = 2,
	GOLD	       = 3,
	SILVER	       = 4,
	BRONZE	       = 5,
};

enum language
{
	INVALID_LANGUAGE = 0,
	DE_DE,
	EN_US,
	ES_ES,
	ES_MX,
	FR_FR,
	IT_IT,
	JA_JP,
	KO_KR,
	PL_PL,
	PT_BR,
	RU_RU,
	TH_TH,
	ZH_CN,
	ZH_TW
};

enum log_component
{
	INVALID_LOG = 0,
	LOADINGSCREEN,
	ARENA,
	POWER,
	ZONE,
	ASSET
};

enum screen_state
{
	INVALID_STATE = 0,
	SS_MENU,
	SS_ARENA,
	SS_RANKED,
	SS_ADVENTURE,
	SS_BRAWL,
	SS_FRIENDLY,
	SS_GAMEPLAY,
	SS_GAMEMODE,
	SS_BATTLEGROUND,
	SS_DUELS,
	SS_PACKOPENING,
	SS_COLLECTIONMANAGER
};

enum format_type
{
	INVALID_FORMAT = 0,
	WILD	       = 1,
	STANDARD       = 2,
	CLASSIC	       = 3
};

enum card_rarity
{
	INVALID_RARITY = 0,
	FREE,
	COMMON,
	RARE,
	EPIC,
	LEGENDARY
};
enum card_type
{
	INVALID_TYPE = 0,
	HERO,
	MINION,
	SPELL,
	ENCHANTMENT,
	WEAPON,
	HERO_POWER
};

enum card_faction
{
	INVALID_FACTION = 0,
	HORDE,
	ALLIANCE
};

enum card_race
{
	INVALID_RACE = 0,
	BLOODELF,
	DRAENEI,
	DWARF,
	GNOME,
	GOBLIN,
	HUMAN,
	NIGHTELF,
	ORC,
	TAUREN,
	TROLL,
	UNDEAD,
	WORGEN,
	GOBLIN2,
	MURLOC,
	DEMON,
	SCOURGE,
	MECHANICAL,
	ELEMENTAL,
	OGRE,
	BEAST,
	TOTEM,
	NERUBIAN,
	PIRATE,
	DRAGON,
	BLANK,
	ALL,
	EGG,
	QUILBOAR
};

enum card_class
{
	INVALID_CLASS = 0,
	DEMONHUNTER   = 10,
	DRUID	      = 6,
	HUNTER	      = 5,
	MAGE	      = 8,
	PALADIN	      = 4,
	PRIEST	      = 9,
	ROGUE	      = 3,
	SHAMAN	      = 2,
	WARLOCK	      = 7,
	WARRIOR	      = 1,
	NEUTRAL	      = 11
};

enum card_set
{
	INVALID_SET	      = 0,
	TEST_TEMPORARY	      = 1,
	CORE		      = 2,
	EXPERT1		      = 3,
	HOF		      = 4,
	MISSIONS	      = 5,
	DEMO		      = 6,
	NONE		      = 7,
	CHEAT		      = 8,
	CS_BLANK	      = 9,
	DEBUG_SP	      = 10,
	PROMO		      = 11,
	NAXX		      = 12,
	GVG		      = 13,
	BRM		      = 14,
	TGT		      = 15,
	CREDITS		      = 16,
	HERO_SKINS	      = 17,
	TB		      = 18,
	SLUSH		      = 19,
	LOE		      = 20,
	OG		      = 21,
	OG_RESERVE	      = 22,
	KARA		      = 23,
	KARA_RESERVE	      = 24,
	GANGS		      = 25,
	GANGS_RESERVE	      = 26,
	UNGORO		      = 27,
	ICECROWN	      = 1001,
	LOOTAPALOOZA	      = 1004,
	GILNEAS		      = 1125,
	BOOMSDAY	      = 1127,
	CS_TROLL	      = 1129,
	DALARAN		      = 1130,
	ULDUM		      = 1158,
	DRAGONS		      = 1347,
	YEAR_OF_THE_DRAGON    = 1403,
	BLACK_TEMPLE	      = 1414,
	WILD_EVENT	      = 1439,
	SCHOLOMANCE	      = 1443,
	BATTLEGROUNDS	      = 1453,
	DEMON_HUNTER_INITIATE = 1463,
	DARKMOON_FAIRE	      = 1466,
	THE_BARRENS	      = 1525,
};

enum card_mechanic
{
	INVALID_MECHANIC = 0,
	M_BATTLECRY,
	M_RUSH,
	M_DISCOVER,
	M_AURA,
	M_TAG_ONE_TURN_EFFECT,
	M_TOPDECK,
	M_SPELLPOWER,
	M_SPELLBURST,
	M_DEATHRATTLE,
	M_TAUNT,
	M_COMBO,
	M_ENCHANTMENT_INVISIBLE,
	M_OUTCAST,
	M_STEALTH,
	M_OVERLOAD,
	M_DIVINE_SHIELD,
	M_LIFESTEAL,
	M_CHARGE,
	M_CHOOSE_ONE,
	M_SECRET,
	M_FREEZE,
	M_WINDFURY,
	M_CANT_ATTACK,
	M_CANT_BE_SILENCED,
	M_UNTOUCHABLE,
	M_POISONOUS,
	M_QUEST,
	M_INSPIRE,
	M_CANT_BE_DESTROYED,
	M_AFFECTED_BY_SPELL_POWER,
	M_TWINSPELL,
	M_ENRAGED,
	M_MODULAR,
	M_OVERKILL,
	M_MULTIPLY_BUFF_VALUE,
	M_ImmuneToSpellpower,
	M_TRIGGER_VISUAL,
	M_AI_MUST_PLAY,
	M_RECEIVES_DOUBLE_SPELLDAMAGE_BONUS,
	M_InvisibleDeathrattle,
	M_FORGETFUL,
	M_REBORN,
	M_CANT_BE_TARGETED_BY_SPELLS,
	M_CANT_BE_TARGETED_BY_HERO_POWERS,
	M_GEARS,
	M_FINISH_ATTACK_SPELL_ON_DAMAGE,
	M_ECHO,
	M_PUZZLE,
	M_APPEAR_FUNCTIONALLY_DEAD,
	M_EVIL_GLOW,
	M_ADJACENT_BUFF,
	M_START_OF_GAME,
	M_GRIMY_GOONS,
	M_KABAL,
	M_JADE_GOLEM,
	M_JADE_LOTUS,
	M_MORPH,
	M_DUNGEON_PASSIVE_BUFF,
	M_CORRUPT,
	M_HEROPOWER_DAMAGE,
	M_SIDEQUEST,
	M_IGNORE_HIDE_STATS_FOR_BIG_CARD,
	M_SILENCE,
	M_SUMMONED,
	M_RITUAL,
	M_GHOSTLY,
	M_COLLECTIONMANAGER_FILTER_MANA_EVEN,
	M_COLLECTIONMANAGER_FILTER_MANA_ODD,
	M_DEATH_KNIGHT,
	M_IMMUNE,
	M_CANT_BE_FATIGUED,
	M_SPARE_PART,
	M_AUTOATTACK,
	M_FRENZY,
};

enum card_spellschool
{
	INVALID_SPELLSCHOOL = 0,
	NATURE,
	FROST,
	FIRE,
	ARCANE,
	FEL,
	SHADOW,
	HOLY,
};

enum card_tag
{
	INVALID_TAG = 0,
	T_BATTLECRY,
	T_OVERLOAD,
	T_OVERKILL,
	T_POISONOUS,
	T_RUSH,
	T_TAUNT,
	T_IMMUNE,
	T_DEATHRATTLE,
	T_DIVINE_SHIELD,
	T_SECRET,
	T_QUEST,
	T_WINDFURY,
	T_REBORN,
	T_COMBO,
	T_DISCOVER,
	T_SPELLPOWER,
	T_CHARGE,
	T_STEALTH,
	T_SILENCE,
	T_FRENZY,
	T_FREEZE,
	T_LIFESTEAL,
	T_ADAPT,
	T_MODULAR,
	T_SPARE_PART,
	T_JADE_GOLEM,
	T_CHOOSE_ONE,
	T_COUNTER,
	T_OUTCAST,
	T_START_OF_GAME,
	T_AUTOATTACK,
	T_RECRUIT,
	T_ECHO,
	T_AI_MUST_PLAY,
	T_CANT_ATTACK,
	T_CORRUPT,
	T_SPELLBURST,
	T_ENRAGED
};

}    // namespace mtk

#endif