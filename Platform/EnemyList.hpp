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

};
