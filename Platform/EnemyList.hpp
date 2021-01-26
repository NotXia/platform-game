#pragma once
#include "Enemy.hpp"
#include <iostream>

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
			Inizializza iter alla testa della lista e prev a NULL.
		*/
		void initIter();

		/*
			Restituisce l'oggetto Enemy del nodo attualmente puntato da iter
			Se iter è NULL, restituisce un oggetto di default.
		*/
		Enemy getCurrent();

		/*
			Prende in input un oggetto Enemy.
			Aggiunge un nuovo nodo in testa a list.
		*/
		void add(Enemy enemy);

		/*
			Restituisce true se iter è NULL, false altrimenti.
		*/
		bool isNull();

		/*
			Sposta prev al nodo attualmente puntato da iter.
			Sposta iter al nodo successivo.
		*/
		void goNext();

		/*
			Prende in input una posizione.
			Indica se in quella posizione c'è un nemico.
			Iter punterà a quel nodo, se esiste.
			Prev punterà al nodo precedente, se esiste.
		*/
		bool pointAt(Position position);

		/*
			Prende in input una posizione.
			Indica se in quella posizione c'è un nemico.
			Non modifica iter e prev.
		*/
		bool existsAt(Position position);

		/*
			Aggiorna l'oggetto Enemy del nodo attualmente puntato da iter.
			Se il nemico è morto, cancella il nodo attualmente puntato da iter e muove iter al nodo successivo.
		*/
		void updateCurrent(Enemy enemy);

		/*
			Restituisce il numero di nodi nella lista
		*/
		int size();

};
