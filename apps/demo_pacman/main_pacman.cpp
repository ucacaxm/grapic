

#include <Grapic.h>
#include <cmath>
#include <cassert>
#include <time.h>
#include "Jeu.h"

using namespace grapic;

const int TAILLE_SPRITE=32;

struct sdlJeu
{
    Jeu jeu;
    Image surface_pacman;
    Image surface_mur;
    Image surface_fantome;
};



void init(sdlJeu& sj)
{
    sj.surface_pacman = image("data/pacman/pacman.png");
    assert( image_isInit(sj.surface_pacman) );

    sj.surface_mur = image("data/pacman/mur.png");
    assert( image_isInit(sj.surface_mur) );

    sj.surface_fantome = image("data/pacman/fantome.png");
    assert( image_isInit(sj.surface_fantome) );
}



void draw(sdlJeu& sj)
{
    backgroundColor( 100, 80, 200, 255 );
    winClear();

    int x,y,i_fant;

    const Jeu *pJeu = &(sj.jeu);
    const Terrain *pTer = jeuGetConstTerrainPtr(pJeu);
    const Pacman *pPac = jeuGetConstPacmanPtr(pJeu);


    for (x=0; x<getDimX(pTer); ++x)
        for (y=0; y<getDimY(pTer); ++y)
            if (terGetXY(pTer,x,y)=='#')
                image_draw( sj.surface_mur, x*TAILLE_SPRITE, y*TAILLE_SPRITE);

    for (i_fant=0; i_fant<pJeu->nb_fantomes; i_fant++)
        image_draw( sj.surface_fantome, (pJeu->tab_fantomes[i_fant].x)*TAILLE_SPRITE, (pJeu->tab_fantomes[i_fant].y)*TAILLE_SPRITE);
    //SDL_apply_surface(  pSdlJeu->surface_fantome, pSdlJeu->surface_ecran, (pJeu->tab_fantomes[i_fant].x)*TAILLE_SPRITE, (pJeu->tab_fantomes[i_fant].y)*TAILLE_SPRITE);

    /* Copier le sprite de Pacman sur l'écran */
    image_draw( sj.surface_pacman, pacGetX(pPac)*TAILLE_SPRITE,  pacGetY(pPac)*TAILLE_SPRITE );
    //SDL_apply_surface(  pSdlJeu->surface_pacman, pSdlJeu->surface_ecran, pacGetX(pPac)*TAILLE_SPRITE,  pacGetY(pPac)*TAILLE_SPRITE );

    winDisplay();
}












int main(int , char** )
{
    sdlJeu sj;
    bool rafraichissement;

    /* Horloges (en secondes) */
    float horloge_courante, horloge_precedente;

    /* Intervalle de temps (en secondes) entre deux évolutions du jeu */
    /* Changer la valeur pour ralentir ou accélérer le déplacement des fantomes */
    float intervalle_horloge = 0.1f;

    jeuInit( &sj.jeu );

    int dimx = getDimX( jeuGetConstTerrainPtr(&sj.jeu) );
    int dimy = getDimY( jeuGetConstTerrainPtr(&sj.jeu) );
    dimx = dimx * TAILLE_SPRITE;
    dimy = dimy * TAILLE_SPRITE;

    winInit("Pacman", dimx, dimy);
    init(sj);
    setKeyRepeatMode(true);
    horloge_precedente = horloge_courante = 0.f;

    while( !winHasFinished() )                              // The application is running in a loop and call the draw function all the time.
    {
        rafraichissement = false;

        /* Récupère l'horloge actuelle et la convertit en secondes */
        horloge_courante = (float)clock()/(float)CLOCKS_PER_SEC;

        /* Si suffisamment de temps s'est écoulé depuis la dernière prise d'horloge */
        if (horloge_courante-horloge_precedente>=intervalle_horloge)
        {
            jeuEvolue(&(sj.jeu));
            rafraichissement = true;
            horloge_precedente = horloge_courante;

            if (isKeyPressed(SDLK_UP))
            {
                jeuActionClavier( &(sj.jeu), 'b');
                rafraichissement = true;
            }
            if (isKeyPressed(SDLK_LEFT))
            {
                jeuActionClavier( &(sj.jeu), 'g');
                rafraichissement = true;
            }
            if (isKeyPressed(SDLK_DOWN))
            {
                jeuActionClavier( &(sj.jeu), 'h');
                rafraichissement = true;
            }
            if (isKeyPressed(SDLK_RIGHT))
            {
                jeuActionClavier( &(sj.jeu), 'd');
                rafraichissement = true;
            }
        }

        if (rafraichissement)
        {
            /* on affiche le jeu sur le buffer caché */
            draw(sj);
        }
    }
    winQuit();
    return 0;
}
