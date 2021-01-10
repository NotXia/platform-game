#include "LavaList.hpp"
#include <iostream>

LavaList::LavaList() {
	list = NULL;
}

void LavaList::insert(Position position) {
	LavaNode *new_node = new LavaNode();
	new_node->position = position;
	new_node->next = list;
	list = new_node;
}

bool LavaList::existsAt(Position position) {
	LavaNode *iterator = list;
	bool found = false;

	while (iterator != NULL && !found) {
		if (iterator->position.equals(position)) {
			found = true;
		}

		iterator = iterator->next;
	}

	return found;
}