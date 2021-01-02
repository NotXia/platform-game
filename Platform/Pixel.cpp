#include "Pixel.hpp"

Pixel::Pixel(char value, int color_foreground, int color_background, bool solid) {
	this->value = value;
	this->color_fg = color_foreground;
	this->color_bg = color_background;
	this->solid = solid;
}


char Pixel::getValue() {
	return value;
}

int Pixel::getColor() {
	return color_fg | color_bg;
}

bool Pixel::isSolid() {
	return solid;
}

void Pixel::setBackgroundColor(int color) {
	this->color_bg = color;
}

int Pixel::getBackgroundColor() {
	return color_bg;
}