#include "Pixel.hpp"

Pixel::Pixel(char value, int color, bool solid) {
	this->value = value;
	this->color = color;
	this->solid = solid;
}

char Pixel::getValue() {
	return value;
}

int Pixel::getColor() {
	return color;
}

bool Pixel::isSolid() {
	return solid;
}