#pragma once
#include "NPC.hpp"

class NPCList {
	protected:
		struct NPCNode {
			NPC npc;
			NPCNode *next;
		};

		NPCNode *list;
		NPCNode *iter, *prev;

	public:
		NPCList();

		/*
			Prende in input un oggetto NPC.
			Inserisce un nodo in testa a list con tale oggetto
		*/
		void insert(NPC npc);

		/*
			Inizializza iter e prev per puntare alla testa di list
		*/
		void initIter();

		/*
			Restituisce l'oggetto NPC contenuto nel nodo attualmente puntato da iter
		*/
		NPC getCurrent();

		/*
			Indica se iter è NULL
		*/
		bool isNull();

		/*
			Muove iter e prev al nodo successivo
		*/
		void goNext();

		/*
			Prende in input un oggetto Position.
			Cerca se esiste un oggetto NPC in tale posizione.
			Al termine della ricerca iter punterà a tale nodo (se esiste) e prev a quello precedente (se esiste).
		*/
		bool pointAt(Position position);

		/*
			Prende in input un oggetto Position.
			Restituisce true se le coordinate di Position sono contenute nel raggio d'azione di un NPC.
		*/
		bool inPlayerRange(Position position);

		/*
			Prende in input un oggetto NPC.
			Aggiorna l'attuale nodo puntato da iter.
		*/
		void setCurrent(NPC npc);
};

