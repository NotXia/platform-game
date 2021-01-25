#pragma once
#include "colors.h"

const int STRING_LEN = 1000;


/*************************
	DIMENSIONE ELEMENTI
*************************/
const int GAME_WIDTH = 100;
const int GAME_HEIGHT = 15;

const int TERRAIN_HEIGHT = 3;
const int PLATFORM_MAX_LENGTH = 25;
const int PLATFORM_MIN_LENGTH = 5;
const int EMPTYZONE_LENGTH = 5;
const int PLATFORM_CHANCE = 20;


/**************
	GAMEPLAY
**************/
const int MAX_LIFE = 10;

const int JUMP_HEIGHT = 4;
const int JUMP_SPEED = 2500;
const int FALL_SPEED = 1800;

const int WEAPON_DISPLAY_TIME = 4000;

const int BULLET_SPEED = 900;

const int ENEMY_REFRESH_RATE = 8000;
const int ENEMY_RANGE = 18;
const int ENEMY_BASE_POINTS = 10; 
const int BONUS_BASE_POINTS = 10;

const int DIFFICULTY_INCREASE_RATE = 5;

const int BOSS_BASE_POINTS = 100;
const int BOSS_BASE_MONEY = 10;

const int MAP_EVENT_SPEED = 10000;
const int LAVA_DAMAGE = 1;


/*************
	GRAFICA
*************/
const int BACKGROUND_DEFAULT = BG_DARKCYAN;

const char PLAYER_HEAD_LEFT = char(17);
const char PLAYER_HEAD_RIGHT = char(16);
const char PLAYER_BODY = char(219);
const int PLAYER_HEAD_COLOR_FG = FG_PINK;
const int PLAYER_BODY_COLOR_FG = FG_PINK;

const char MONEY_SYMBOL = char(207);
const int MONEY_COLOR = FG_DARKYELLOW;

const char HP_SYMBOL = char(3);
const int HP_COLOR = FG_DARKRED;

const char PLATFORM_SYMBOL = char(219);
const int PLATFORM_COLOR_FG = FG_DARKYELLOW;
const int PLATFORM_COLOR_BG = BACKGROUND_DEFAULT;

const char GRASS_SYMBOL = char(177);
const int GRASS_COLOR_FG = FG_GREEN;
const int GRASS_COLOR_BG = BG_DARKGREEN;

const char ROCK_SYMBOL = char(177);
const int ROCK_COLOR_FG = FG_GREY;
const int ROCK_COLOR_BG = BG_BLACK;

const char SKY_SYMBOL = ' ';
const int SKY_COLOR_FG = 0;
const int SKY_COLOR_BG = BACKGROUND_DEFAULT;

const char WALL_SYMBOL = char(177);
const int WALL_COLOR_FG = FG_DARKRED;
const int WALL_COLOR_BG = BG_BLACK;

const char LAVA_SYMBOL = char(176);
const int LAVA_COLOR_FG = FG_DARKYELLOW;
const int LAVA_COLOR_BG = BG_DARKRED;

const char ENEMY_HEAD_LEFT = '<';
const char ENEMY_HEAD_RIGHT = '>';
const char ENEMY_BODY = char(219);
const int ENEMY_HEAD_COLOR_FG = FG_RED;
const int ENEMY_BODY_COLOR_FG = FG_RED;
const int ANGRY_ENEMY_HEAD_COLOR_FG = FG_DARKRED;
const int ANGRY_ENEMY_BODY_COLOR_FG = FG_DARKRED;
const int STUNNED_BOSS_HEAD_COLOR_FG = FG_GREY;
const int STUNNED_BOSS_BODY_COLOR_FG = FG_GREY;

const int NPC_HEAD_COLOR_FG = FG_DARKBLUE;
const int NPC_BODY_COLOR_FG = BG_DARKBLUE;

