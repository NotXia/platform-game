#pragma once
#include "Pixel.hpp"
#include "Bullet.hpp"
#include "settings.h"

class Weapon {
	protected:
		char name[STRING_LEN];
		Pixel textureLeft;
		Pixel textureRight;
		Bullet bullet;
		int ammo, curr_ammo;
		bool reloading;
		int reloadDelay, curr_reloadDelay;
		bool shooting;
		int shootDelay, curr_shootDelay;

		/*
			Serve per le funzioni di confronto
			Prende in input un intero.
			Restituisce un carattere in base al valore.
		*/
		char getCheckSymbol(int check);

	public:
		Weapon(const char name[]="", Pixel left=Pixel(), Pixel right=Pixel(), Bullet bullet=Bullet(), int ammo=1, int reloadDelay=1000, int shootDelay=1000);

		Pixel getTexture(bool direction);
		Bullet getBullet();
		void getName(char name[]);
		int getCurrAmmo();
		bool isShooting();
		bool isReloading();

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

