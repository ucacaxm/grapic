#include "Jeu.h"
#include <stdlib.h>

void jeuInit(Jeu * pJeu) {
    int x, y, i_fant;

	pacInit(&(pJeu->pac));
	terInit(&(pJeu->ter));

	pJeu->nb_fantomes = 5;
	pJeu->tab_fantomes = (Fantome*)malloc(pJeu->nb_fantomes*sizeof(Fantome));

	for (i_fant=0; i_fant<pJeu->nb_fantomes; i_fant++) {
        do {
            x = rand()%pJeu->ter.dimx;
            y = rand()%pJeu->ter.dimy;
        } while (!terEstPositionPersoValide(&(pJeu->ter), x, y));

        pJeu->tab_fantomes[i_fant].x = x;
        pJeu->tab_fantomes[i_fant].y = y;

        pJeu->tab_fantomes[i_fant].direction = (Direction)(rand()%4);
    }
}

void jeuLibere(Jeu *pJeu) {
	terLibere(&(pJeu->ter));
	free(pJeu->tab_fantomes);
}

Terrain *jeuGetTerrainPtr(Jeu *pJeu) {
	return &(pJeu->ter);
}

Pacman *jeuGetPacmanPtr(Jeu *pJeu) {
	return &(pJeu->pac);
}

const Terrain *jeuGetConstTerrainPtr(const Jeu *pJeu) {
	return &(pJeu->ter);
}

const Pacman *jeuGetConstPacmanPtr(const Jeu *pJeu) {
	return &(pJeu->pac);
}

void jeuActionClavier(Jeu *pJeu, const char touche) {
	switch(touche) {
		case 'g' :
				pacGauche(&(pJeu->pac), &(pJeu->ter));
				break;
		case 'd' :
				pacDroite(&(pJeu->pac), &(pJeu->ter));
				break;
		case 'h' :
				pacHaut(&(pJeu->pac), &(pJeu->ter));
				break;
		case 'b' :
				pacBas(&(pJeu->pac), &(pJeu->ter));
				break;
		default : break;
	}
}

void jeuEvolue(Jeu *pJeu) {
    int i_fant, testx, testy;
    Fantome *p_fant;
    int depx[] = {-1, 1, 0, 0};
    int depy[] = {0, 0, -1, 1};

    for (i_fant=0; i_fant<pJeu->nb_fantomes; i_fant++) {
        p_fant = &(pJeu->tab_fantomes[i_fant]);

        testx = p_fant->x + depx[p_fant->direction];
        testy = p_fant->y + depy[p_fant->direction];

        if (terEstPositionPersoValide(&(pJeu->ter), testx, testy)) {
            p_fant->x = testx;
            p_fant->y = testy;
        }
        else {
            /* Changement de direction */
            p_fant->direction = (Direction)(rand()%4);
        }
    }
}
