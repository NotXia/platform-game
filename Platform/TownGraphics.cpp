#include "TownGraphics.hpp"
#include "colors.h"
#include <cstdlib>

TownGraphics::TownGraphics(Pixel terrain[GAME_WIDTH][GAME_HEIGHT], int start_x, int start_y) {
	createHouse1(terrain, start_x, start_y);
	start_x += rand() % 10 + 2;
	createHouse1(terrain, start_x, start_y);
	start_x += rand() % 10 + 2;
	createHouse1(terrain, start_x, start_y);
	start_x += rand() % 10 + 2;
	createHouse1(terrain, start_x, start_y);
}

void TownGraphics::createHouse1(Pixel terrain[GAME_WIDTH][GAME_HEIGHT], int& start_x, int start_y) {
	int left_size = rand() % 4 + 1;
	int right_size = rand() % 4 + 1;
	int height = rand() % 3 + 5;
	int border_color = BG_BLACK;
	int color = BG_LIGHTGREY;
	int door_color = BG_DARKYELLOW;

	// Muro sinistro
	for (int j=0; j<height; j++) {
		terrain[start_x][start_y-j] = Pixel(' ', 0, border_color, false);
	}
	start_x++;

	// Prima colonna
	for (int j=0; j<height-1; j++) {
		terrain[start_x][start_y-j] = Pixel(' ', 0, color, false);
	}
	terrain[start_x][start_y-height+1] = Pixel(' ', 0, border_color, false);
	start_x++;

	// Larghezza a sinistra
	for (int i=0; i<left_size; i++) {
		for (int j=0; j<height-1; j++) {
			terrain[start_x][start_y-j] = Pixel(' ', 0, color, false);
		}
		//if (right_size > left_size) {
		//	terrain[start_x][start_y-2] = Pixel(' ', 0, border_color, false);
		//}
		terrain[start_x][start_y-height+1] = Pixel(' ', 0, border_color, false);
		start_x++;
	}

	// Colonna prima della porta
	for (int j=0; j<height-1; j++) {
		terrain[start_x][start_y-j] = Pixel(' ', 0, color, false);
	}
	terrain[start_x][start_y-height+1] = Pixel(' ', 0, border_color, false);
	start_x++;

	// Porta
	for (int j=0; j<2; j++) {
		terrain[start_x][start_y-j] = Pixel(' ', 0, door_color, false);
	}
	for (int j=0; j<height-3; j++) {
		terrain[start_x][start_y-2-j] = Pixel(' ', 0, color, false);
	}
	terrain[start_x][start_y-height+1] = Pixel(' ', 0, border_color, false);
	start_x++;

	for (int j=0; j<2; j++) {
		terrain[start_x][start_y-j] = Pixel(' ', 0, door_color, false);
	}
	for (int j=0; j<height-3; j++) {
		terrain[start_x][start_y-2-j] = Pixel(' ', 0, color, false);
	}
	terrain[start_x][start_y-height+1] = Pixel(' ', 0, border_color, false);
	start_x++;

	// Colonna dopo la porta
	for (int j=0; j<height-1; j++) {
		terrain[start_x][start_y-j] = Pixel(' ', 0, color, false);
	}
	terrain[start_x][start_y-height+1] = Pixel(' ', 0, border_color, false);
	start_x++;

	// Larghezza a destra
	for (int i=0; i<right_size; i++) {
		for (int j=0; j<height-1; j++) {
			terrain[start_x][start_y-j] = Pixel(' ', 0, color, false);
		}
		//if (left_size >= right_size) {
		//	terrain[start_x][start_y-2] = Pixel(' ', 0, border_color, false);
		//}
		terrain[start_x][start_y-height+1] = Pixel(' ', 0, border_color, false);
		start_x++;
	}

	// Ultima colonna
	for (int j=0; j<height-1; j++) {
		terrain[start_x][start_y-j] = Pixel(' ', 0, color, false);
	}
	terrain[start_x][start_y-height+1] = Pixel(' ', 0, border_color, false);
	start_x++;

	// Muro destro
	for (int j=0; j<height; j++) {
		terrain[start_x][start_y-j] = Pixel(' ', 0, border_color, false);
	}
	start_x++;
}
