#include "Block.hpp"

Block::Block(Pixel body, Position position) {
	this->body = body;
	this->position = position;
}

Pixel Block::getBody() {
	return body;
}

Position Block::getBodyPosition() {
	return position;
}

// Restituisce la posizione sotto position
Position Block::getBelowPosition() {
	Position below = position;
	below.setY(below.getY()+1);

	return below;
}