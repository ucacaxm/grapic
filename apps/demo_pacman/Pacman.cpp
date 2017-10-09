#include "Pacman.h"

void pacInit(Pacman *pPac) {
	pPac->x = 1;
	pPac->y = 1;
}

void pacGauche(Pacman *pPac, const Terrain *pTer) {
	if (terEstPositionPersoValide( pTer, pPac->x-1, pPac->y))
		pPac->x--;
}

void pacDroite(Pacman *pPac, const Terrain *pTer) {
	if (terEstPositionPersoValide( pTer, pPac->x+1, pPac->y))
		pPac->x++;
}

void pacHaut(Pacman *pPac, const Terrain *pTer) {
	if (terEstPositionPersoValide( pTer, pPac->x, pPac->y-1))
		pPac->y--;
}

void pacBas(Pacman *pPac, const Terrain *pTer) {
	if (terEstPositionPersoValide( pTer, pPac->x, pPac->y+1))
		pPac->y++;
}

int pacGetX(const Pacman *pPac) {
	return pPac->x;
}

int pacGetY(const Pacman *pPac) {
	return pPac->y;
}
