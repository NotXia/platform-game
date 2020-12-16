#include "EnemyList.hpp"

EnemyList::EnemyList() {
	list = NULL;
	iter = NULL;
	prev = NULL;
}

/*
	Inizializza iter alla testa della lista
*/
void EnemyList::initIter() {
	iter = list;
}

/*
	Restituisce l'oggetto Enemy del nodo attualmente puntato da iter
	Se iter è NULL, restituisce un oggetto di default
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
	Muove l'iteratore al nodo successivo
*/
void EnemyList::goNext() {
	if (iter != NULL) {
		prev = iter;
		iter = iter->next;
	}
}

/*
	Elimina il nodo attualmente puntato da iter
*/
void EnemyList::deleteCurrent() {
	if (iter != NULL) {
		if (prev == NULL) {
			EnemyNode *to_del = list;
			list = list->next;
			delete to_del;
		}
		else {
			EnemyNode *to_del = iter;
			prev->next = iter->next;
			delete to_del;
		}
	}
}

/*
	Aggiunge un nuovo nodo in testa a list
*/
void EnemyList::add(Enemy enemy) {
	EnemyNode *new_node = new EnemyNode;
	new_node->enemy = enemy;
	new_node->next = list;
	list = new_node;
}

/*
	Restituisce true se iter è NULL, false altrimenti
*/
bool EnemyList::isNull() {
	return iter == NULL;
}
