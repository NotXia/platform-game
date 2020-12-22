#pragma once

#include "Weapon.hpp"
#include "colors.h"
#include "settings.h"

Weapon knife = Weapon(
    "Coltellino",
    Pixel('-', FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Pixel('-', FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Bullet(Pixel(' ', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 1, 2),
    1,
    0,
    3500
);

// TODO: la spada non lancia proiettili, ma una linea retta continua
Weapon sword = Weapon(
    "Spada",
    Pixel(char(196), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Pixel(char(196), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Bullet(Pixel(' ', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 5, 3),
    1,
    0,
    5500
);

Weapon pistol = Weapon(
    "Pistola",
    Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Bullet(Pixel('o', FG_BLACK | BACKGROUND_DEFAULT, false), 2, 10),
    10,
    15000,
    12000
);

Weapon machine_gun = Weapon(
    "Mitraglietta",
    Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Bullet(Pixel('-', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 5, 15),
    20,
    12000,
    5000
);

Weapon assault_rifle = Weapon(
    "Fucile d'assalto",
    Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Bullet(Pixel('-', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 8, 20),
    35,
    15000,
    3500
);

Weapon sniper_rifle = Weapon(
    "Fucile di precisione",
    Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Bullet(Pixel('-', BACKGROUND_DEFAULT | BACKGROUND_DEFAULT, false), 10, GAME_WIDTH),
    5,
    30000,
    70000
);

// TODO: Armi intermedie

// TODO: Migliorare il laser (togliere il proiettile e fare un effettivo laser)
Weapon laser = Weapon(
    "Laser",
    Pixel(char(191), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Pixel(char(218), FG_DARKBLUE | BACKGROUND_DEFAULT, false),
    Bullet(Pixel(char(196), FG_DARKRED | BACKGROUND_DEFAULT, false), 30, GAME_WIDTH),
    1,
    100000,
    90000
);

/*
    Weapon variabile = Weapon(
        "Nome arma",
        Pixel('ch sx', colore | BACKGROUND_DEFAULT, false),
        Pixel('ch dx', colore| BACKGROUND_DEFAULT, false),
        Bullet(Pixel('ch proiettile sx', 'ch proiettile dx', colore | BACKGROUND_DEFAULT, false), danni, range),
        n_caricatore,
        tempo_ricarica,
        tempo_sparo
    );
*/