#pragma once
#include "Bonus.hpp"

class BonusList {
	protected:
		struct BonusNode {
			Bonus bonus;
			BonusNode *next;
		};

		BonusNode *list;
		BonusNode *prev, *iter;

	public:
		BonusList();

		/*
			Prende in input un oggetto Bonus.
			Lo inserisce in testa alla lista list.
		*/
		void insert(Bonus bonus);

		/*
			Inizializza iter alla testa della lista e prev a NULL.
		*/
		void initIter();

		/*
			Restituisce true se iter è NULL, false altrimenti.
		*/
		bool isNull();

		/*
			Restituisce l'oggetto Bonus contenuto nel nodo attualmente puntato da iter.
			Se iter è NULL, restituisce un oggetto di default.
		*/
		Bonus getCurrent();


		/*
			Elimina il nodo attualmente puntato da iter e muove iter al nodo successivo.
		*/
		void deleteCurrent();

		/*
			Muove iter e prev per puntare al nodo successivo.
		*/
		void goNext();

		/*
			Prende in input un oggetto Position.
			Restituisce true se esiste un nodo con un Bonus nella stessa posizione del parametro, false altrimenti.
			Muove iter e prev in modo tale da puntare a quel nodo, se esiste.
		*/
		bool pointAt(Position position);

		/*
			Prende in input un oggetto Bonus.
			Aggiorna il contenuto del nodo attualmente puntato da iter.
		*/
		void updateCurrent(Bonus bonus);

};

