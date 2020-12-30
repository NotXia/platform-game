#pragma once
#include "Entity.hpp"
#include "Weapon.hpp"
#include <iostream>
class Bullet;

class ArmedEntity : public Entity {
	protected:
		Weapon weapon;
		bool is_attacking;
		int weapon_display_counter;

		/*
			Se is_attacking è true: incrementa weapon_loop_counter di 1.
			Se supera il limite, viene azzerato.
		*/
		void incWeaponDisplay();

	public:
		ArmedEntity(int health=0, Pixel head_left=Pixel(' ', 0, false), Pixel head_right=Pixel(' ', 0, false), Pixel body=Pixel(' ', 0, false), Position position=Position(0, 0), Weapon weapon=Weapon());

		Weapon getWeapon();
		void setWeapon(Weapon weapon);


		/****************************
		   INIZIO GESTIONE ATTACCO   
		****************************/

		/*
			Imposta i vari parametri per l'attacco e restituisce un oggetto Bullet identico al Bullet associato all'oggetto weapon
		*/
		Bullet attack();

		/*
			Restituisce true se ci sono le condizioni per attaccare
		*/
		bool canAttack();

		/*
			Indica se terminare la visualizzazione dell'arma quando il giocatore attacca
		*/
		bool endWeaponDisplay();

		/*
			Se il delay per lo sparo è terminato, permette di sparare nuovamente
		*/
		bool hasShootDelayFinished();

		/* FINE GESTIONE ATTACCO  
		*************************/


		/*****************************
		   INIZIO GESTIONE RICARICA   
		*****************************/

		/*
			Avvia la ricarica dell'arma
		*/
		void reload();
		
		/*
			Restituisce true se ci sono le condizioni per ricaricare
		*/
		bool canReload();

		/*
			Se il delay per la ricarica è terminato, ricarica l'arma
		*/
		bool hasReloadFinished();

		/* FINE GESTIONE RICARICA  
		**************************/


		/*
			Incrementa i vari contatori
		*/
		void incCounters();
};

