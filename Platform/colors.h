#pragma once
/*
	Doc: https://docs.microsoft.com/en-us/windows/console/console-screen-buffers?redirectedfrom=MSDN#_win32_character_attributes
	
	Esempio di utilizzo:
		Carattere rosso su sfondo nero = FG_RED | BG_BLACK
*/

const int FG_BLACK = 0x00;
const int FG_DARKBLUE = 0x01;
const int FG_DARKGREEN = 0x02;
const int FG_DARKCYAN = 0x03;
const int FG_DARKRED = 0x04;
const int FG_DARKPINK = 0x05;
const int FG_DARKYELLOW = 0x06;
const int FG_LIGHTGREY = 0x07;
const int FG_GREY = 0x08;
const int FG_BLUE = 0x09;
const int FG_GREEN = 0x0A;
const int FG_CYAN = 0x0B;
const int FG_RED = 0x0C;
const int FG_PINK = 0x0D;
const int FG_YELLOW = 0x0E;
const int FG_WHITE = 0x0F;

const int BG_BLACK = 0x00;
const int BG_DARKBLUE = 0x10;
const int BG_DARKGREEN = 0x20;
const int BG_DARKCYAN = 0x30;
const int BG_DARKRED = 0x40;
const int BG_DARKPINK = 0x50;
const int BG_DARKYELLOW = 0x60;
const int BG_LIGHTGREY = 0x70;
const int BG_GREY = 0x80;
const int BG_BLUE = 0x90;
const int BG_GREEN = 0xA0;
const int BG_CYAN = 0xB0;
const int BG_RED = 0xC0;
const int BG_PINK = 0xD0;
const int BG_YELLOW = 0xE0;
const int BG_WHITE = 0xF0;

//const int LIGHTGREY_BLACK = 0x07;
//const int RED_BLACK = 0x0C;

const int CONSOLE_DEFAULT = FG_LIGHTGREY | BG_BLACK;

const int HP_COLOR = FG_DARKRED | BG_BLACK;

const int BACKGROUND_DEFAULT = BG_DARKCYAN;

const int PLATFORM_COLOR = FG_DARKYELLOW | BACKGROUND_DEFAULT;
const int TERRAIN_COLOR = FG_DARKGREEN | BG_DARKGREEN;

const int PLAYER_HEAD_COLOR = FG_PINK | BACKGROUND_DEFAULT;
const int PLAYER_BODY_COLOR = FG_PINK | BACKGROUND_DEFAULT;

const int ENEMY_HEAD_COLOR = FG_RED | BACKGROUND_DEFAULT;
const int ENEMY_BODY_COLOR = FG_RED | BACKGROUND_DEFAULT;
