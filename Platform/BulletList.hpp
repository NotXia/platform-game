#include "Bullet.hpp"
#include "Position.hpp"

class BulletList {
	protected:
		struct FlyingBullet {
			Bullet bullet;
			FlyingBullet *next;
		};

		FlyingBullet *list;
		FlyingBullet *prev, *iter;

	public:
		BulletList();

		/*
			Prende in input un oggetto di tipo Bullet.
			Inserisce l'input in testa alla lista list
		*/
		void insert(Bullet bullet);

		/*
			Inizializza iter alla testa della lista e prev a NULL
		*/
		void initIter();

		/*
			Restituisce true se iter è NULL, false altrimenti
		*/
		bool isNull();

		/*
			Restituisce l'oggetto Bullet del nodo attualmente puntato da iter. Se è NULL, restituisce l'oggetto di default
		*/
		Bullet getCurrent();

		/*
			Aggiorna l'oggetto Bullet del nodo attualmente puntato da iter
		*/
		void updateCurrent(Bullet bullet);

		/*
			Prende in input due oggetti Position.
			Restituisce true se è presente un Bullet in una delle due posizioni. False altrimenti.
			Iter punterà a quel nodo, se esiste.
			Prev punterà al nodo precedente, se esiste.
		*/
		bool pointAt(Position position1, Position position2);

		/*
			Prende in input tre oggetti Position.
			Restituisce true se è presente un Bullet in una delle tre posizioni. False altrimenti.
			Iter punterà a quel nodo, se esiste.
			Prev punterà al nodo precedente, se esiste.
		*/
		bool pointAt(Position position1, Position position2, Position position3);

};


