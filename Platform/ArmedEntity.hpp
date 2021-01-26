#pragma once
#include <iostream>
#include "Entity.hpp"
#include "Weapon.hpp"

class ArmedEntity : public Entity {
	protected:
		Weapon weapon;

		bool mapEvent;
		AnimationTimer mapEventsTimer;

	public:
		ArmedEntity(int health=0, Pixel head_left=Pixel(), Pixel head_right=Pixel(), Pixel body=Pixel(), Position position=Position(), Weapon weapon=Weapon());

		Weapon getWeapon();
		void setWeapon(Weapon weapon);


		/****************************
		   INIZIO GESTIONE ATTACCO   
		****************************/

		/*
			Imposta i vari parametri per l'attacco e restituisce un oggetto Bullet identico al Bullet associato all'oggetto weapon.
		*/
		Bullet attack();

		/*
			Restituisce true se ci sono le condizioni per attaccare.
		*/
		bool canAttack();

		/*
			Se il delay per lo sparo � terminato, permette di sparare nuovamente.
		*/
		bool hasShootDelayFinished();

		/* FINE GESTIONE ATTACCO  
		*************************/


		/*****************************
		   INIZIO GESTIONE RICARICA   
		*****************************/

		/*
			Avvia la ricarica dell'arma.
		*/
		void reload();
		
		/*
			Restituisce true se ci sono le condizioni per ricaricare.
		*/
		bool canReload();

		/*
			Se il delay per la ricarica � terminato, ricarica l'arma.
		*/
		bool hasReloadFinished();

		/* FINE GESTIONE RICARICA  
		**************************/


		/**************************************
		   INIZIO GESTIONE EVENTI AMBIENTALI
		**************************************/

		bool isMapEvent();

		/*
			Restituisce true se � possibile "valutare" gli eventi ambientali. False altrimenti.
		*/
		bool canMapEvents();

		/*
			Imposta i parametri per gestire gli eventi ambientali.
		*/
		void startMapEvent();
		
		/*
			Imposta mapEvent a false.
		*/
		void stopMapEvent();

		/* FINE GESTIONE EVENTI AMBIENTALI
		***********************************/


		/*
			Incrementa i vari contatori
		*/
		void incCounters();

};

