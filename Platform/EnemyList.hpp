#pragma once
#include "Enemy.hpp"
#include <iostream>

const int NOT_FOUND = 0;
const int BODY_FOUND = 1;
const int HEAD_FOUND = 2;

class EnemyList {
	protected:
		struct EnemyNode {
			Enemy enemy;
			EnemyNode *next;
		};
		EnemyNode *list;
		EnemyNode *iter, *prev;

	public:
		EnemyList();

		/*
			Inizializza iter alla testa della lista
		*/
		void initIter();

		/*
			Restituisce l'oggetto Enemy del nodo attualmente puntato da iter
			Se iter è NULL, restituisce un oggetto di default
		*/
		Enemy getCurrent();

		/*
			Muove l'iteratore al nodo successivo
		*/
		void goNext();

		/*
			Elimina il nodo attualmente puntato da iter
		*/
		void deleteCurrent();

		/*
			Aggiunge un nuovo nodo in testa a list
		*/
		void add(Enemy enemy);

		/*
			Restituisce true se iter è NULL, false altrimenti
		*/
		bool isNull();

		/*
			Prende in input una posizione.
			Indica se in quella posizione c'è un nemico.
		*/
		bool existsAt(Position position);

		/*
			Aggiorna l'oggetto Enemy del nodo attualmente puntato da iter
		*/
		void updateCurrent(Enemy enemy);

};
