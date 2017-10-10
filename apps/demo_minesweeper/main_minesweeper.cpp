

#include <Grapic.h>

#define CORRECTION

using namespace grapic;
using namespace std;

const int WIN_DIM_X = 300;
const int WIN_DIM_Y = 300;

struct CaseGrille
{
    int mines;
    bool decouverte;
    bool drapeau;
};

struct Jeu
{
    CaseGrille g[100][100];
	int dx,dy;
	int nb_mines;
	int nb_cases_a_decouvrir;
	bool perdu;
	Image im_mine;
	Image im_drapeau;
};


void placeMines(Jeu& jj)
{
#ifdef CORRECTION
    int i;
    int x,y;
    for(i=0;i<jj.nb_mines;++i)
    {
        do
        {
            x = rand()%jj.dx;
            y = rand()%jj.dy;
        } while( (jj.g[x][y].mines==-1) );
        jj.g[x][y].mines = -1;
    }
#endif //CORRECTION
}


void compteMinesVoisines(Jeu& jj)
{
#ifdef CORRECTION
    int i,j,x,y,n;
    for(i=0;i<jj.dx;++i)
        for(j=0;j<jj.dy;++j)
        {
            if (jj.g[i][j].mines!=-1)
            {
                n = 0;
                for(x=i-1;x<=i+1;++x)
                    for(y=j-1;y<=j+1;++y)
                        if ((x>=0) && (y>=0) && (x<jj.dx) && (y<jj.dy) && (jj.g[x][y].mines==-1))
                        {
                            ++n;
                        }
                jj.g[i][j].mines = n;
                //cout<<i<<" "<<j<<" "<<n<<endl;
            }
        }
#endif //CORRECTION
}



#ifdef CORRECTION
void decouvre(Jeu& jj, int i, int j)
{
    if ( ! ((i>=0) && (j>=0) && (i<jj.dx) && (j<jj.dy))) return;
    if (jj.g[i][j].decouverte) return;
    jj.g[i][j].decouverte = true;
    jj.nb_cases_a_decouvrir--;
    if (jj.g[i][j].mines!=0) return;
    int x,y;
    for(x=i-1;x<=i+1;++x)
        for(y=j-1;y<=j+1;++y)
            decouvre(jj,x,y);
}
#endif //CORRECTION



void init(Jeu& jj)
{
	jj.dx = 10;
	jj.dy = 10;
	jj.im_mine = image("data/mine.png");
	jj.im_drapeau = image("data/flag.png");
	int i,j;
	for(i=0;i<jj.dx;++i)
        for(j=0;j<jj.dy;++j)
        {
            jj.g[i][j].mines = 0;
            jj.g[i][j].decouverte = false;
            jj.g[i][j].drapeau = false;
        }
    jj.nb_mines = 4;
    jj.nb_cases_a_decouvrir = jj.dx * jj.dy - jj.nb_mines;
    jj.perdu = false;

    placeMines(jj);
    compteMinesVoisines(jj);
}

int caseVersEcranX(const Jeu& jj, int x)
{
    return int(x * WIN_DIM_X/jj.dx);
}

int caseVersEcranY(const Jeu& jj, int y)
{
    return int(y * WIN_DIM_Y/jj.dy);
}

int ecranVersCaseX(const Jeu& jj, int x)
{
    return int(x / (WIN_DIM_X/jj.dx));
}

int ecranVersCaseY(const Jeu& jj, int y)
{
    return int(y / (WIN_DIM_Y/jj.dy));
}


