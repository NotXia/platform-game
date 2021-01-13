#include "BulletList.hpp"
#include <iostream>

BulletList::BulletList() {
	list = NULL; 
	prev = NULL;
	iter = NULL;
}

/*
	Prende in input un oggetto di tipo Bullet.
	Inserisce l'input in testa alla lista list
*/
void BulletList::insert(Bullet bullet) {
	FlyingBullet *new_node = new FlyingBullet;
	new_node->bullet = bullet;
	new_node->next = list;
	list = new_node;
}

/*
	Inizializza iter alla testa della lista e prev a NULL
*/
void BulletList::initIter() {
	iter = list;
	prev = NULL;
}

/*
	Restituisce true se iter è NULL, false altrimenti
*/
bool BulletList::isNull() {
	return iter == NULL;
}

/*
	Restituisce l'oggetto Bullet del nodo attualmente puntato da iter. Se è NULL, restituisce l'oggetto di default
*/
Bullet BulletList::getCurrent() {
	if (iter != NULL) {
		return iter->bullet;
	}
	else {
		return Bullet();
	}
}

/*
	Aggiorna l'oggetto Bullet del nodo attualmente puntato da iter
*/
void BulletList::updateCurrent(Bullet bullet) {
	if (iter != NULL) {
		if (bullet.canTravel()) {
			iter->bullet = bullet;
			prev = iter;
			iter = iter->next;
		}
		else {
			if (prev == NULL) {
				FlyingBullet *to_del = list;
				list = list->next;
				iter = list;
				delete to_del;
			}
			else {
				FlyingBullet *to_del = iter;
				prev->next = iter->next;
				iter = iter->next;
				delete to_del;
			}
		}
	}
}

/*
	Prende in input due oggetti Position.
	Restituisce true se è presente un Bullet in una delle due posizioni. False altrimenti.
	Iter punterà a quel nodo, se esiste.
	Prev punterà al nodo precedente, se esiste.
*/
bool BulletList::pointAt(Position position1, Position position2) {
	initIter();
	bool found = false;

	while (iter != NULL && !found) {
		if (iter->bullet.getPosition().equals(position1) || iter->bullet.getPosition().equals(position2)) {
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
	Prende in input tre oggetti Position.
	Restituisce true se è presente un Bullet in una delle tre posizioni. False altrimenti.
	Iter punterà a quel nodo, se esiste.
	Prev punterà al nodo precedente, se esiste.
*/
bool BulletList::pointAt(Position position1, Position position2, Position position3) {
	initIter();
	bool found = false;

	while (iter != NULL && !found) {
		if (iter->bullet.getPosition().equals(position1) || iter->bullet.getPosition().equals(position2) || iter->bullet.getPosition().equals(position3)) {
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
	Restituisce il numero di nodi con campo hostile=true nella lista
*/
int BulletList::sizeHostile() {
	int i=0;
	FlyingBullet *iterator = list;

	while (iterator != NULL) {
		if (iterator->bullet.isHostile()) {
			i++;
		}
		iterator = iterator->next;
	}

	return i;
}