#pragma once
#include "Entity.hpp"
#include "Weapon.hpp"
#include <iostream>
class Bullet;

class ArmedEntity : public Entity {
	protected:
		Weapon weapon;
		bool is_attacking;
		AnimationTimer weapon_animation;

		AnimationTimer mapEventsTimer;
		bool mapEvent;

		/*
			Se is_attacking è true: incrementa weapon_loop_counter di 1.
			Se supera il limite, viene azzerato.
		*/
		void incWeaponDisplay();

	public:
		ArmedEntity(int health=0, Pixel head_left=Pixel(), Pixel head_right=Pixel(), Pixel body=Pixel(), Position position=Position(0, 0), Weapon weapon=Weapon());

		Weapon getWeapon();
		void setWeapon(Weapon weapon);
		bool isAttacking();


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


		/**************************************
		   INIZIO GESTIONE EVENTI AMBIENTALI
		**************************************/

		bool isMapEvent();

		/*
			Restituisce true se è possibile "valutare" gli eventi ambientali. False altrimenti
		*/
		bool canMapEvents();

		/*
			Imposta mapEvent a true
		*/
		void startMapEvent();
		
		/*
			Imposta mapEvent a false
		*/
		void stopMapEvent();

		/* FINE GESTIONE EVENTI AMBIENTALI
		***********************************/


		/*
			Incrementa i vari contatori
		*/
		void incCounters();

};

