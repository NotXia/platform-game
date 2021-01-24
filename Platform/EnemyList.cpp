#include "EnemyList.hpp"

EnemyList::EnemyList() {
	list = NULL;
	iter = NULL;
	prev = NULL;
}

/*
	Inizializza iter alla testa della lista e prev a NULL.
*/
void EnemyList::initIter() {
	iter = list;
	prev = NULL;
}

/*
	Restituisce l'oggetto Enemy del nodo attualmente puntato da iter
	Se iter è NULL, restituisce un oggetto di default.
*/
Enemy EnemyList::getCurrent() {
	if (iter != NULL) {
		return iter->enemy;
	}
	else {
		return Enemy();
	}
}

/*
	Prende in input un oggetto Enemy.
	Aggiunge un nuovo nodo in testa a list.
*/
void EnemyList::add(Enemy enemy) {
	EnemyNode *new_node = new EnemyNode;
	new_node->enemy = enemy;
	new_node->next = list;
	list = new_node;
}

/*
	Restituisce true se iter è NULL, false altrimenti.
*/
bool EnemyList::isNull() {
	return (iter == NULL);
}

/*
	Sposta prev al nodo attualmente puntato da iter.
	Sposta iter al nodo successivo.
*/
void EnemyList::goNext() {
	prev = iter;
	iter = iter->next;
}

/*
	Prende in input una posizione.
	Indica se in quella posizione c'è un nemico.
	Iter punterà a quel nodo, se esiste.
	Prev punterà al nodo precedente, se esiste.
*/
bool EnemyList::pointAt(Position position) {
	initIter();
	bool found = false;

	while (iter != NULL && !found) {
		if (iter->enemy.getBodyPosition().equals(position) || iter->enemy.getHeadPosition().equals(position)) {
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
	Prende in input una posizione.
	Indica se in quella posizione c'è un nemico.
	Non modifica iter e prev.
*/
bool EnemyList::existsAt(Position position) {
	EnemyNode *iterator = list;
	bool found = false;

	while (iterator != NULL && !found) {
		if (iterator->enemy.getBodyPosition().equals(position) || iterator->enemy.getHeadPosition().equals(position)) {
			found = true;
		}
		else {
			iterator = iterator->next;
		}
	}

	return found;
}

/*
	Aggiorna l'oggetto Enemy del nodo attualmente puntato da iter.
	Se il nemico è morto, cancella il nodo attualmente puntato da iter e muove iter al nodo successivo.
*/
void EnemyList::updateCurrent(Enemy enemy) {
	if (iter != NULL) {
		if (!enemy.isDead()) {
			iter->enemy = enemy;
			prev = iter;
			iter = iter->next;
		}
		else {
			if (prev == NULL) {
				EnemyNode *to_del = iter;
				list = list->next;
				iter = list;
				delete to_del;
			}
			else {
				EnemyNode *to_del = iter;
				prev->next = iter->next;
				iter = iter->next;
				delete to_del;
			}
		}
	}
}

/*
	Restituisce il numero di nodi nella lista
*/
int EnemyList::size() {
	int i=0;
	EnemyNode *iterator = list;

	while (iterator != NULL) {
		i++;
		iterator = iterator->next;
	}

	return i;
}