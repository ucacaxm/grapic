#ifndef _PACMAN_H
#define _PACMAN_H

#include "Terrain.h"

typedef struct {
	int x,y;
} Pacman;

void pacInit(Pacman *);

void pacGauche(Pacman *, const Terrain *);
void pacDroite(Pacman *, const Terrain *);
void pacHaut(Pacman *, const Terrain *);
void pacBas(Pacman *, const Terrain *);

int pacGetX(const Pacman *);
int pacGetY(const Pacman *);

#endif
