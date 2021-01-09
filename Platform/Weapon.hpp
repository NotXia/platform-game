#pragma once
#include "Pixel.hpp"
#include "Bullet.hpp"
#include "settings.h"

const bool WEAPON_MELEE = false;
const bool WEAPON_RANGED = true;

class Weapon {
	protected:
		char name[STRING_LEN];
		Pixel textureLeft;
		Pixel textureRight;
		Bullet bullet;
		bool type;
		int range;

		int ammo, curr_ammo;
		bool reloading;
		AnimationTimer reload_delay;
		bool shooting;
		AnimationTimer shoot_delay;

		/*
			Serve per le funzioni di confronto
			Prende in input un intero.
			Restituisce un carattere in base al valore.
		*/
		char getCheckSymbol(int check);

	public:
		Weapon(const char name[]="", Pixel left=Pixel(), Pixel right=Pixel(), Bullet bullet=Bullet(), int ammo=1, int reloadDelay=1000, int shootDelay=1000, bool type=WEAPON_RANGED, int range=1);

		Pixel getTexture(bool direction);
		void getName(char name[]);
		int getCurrAmmo();
		bool isShooting();
		bool isReloading();
		bool getType();
		int getRange();

		/*
			Restituisce l'oggetto Bullet associato all'arma.
			Se si tratta di un'arma a distanza, viene impostato la distanza di percorrenza del proiettile uguale a range
			Se si tratta di un'arma corpo a corpo, viene impostato la distanza di percorrenza del proiettile a 1
		*/
		Bullet getBullet();

		/*
			Restituisce true se curr_ammo è maggiore di 0, false altrimenti
		*/
		bool hasAmmo();


		/*****************************
		   INIZIO GESTIONE RICARICA   
		*****************************/

		/*
			Inizializza i parametri per visualizzare il tempo di ricarica
		*/
		void startReloadDelay();

		/*
			Incrementa di 1 curr_reloadDelay
		*/
		void incReloadDelay();

		/*
			Restituisce true se curr_reloadDelay ha raggiunto reloadDelay
		*/
		bool canEndReloadDelay();

		/*
			Imposta curr_ammo ad ammo e reloading a false
		*/
		void endReload();

		/* FINE GESTIONE RICARICA
		**************************/


		/**************************
		   INIZIO GESTIONE SPARO   
		**************************/

		/*
			Inizializza i parametri per visualizzare l'attesa tra un colpo e l'altro
		*/
		void startShootDelay();

		/*
			Incrementa di 1 curr_shootDelay
		*/
		void incShootDelay();

		/*
			Restituisce true se curr_shootDelay ha raggiunto shootDelay
		*/
		bool canEndShootDelay();

		/*
			Imposta shooting a false
		*/
		void endShoot();

		/* FINE GESTIONE SPARO
		***********************/


		/******************************
		   INIZIO GESTIONE CONFRONTO   
		******************************/

		/*
			Queste funzioni restituiscono:
			'+' se il campo confrontato di questo oggetto è maggiore di quello del parametro
			'=' se il campo confrontato di questo oggetto è uguale a quello del parametro
			'-' se il campo confrontato di questo oggetto è minore di quello del parametro
		*/

		// Confronto danni
		char higherDamage(Weapon weapon);

		// Confronto capienza caricatore
		char higherAmmo(Weapon weapon);

		// Contronto distanza di sparo
		char higherRange(Weapon weapon);

		// Confronto velocità ricarica
		char fasterReload(Weapon weapon);

		// Confronto velocità sparo
		char fasterShootRate(Weapon weapon);

		/*
			Prende in input un oggetto Weapon
			Confronta l'oggetto corrente con il parametro e dice se sono uguali
		*/
		bool equals(Weapon weapon);

		/* FINE GESTIONE CONFRONTO
		***************************/
};