void draw(Jeu& jj)
{
	int i,j;
	backgroundColor( 100, 80, 200, 255 );

	color(255, 0, 0);
	grid(0, 0, WIN_DIM_X-1, WIN_DIM_Y-1, jj.dx, jj.dy);

	color(255, 255, 0);
	fontSize(15);

#ifdef CORRECTION
    if (isKeyPressed(SDLK_r)) init(jj);

	for(i=0;i<jj.dx;++i)
        for(j=0;j<jj.dy;++j)
        {
            if (jj.g[i][j].decouverte)
            {
                if (jj.g[i][j].mines==-1)
                {
                    image_draw( jj.im_mine, caseVersEcranX(jj,i) + 5, caseVersEcranY(jj,j) + 5, WIN_DIM_X/jj.dx - 10, WIN_DIM_Y/jj.dy -10);
                    jj.perdu = true;
                }
                else
                {
                    color(220, 230, 240);
                    rectangleFill( caseVersEcranX(jj,i) + 2, caseVersEcranY(jj,j) + 2, caseVersEcranX(jj,i+1) - 2, caseVersEcranY(jj,j+1) - 2);
                    color(255,0,0);
                    print( caseVersEcranX(jj,i)+10, caseVersEcranY(jj,j)+5, jj.g[i][j].mines);
                }
            }
            else
            if (jj.g[i][j].drapeau)
            {
                image_draw( jj.im_drapeau, caseVersEcranX(jj,i) + 5, caseVersEcranY(jj,j) + 5, WIN_DIM_X/jj.dx - 10, WIN_DIM_Y/jj.dy -10);
            }
        }

    if (jj.perdu)
    {
        fontSize(30);
        color(255,120,120);
        print( 10, 10, "PERDU");
        return;
    }
    if (jj.nb_cases_a_decouvrir==0)
    {
        fontSize(30);
        color(255,120,120);
        print( 10, 10, "GAGNE");
        return;
    }

	if (isMousePressed(SDL_BUTTON_LEFT))
	{
		int x, y, i, j;
		mousePos(x, y);
        i = ecranVersCaseX(jj,x);
        j = ecranVersCaseY(jj,y);
        decouvre( jj, i,j);
	}

	if (isMousePressed(SDL_BUTTON_RIGHT))
	{
		int x, y, i, j;
		mousePos(x, y);
        i = ecranVersCaseX(jj,x);
        j = ecranVersCaseY(jj,y);
        jj.g[i][j].drapeau = true; //!jj.g[i][j].drapeau;
	}


    return;
#endif //CORRECTION


	for (i = 0; i < jj.dx; ++i)
	{
        print( caseVersEcranX(jj,i)+10, caseVersEcranY(jj,2)+5, i);
        image_draw( jj.im_mine, caseVersEcranX(jj,2) + 5, caseVersEcranY(jj,i) + 5, WIN_DIM_X/jj.dx - 10, WIN_DIM_Y/jj.dy -10);
	}

	if (isMousePressed(SDL_BUTTON_LEFT))
	{
		int x, y, i, j;
		mousePos(x, y);

        i = ecranVersCaseX(jj,x);
        j = ecranVersCaseY(jj,y);

        image_draw( jj.im_mine, caseVersEcranX(jj,i) + 5, caseVersEcranY(jj,j) + 5, WIN_DIM_X/jj.dx - 10, WIN_DIM_Y/jj.dy -10);
	}

	if (isMousePressed(SDL_BUTTON_RIGHT))
	{
		int x, y, i, j;
		mousePos(x, y);

        i = ecranVersCaseX(jj,x);
        j = ecranVersCaseY(jj,y);

        image_draw( jj.im_drapeau, caseVersEcranX(jj,i) + 5, caseVersEcranY(jj,j) + 5, WIN_DIM_X/jj.dx - 10, WIN_DIM_Y/jj.dy -10);
	}
}





int main(int , char ** )
{
    Jeu jeu;
    bool stop=false;
	winInit("Minesweeper", WIN_DIM_X, WIN_DIM_Y);
	setKeyRepeatMode(false);
	init(jeu);
	while( !stop )
    {
        backgroundColor( 100, 150, 250 );
        winClear();
        draw(jeu);
        stop = winDisplay();
    }
    winQuit();
	return 0;
}
