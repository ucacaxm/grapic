

#include <Grapic.h>
using namespace grapic;


const int DIM = 9;
const int DIM_SPRITE = 32;
enum Case { Blanc, Noir, Vide };
struct Data
{
	Case p[DIM][DIM];
	int b;		// nb piece blanche prise
	int n;		// nb piece noire prise
};


bool caseBlanc(int i, int j)
{
	return ((j*DIM + i) % 2) == 0;
}

void damierAff(Data& d)
{
	int i, j;
	for (i = 0; i < DIM; ++i)
		for (j = 0; j < DIM; ++j)
		{
			if (caseBlanc(i, j))
				color(255, 255, 255);
			else
				color(0, 0, 0);
			rectangleFill(i*DIM_SPRITE, j*DIM_SPRITE, (i + 1)*DIM_SPRITE, (j + 1)*DIM_SPRITE);

			if (d.p[i][j]==Blanc)
				color(255, 255, 0);
			else if (d.p[i][j] == Noir)
				color(0, 0, 255);
			if (d.p[i][j]!=Vide)
				rectangleFill(i*DIM_SPRITE+10, j*DIM_SPRITE+10, (i + 1)*DIM_SPRITE-10, (j + 1)*DIM_SPRITE-10);

		}
}


void init(Data& d)
{
	d.b = d.n = 0;
	int i, j;
	for (i = 0; i < DIM; ++i)
		for (j = 0; j < DIM; ++j)
		{
			d.p[i][j] = Vide;
			if (caseBlanc(i, j))
			{
				if (j < 3) d.p[i][j] = Blanc;
				else if (j > DIM-3-1) d.p[i][j] = Noir;

			}
		}
}



void selectCase(int x, int y, int& i, int& j)
{
	i = x / 32;
	j = y / 32;
}

void draw(Data& d)
{
	backgroundColor( 100, 80, 200, 255 );
	damierAff(d);

	if (isMousePressed(SDL_BUTTON_LEFT))
	{
		int x, y, i, j;
		mousePos(x, y);
		selectCase(x, y, i, j);
		d.p[i][j] = Vide;
	}
}




int main(int , char ** )
{
	Data dat;
    bool stop=false;
	winInit("Checkers", DIM*DIM_SPRITE, DIM*DIM_SPRITE);
	init(dat);
	while( !stop )
    {
        backgroundColor( 100, 80, 200, 255 );
        winClear();
        draw(dat);
        stop = winDisplay();
    }
    winQuit();
	return 0;
}
