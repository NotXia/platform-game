#pragma once
#include "ArmedEntity.hpp"
#include "Weapon.hpp"

class Enemy : public ArmedEntity {
	protected:
		int points;
		int money;
		Position *lastPlayerPosition;
		int visualRange;

		int refreshTime, currRefresh;

	public:
		Enemy(int health=0, int points=0, int money=0, Pixel head_left=Pixel(), Pixel head_right=Pixel(), Pixel body=Pixel(), Position position=Position(), Weapon *weapon=NULL, int visualRange=0);

		/*
			Indica se è possibile eseguire la prossima azione (currRefresh ha raggiunto refreshTime)
		*/
		bool canRefresh();

		/*
			Incrementa currRefresh di 1. Se supera refreshTime, viene azzerato
		*/
		void incRefresh();
};

