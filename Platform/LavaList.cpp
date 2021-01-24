#include "LavaList.hpp"
#include <iostream>

LavaList::LavaList() {
	list = NULL;
}

/*
	Prende in input un oggetto Position.
	Inserisce un nuovo nodo in testa a list.
*/
void LavaList::insert(Position position) {
	LavaNode *new_node = new LavaNode();
	new_node->position = position;
	new_node->next = list;
	list = new_node;
}

/*
	Prende in input un oggetto Position.
	Restituisce true se esiste un nodo con quella posizione, false altrimenti.
*/
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