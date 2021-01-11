#pragma once
#include "Position.hpp"

class LavaList {
	protected:
		struct LavaNode {
			Position position;
			LavaNode *next;
		};

		LavaNode *list;

	public:
		LavaList();

		/*
			Prende in input un oggetto Position.
			Inserisce un nuovo nodo in testa a list.
		*/
		void insert(Position position);

		/*
			Prende in input un oggetto Position.
			Restituisce true se esiste un nodo con quella posizione, false altrimenti.
		*/
		bool existsAt(Position position);
};

