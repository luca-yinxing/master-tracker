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

#ifndef HSDATA_CONV_H
#define HSDATA_CONV_H

#include "types.hpp"

#include <utils/utils.hpp>

#include <stdio.h>

namespace mtk
{
// clang-format off

extern const format_type conv_array_format[] = {
    INVALID_FORMAT,
    WILD,
    STANDARD,
    CLASSIC
};

extern const size_t conv_array_format_len = sizeof(conv_array_format) / sizeof(format_type);

extern const char* conv_array_format_string[] = {
    "",
    "Wild",
    "Standard",
    "Classic"
};

extern const card_zone conv_array_zone[] = {
    INVALID_ZONE,
    PLAY,
    DECK,
    HAND,
    GRAVEYARD,
    REMOVEDFROMGAME,
    SETASIDE,
    SECRET,
};

extern const size_t conv_array_zone_len = sizeof(conv_array_zone) / sizeof(card_zone);

extern const char* conv_array_zone_string[] = {
    "",
    "PLAY",
    "DECK",
    "HAND",
    "GRAVEYARD",
    "REMOVEDFROMGAME",
    "SETASIDE",
    "SECRET",
};

extern const language conv_array_language[] = {
    INVALID_LANGUAGE,
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

extern const size_t conv_array_language_len = sizeof(conv_array_language) / sizeof(language);

extern const char* conv_array_language_string[] {
    "",
    "deDE",
    "enUS",
    "esES",
    "esMX",
    "frFR",
    "itIT",
    "jaJP",
    "koKR",
    "plPL",
    "ptBR",
    "ruRU",
    "thTH",
    "zhCN",
    "zhTW"
};

extern const log_component conv_array_log_component[] = {
    INVALID_LOG,
    LOADINGSCREEN,
    ARENA,
    POWER,
    ZONE,
    ASSET
};

extern const size_t conv_array_log_component_len = sizeof(conv_array_log_component) / sizeof(log_component);

extern const char* conv_array_log_component_string[] = {
    "",
    "LoadingScreen",
    "Arena",
    "Power",
    "Zone",
    "Asset"
};

extern const screen_state conv_array_screen_state[] = {
    INVALID_STATE,
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

extern const size_t conv_array_screen_state_len = sizeof(conv_array_screen_state) / sizeof(screen_state);

extern const char* conv_array_screen_state_string[] = {
    "",
    "HUB",
    "DRAFT",
    "TOURNAMENT",
    "ADVENTURE",
    "TAVERN_BRAWL",
    "FRIENDLY",
    "GAMEPLAY",
    "GAME_MODE",
    "BACON",
    "PVP_DUNGEON_RUN",
    "PACKOPENING",
    "COLLECTIONMANAGER"
};

extern const card_rarity conv_array_rarity[] = {
    INVALID_RARITY,
    FREE,
    COMMON,
    RARE,
    EPIC,
    LEGENDARY
};

extern const size_t conv_array_rarity_len = sizeof(conv_array_rarity) / sizeof(card_rarity);

extern const char* conv_array_rarity_string[] = {
    "INVALID_RARIRY",
    "FREE",
    "COMMON",
    "RARE",
    "EPIC",
    "LEGENDARY"
};

extern const unsigned char conv_array_rarity_color[conv_array_rarity_len][3] = {
    { 255, 255, 255 },
    { 255, 255, 255 },
    { 255, 255, 255 },
    { 0, 112, 221 },
    { 163, 53, 238 },
    { 255, 128, 0 }
};

extern const card_type conv_array_type[] = {
    INVALID_TYPE,
    HERO,
    MINION,
    SPELL,
    ENCHANTMENT,
    WEAPON,
    HERO_POWER
};

extern const size_t conv_array_type_len = sizeof(conv_array_type) / sizeof(card_type);

extern const char* conv_array_type_string[] = {
    "INVALID_TYPE",
    "HERO",
    "MINION",
    "SPELL",
    "ENCHANTMENT",
    "WEAPON",
    "HERO_POWER"
};

extern const card_race conv_array_race[] = {
    INVALID_RACE,
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

extern const size_t conv_array_race_len = sizeof(conv_array_race) / sizeof(card_race);

extern const char* conv_array_race_string[] = {
    "INVALID_RACE",
    "BLOODELF",
    "DRAENEI",
    "DWARF",
    "GNOME",
    "GOBLIN",
    "HUMAN",
    "NIGHTELF",
    "ORC",
    "TAUREN",
    "TROLL",
    "UNDEAD",
    "WORGEN",
    "GOBLIN2",
    "MURLOC",
    "DEMON",
    "SCOURGE",
    "MECHANICAL",
    "ELEMENTAL",
    "OGRE",
    "BEAST",
    "TOTEM",
    "NERUBIAN",
    "PIRATE",
    "DRAGON",
    "BLANK",
    "ALL",
    "EGG",
    "QUILBOAR"
};

extern const char* conv_array_race_string_en[] = {
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "Murloc",
    "Demon",
    "",
    "Mech",
    "Elemental",
    "",
    "Beast",
    "Totem",
    "",
    "Pirate",
    "Dragon",
    "",
    "All",
    "",
    "Quilboar"
};

extern const card_class conv_array_class[] = {
    INVALID_CLASS,
    DEMONHUNTER,
    DRUID,
    HUNTER,
    MAGE,
    PALADIN,
    PRIEST,
    ROGUE,
    SHAMAN,
    WARLOCK,
    WARRIOR,
    NEUTRAL
};

extern const size_t conv_array_class_len = sizeof(conv_array_class) / sizeof(card_class);

extern const char* conv_array_class_string[] = {
    "INVALID_CLASS",
    "DEMONHUNTER",
    "DRUID",
    "HUNTER",
    "MAGE",
    "PALADIN",
    "PRIEST",
    "ROGUE",
    "SHAMAN",
    "WARLOCK",
    "WARRIOR",
    "NEUTRAL"
};

extern const char* conv_array_class_string_en[] = {
    "",
    "Demon Hunter",
    "Druid",
    "Hunter",
    "Mage",
    "Paladin",
    "Priest",
    "Rogue",
    "Shaman",
    "Warlock",
    "Warrior",
    "Neutral"
};

extern const unsigned char conv_array_class_color[conv_array_class_len][3] = {
    { 60, 72, 84 },
    { 30, 91, 61 },
    { 119, 70, 37 },
    { 46, 117, 39 },
    { 28, 82, 148 },
    { 133, 88, 35 },
    { 117, 117, 117 },
    { 40, 44, 48 },
    { 39, 51, 119 },
    { 92, 55, 103 },
    { 130, 38, 26 },
    { 60, 72, 84 }
};

extern const char* conv_array_class_string_number[] = {
    "",
    "10",
    "06",
    "05",
    "08",
    "04",
    "09",
    "03",
    "02",
    "07",
    "01",
    ""
};

extern const card_set conv_array_set[] = {
    INVALID_SET,
    TEST_TEMPORARY,
    CORE,
    EXPERT1,
    HOF,
    MISSIONS,
    DEMO,
    NONE,
    CHEAT,
    CS_BLANK,
    DEBUG_SP,
    PROMO,
    NAXX,
    GVG,
    BRM,
    TGT,
    CREDITS,
    HERO_SKINS,
    TB,
    SLUSH,
    LOE,
    OG,
    OG_RESERVE,
    KARA,
    KARA_RESERVE,
    GANGS,
    GANGS_RESERVE,
    UNGORO,
    ICECROWN,
    LOOTAPALOOZA,
    GILNEAS,
    BOOMSDAY,
    CS_TROLL,
    DALARAN,
    ULDUM,
    DRAGONS,
    YEAR_OF_THE_DRAGON,
    BLACK_TEMPLE,
    WILD_EVENT,
    SCHOLOMANCE,
    BATTLEGROUNDS,
    DEMON_HUNTER_INITIATE,
    DARKMOON_FAIRE,
    THE_BARRENS,
};

extern const size_t conv_array_set_len = sizeof(conv_array_set) / sizeof(card_set);

extern const char* conv_array_set_string[] = {
    "INVALID_SET",
    "TEST_TEMPORARY",
    "CORE",
    "EXPERT1",
    "HOF",
    "MISSIONS",
    "DEMO",
    "NONE",
    "CHEAT",
    "CS_BLANK",
    "DEBUG_SP",
    "PROMO",
    "NAXX",
    "GVG",
    "BRM",
    "TGT",
    "CREDITS",
    "HERO_SKINS",
    "TB",
    "SLUSH",
    "LOE",
    "OG",
    "OG_RESERVE",
    "KARA",
    "KARA_RESERVE",
    "GANGS",
    "GANGS_RESERVE",
    "UNGORO",
    "ICECROWN",
    "LOOTAPALOOZA",
    "GILNEAS",
    "BOOMSDAY",
    "CS_TROLL",
    "DALARAN",
    "ULDUM",
    "DRAGONS",
    "YEAR_OF_THE_DRAGON",
    "BLACK_TEMPLE",
    "WILD_EVENT",
    "SCHOLOMANCE",
    "BATTLEGROUNDS",
    "DEMON_HUNTER_INITIATE",
    "DARKMOON_FAIRE",
    "THE_BARRENS",
};

extern const card_mechanic conv_array_mechanic[] = {
    INVALID_MECHANIC,
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
    M_FRENZY
};

extern const size_t conv_array_mechanic_len = sizeof(conv_array_mechanic) / sizeof(card_mechanic);

extern const char* conv_array_mechanic_string[] = {
    "INVALID_MECHANIC",
    "BATTLECRY",
    "RUSH",
    "DISCOVER",
    "AURA",
    "TAG_ONE_TURN_EFFECT",
    "TOPDECK",
    "SPELLPOWER",
    "SPELLBURST",
    "DEATHRATTLE",
    "TAUNT",
    "COMBO",
    "ENCHANTMENT_INVISIBLE",
    "OUTCAST",
    "STEALTH",
    "OVERLOAD",
    "DIVINE_SHIELD",
    "LIFESTEAL",
    "CHARGE",
    "CHOOSE_ONE",
    "SECRET",
    "FREEZE",
    "WINDFURY",
    "CANT_ATTACK",
    "CANT_BE_SILENCED",
    "UNTOUCHABLE",
    "POISONOUS",
    "QUEST",
    "INSPIRE",
    "CANT_BE_DESTROYED",
    "AFFECTED_BY_SPELL_POWER",
    "TWINSPELL",
    "ENRAGED",
    "MODULAR",
    "OVERKILL",
    "MULTIPLY_BUFF_VALUE",
    "ImmuneToSpellpower",
    "TRIGGER_VISUAL",
    "AI_MUST_PLAY",
    "RECEIVES_DOUBLE_SPELLDAMAGE_BONUS",
    "InvisibleDeathrattle",
    "FORGETFUL",
    "REBORN",
    "CANT_BE_TARGETED_BY_SPELLS",
    "CANT_BE_TARGETED_BY_HERO_POWERS",
    "GEARS",
    "FINISH_ATTACK_SPELL_ON_DAMAGE",
    "ECHO",
    "PUZZLE",
    "APPEAR_FUNCTIONALLY_DEAD",
    "EVIL_GLOW",
    "ADJACENT_BUFF",
    "START_OF_GAME",
    "GRIMY_GOONS",
    "KABAL",
    "JADE_GOLEM",
    "JADE_LOTUS",
    "MORPH",
    "DUNGEON_PASSIVE_BUFF",
    "CORRUPT",
    "HEROPOWER_DAMAGE",
    "SIDEQUEST",
    "IGNORE_HIDE_STATS_FOR_BIG_CARD",
    "SILENCE",
    "SUMMONED",
    "RITUAL",
    "GHOSTLY",
    "COLLECTIONMANAGER_FILTER_MANA_EVEN",
    "COLLECTIONMANAGER_FILTER_MANA_ODD",
    "DEATH_KNIGHT",
    "IMMUNE",
    "CANT_BE_FATIGUED",
    "SPARE_PART",
    "AUTOATTACK",
    "FRENZY"
};

extern const card_spellschool conv_array_spellschool[] = {
    INVALID_SPELLSCHOOL,
    NATURE,
    FROST,
    FIRE,
    ARCANE,
    FEL,
    SHADOW,
    HOLY,
};

extern const size_t conv_array_spellschool_len = sizeof(conv_array_spellschool) / sizeof(card_spellschool);

extern const char* conv_array_spellschool_string[] = {
    "INVALID_SPELLSCHOOL",
    "NATURE",
    "FROST",
    "FIRE",
    "ARCANE",
    "FEL",
    "SHADOW",
    "HOLY",
};

extern const char* conv_array_spellschool_string_en[] = {
    "",
    "Nature",
    "Frost",
    "Fire",
    "Arcane",
    "Fel",
    "Shadow",
    "Holy",
};

extern const card_faction conv_array_faction[] = {
    INVALID_FACTION,
    HORDE,
    ALLIANCE
};

extern const size_t conv_array_faction_len = sizeof(conv_array_faction) / sizeof(card_faction);

extern const char* conv_array_faction_string[] = {
    "",
    "HORDE",
    "ALLIANCE"
};

extern const card_tag conv_array_tag[] = {
    INVALID_TAG,
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

extern const size_t conv_array_tag_len = sizeof(conv_array_tag) / sizeof(card_tag);

extern const char* conv_array_tag_string[] = {
    "",
    "BATTLECRY",
    "OVERLOAD",
    "OVERKILL",
    "POISONOUS",
    "RUSH",
    "TAUNT",
    "IMMUNE",
    "DEATHRATTLE",
    "DIVINE_SHIELD",
    "SECRET",
    "QUEST",
    "WINDFURY",
    "REBORN",
    "COMBO",
    "DISCOVER",
    "SPELLPOWER",
    "CHARGE",
    "STEALTH",
    "SILENCE",
    "FRENZY",
    "FREEZE",
    "LIFESTEAL",
    "ADAPT",
    "MODULAR",
    "SPARE_PART",
    "JADE_GOLEM",
    "CHOOSE_ONE",
    "COUNTER",
    "OUTCAST",
    "START_OF_GAME",
    "AUTOATTACK",
    "RECRUIT",
    "ECHO",
    "AI_MUST_PLAY",
    "CANT_ATTACK",
    "CORRUPT",
    "SPELLBURST",
    "ENRAGED"
};

// clang-format on
}    // namespace mtk

#endif