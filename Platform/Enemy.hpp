#pragma once
#include "ArmedEntity.hpp"
#include "Player.hpp"
class Map;

const int ACTION_DO_NOTHING = 0;
const int ACTION_GO_LEFT = 1;
const int ACTION_GO_RIGHT = 2;
const int ACTION_JUMP = 3;
const int ACTION_FALL = 4;
const int ACTION_ATTACK = 5;


class Enemy : public ArmedEntity {
	protected:
		int points;
		int money;
		Position *lastPlayerPosition;
		int visualRange, default_visualRange;

		AnimationTimer refresh;

	public:
		Enemy(int health=0, int points=0, int money=0, Pixel head_left=Pixel(), Pixel head_right=Pixel(), Pixel body=Pixel(), Position position=Position(), Weapon weapon=Weapon());

		int getPoints();
		int getMoney();
		void setMoney(int money);

		/*
			Indica se è possibile eseguire la prossima azione.
		*/
		bool canRefresh();

		/*
			Prende in input un oggetto Player.
			Aggiorna lastPlayerPosition se la posizione del player è nel campo visivo.
		*/
		void searchForPlayer(Player player);

		/*
			Prende in input un oggetto Map e Player.
			Restituisce l'azione da eseguire.
		*/
		int getAction(Map *map, Player player);

		/*
			Richiama la funzione attack() della superclasse e imposta il campo hostile dell'oggetto Bullet restituito a true.
			Restituisce tale oggetto.
		*/
		Bullet attack();

		/*
			Incrementa i vari contatori.
		*/
		void incCounters();

};

