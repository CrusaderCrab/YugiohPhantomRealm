#pragma once
#include <string>

//general definitions
#define YUG_UNBIND 0 
#define YUG_NULL_ID 0
#define YUG_NO_CARD -88
#define YUG_LOWEST_CARD_NO 100
#define YUG_HIGHEST_CARD_NO 999
#define YUG_MAX_NO_OF_CARDS 900
#define YUG_NO_OF_FREE_DUELISTS 34
#define YUG_DATA_FILE_PATH "C:/Cplusplus/Yugioh/Game/"
#define YUG_PLANE_FILE_PATH "GameData/models/plane02.obj"
#define YUG_CONCAT_FILE_PATH(a) std::string(YUG_DATA_FILE_PATH, a)
#define YUG_PI 3.1415926f
#define YUG_HALF_PI 1.5707963f
#define YUG_SCREEN_RATIO 1.25f
#define DEFAULT_NEAR_PLANE 0.1f
#define DEFAULT_FAR_PLANE 15.0f

//time
#define YUG_BOARD_BEGIN_GAME_TIME 5.0f
#define YUG_WAIT_SMALL_WAIT 0.1f
#define YUG_WAIT_INPUT_WAIT 0.2f


//card defines
#define YUG_ATTACK_MODE 0
#define YUG_DEFENSE_MODE 1
#define YUG_FACE_UP 3
#define YUG_FACE_DOWN 4
#define YUG_MONSTER_CARD 0
#define YUG_MAGIC_CARD 1
#define YUG_TRAP_CARD 2
#define YUG_EQUIP_CARD 4
#define YUG_STRONG_AGAINST 8
#define YUG_WEAK_AGAINST 9
#define YUG_EQUAL_AGAINST 10

//field defines
#define YUG_FIELD_NORMAL 0
#define YUG_FIELD_YAMI 1
#define YUG_FIELD_UMI 2
#define YUG_FIELD_SOGEN 3
#define YUG_FIELD_FOREST 4
#define YUG_FIELD_DESERT 5
#define YUG_FIELD_MOUNTAIN 6

//fusion markings
#define YUG_MARKINGS_UP 1
#define YUG_MARKINGS_DOWN -1
#define YUG_MARKINGS_MIDDLE 0

//symbol definitions
#define YUG_MERCURY 0
#define YUG_SUN 1
#define YUG_MOON 2
#define YUG_VENUS 3

#define YUG_MARS 4
#define YUG_JUPITER 5
#define YUG_SATURN 6
#define YUG_URANUS 7
#define YUG_PLUTO 8
#define YUG_NEPTUNE 9
#define YUG_STAR 10
//monster type definitions
#define YUG_DRAGON 11
#define YUG_SPELLCASTER 12
#define YUG_ZOMBIE 13
#define YUG_WARRIOR 14
#define YUG_BEASTWARRIOR 15
#define YUG_BEAST 16
#define YUG_WINGEDBEAST 17
#define YUG_FIEND 18
#define YUG_FAIRY 19
#define YUG_INSECT 20
#define YUG_DINOSAUR 21
#define YUG_REPTILE 22
#define YUG_FISH 23
#define YUG_SEASERPENT 24
#define YUG_MACHINE 25
#define YUG_THUNDER 26
#define YUG_AQUA 27
#define YUG_PYRO 28
#define YUG_ROCK 29
#define YUG_PLANT 30
#define YUG_MAGIC_SYMBOL 31
#define YUG_EQUIP_SYMBOL 32
#define YUG_RITUAL_SYMBOL 33
#define YUG_TRAP_SYMBOL 34
//Element symbols
#define YUG_MAGIC_ELEMENT 35
	//#define YUG_EQUIP_ELEMENT 37
#define YUG_TRAP_ELEMENT 37
	//#define YUG_RITUAL_ELEMENT 39
#define YUG_FIRE_ELEMENT 39
#define YUG_WATER_ELEMENT 40
#define YUG_WIND_ELEMENT 41
#define YUG_EARTH_ELEMENT 42
#define YUG_LIGHT_ELEMENT 43
#define YUG_DARK_ELEMENT 44
//little fusion order symbols
#define YUG_FUSION_SYMBOL_1 45
#define YUG_FUSION_SYMBOL_2 46
#define YUG_FUSION_SYMBOL_3 47
#define YUG_FUSION_SYMBOL_4 48
#define YUG_FUSION_SYMBOL_5 49
//battle Numbers
#define YUG_BATTLE_1 52
#define YUG_BATTLE_2 53
#define YUG_BATTLE_3 54
#define YUG_BATTLE_4 55
#define YUG_BATTLE_5 56
#define YUG_BATTLE_6 57
#define YUG_BATTLE_7 58
#define YUG_BATTLE_8 59
#define YUG_BATTLE_9 60
#define YUG_BATTLE_0 61

//informal Types
#define YUG_INFORMAL_ANIMAL 31
#define YUG_INFORMAL_TURTLE 32
#define YUG_INFORMAL_SPELL 33
#define YUG_INFORMAL_EVIL 34
#define YUG_INFORMAL_ELF 35
#define YUG_INFORMAL_JAR 36
#define YUG_INFORMAL_FEMALE 10
#define YUG_INFORMAL_BIG 38
#define YUG_INFORMAL_XXXXXX 39
#define YUG_INFORMAL_WINGED 40
#define YUG_INFORMAL_CLAWED 41
#define YUG_INFORMAL_LIGHT 42
#define YUG_INFORMAL_MONSTER 43
#define YUG_INFORMAL_MAN 44
#define YUG_INFORMAL_SWORD 45
#define YUG_INFORMAL_SERPENT 46

#define YUG_INFORMAL_NONE 0

















