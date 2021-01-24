#include "BonusList.hpp"
#include <iostream>

BonusList::BonusList() {
	list = NULL;
	prev = NULL;
	iter = NULL;
}

/*
	Prende in input un oggetto Bonus.
	Lo inserisce in testa alla lista list.
*/
void BonusList::insert(Bonus bonus) {
	BonusNode *new_node = new BonusNode;
	new_node->bonus = bonus;
	new_node->next = list;
	list = new_node;
}

/*
	Inizializza iter alla testa della lista e prev a NULL.
*/
void BonusList::initIter() {
	iter = list;
	prev = NULL;
}

/*
	Restituisce true se iter è NULL, false altrimenti
*/
bool BonusList::isNull() {
	return iter == NULL;
}

/*
	Restituisce l'oggetto Bonus contenuto nel nodo attualmente puntato da iter.
	Se iter è NULL, restituisce un oggetto di default.
*/
Bonus BonusList::getCurrent() {
	if (iter != NULL) {
		return iter->bonus;
	}
	else {
		return Bonus();
	}
}

/*
	Elimina il nodo attualmente puntato da iter e muove iter al nodo successivo.
*/
void BonusList::deleteCurrent() {
	if (iter != NULL) {
		if (prev == NULL) {
			BonusNode *to_del = list;
			list = list->next;
			iter = list;
			prev = NULL;
			delete to_del;
		}
		else {
			BonusNode *to_del = iter;
			prev->next = iter->next;
			iter = iter->next;
			delete to_del;
		}
	}
}

/*
	Muove iter e prev per puntare al nodo successivo.
*/
void BonusList::goNext() {
	if (iter != NULL) {
		prev = iter;
		iter = iter->next;
	}
}

/*
	Prende in input un oggetto Position.
	Restituisce true se esiste un nodo con un Bonus nella stessa posizione del parametro, false altrimenti.
	Muove iter e prev in modo tale da puntare a quel nodo, se esiste.
*/
bool BonusList::pointAt(Position position) {
	initIter();
	bool found = false;
	
	while (iter != NULL && !found) {
		if (iter->bonus.getBodyPosition().equals(position)) {
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
	Prende in input un oggetto Bonus.
	Aggiorna il contenuto del nodo attualmente puntato da iter.
*/
void BonusList::updateCurrent(Bonus bonus) {
	if (iter != NULL) {
		iter->bonus = bonus;
	}
}