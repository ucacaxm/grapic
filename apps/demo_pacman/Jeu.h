#ifndef _JEU_H
#define _JEU_H

#include "Pacman.h"
#include "Terrain.h"

enum Direction {DROITE, GAUCHE, HAUT, BAS};

typedef struct {
    int x, y;
     Direction direction;
} Fantome;

typedef struct {
	Terrain ter;
	Pacman pac;
	Fantome *tab_fantomes;
	int nb_fantomes;
} Jeu;

void jeuInit(Jeu *);
void jeuLibere(Jeu *);

Terrain * jeuGetTerrainPtr(Jeu *);
Pacman * jeuGetPacmanPtr(Jeu *);
const Terrain * jeuGetConstTerrainPtr(const Jeu *);
const Pacman * jeuGetConstPacmanPtr(const Jeu *);

void jeuActionClavier(Jeu *j, const char);
void jeuEvolue(Jeu *pJeu);

#endif
