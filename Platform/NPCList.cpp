#include "NPCList.hpp"
#include <iostream>

NPCList::NPCList() {
	list = NULL;
	iter = NULL;
	prev = NULL;
}

/*
	Prende in input un oggetto NPC.
	Inserisce un nodo in testa a list con tale oggetto.
*/
void NPCList::insert(NPC npc) {
	NPCNode *new_node = new NPCNode;
	new_node->npc = npc;
	new_node->next = list;
	list = new_node;
}

/*
	Inizializza iter e prev per puntare alla testa di list.
*/
void NPCList::initIter() {
	iter = list;
	prev = NULL;
}

/*
	Restituisce l'oggetto NPC contenuto nel nodo attualmente puntato da iter.
*/
NPC NPCList::getCurrent() {
	if (iter != NULL) {
		return iter->npc;
	}
	else {
		return NPC();
	}
}

/*
	Indica se iter è NULL.
*/
bool NPCList::isNull() {
	return iter == NULL;
}

/*
	Muove iter e prev al nodo successivo.
*/
void NPCList::goNext() {
	if (iter != NULL) {
		prev = iter;
		iter = iter->next;
	}
}

/*
	Prende in input un oggetto Position.
	Cerca se esiste un oggetto NPC in tale posizione.
	Al termine della ricerca iter punterà a tale nodo (se esiste) e prev a quello precedente (se esiste).
*/
bool NPCList::pointAt(Position position) {
	initIter();
	bool found = false;

	while (iter != NULL && !found) {
		if (iter->npc.getBodyPosition().equals(position) || iter->npc.getHeadPosition().equals(position)) {
			found = true;
		}
		else {
			prev = iter;
			iter = iter->next;
		}
	}

	return found;
}

/*
	Prende in input un oggetto Position.
	Restituisce true se le coordinate di Position sono contenute nel raggio d'azione di un NPC.
*/
bool NPCList::inPlayerRange(Position position) {
	initIter();
	bool found = false;

	while (iter != NULL && !found) {
		if (iter->npc.getBodyPosition().equals(position) || iter->npc.getHeadPosition().equals(position) ||
			iter->npc.getBodyFrontPosition().equals(position) || iter->npc.getBackPosition().equals(position)) {
			found = true;
		}
		else {
			prev = iter;
			iter = iter->next;
		}
	}

	return found;
}

/*
	Prende in input un oggetto NPC.
	Aggiorna l'attuale nodo puntato da iter.
*/
void NPCList::setCurrent(NPC npc) {
	if (iter != NULL) {
		iter->npc = npc;
	}
}
