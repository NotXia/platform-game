#pragma once
#include "Pixel.hpp"
#include "Position.hpp"

class Block {
	protected:
		Pixel body;
		Position position;

	public:
		Block(Pixel body=Pixel(), Position position=Position());

		Pixel getBody();
		Position getBodyPosition();

};

