#include <Grapic.h>
using namespace grapic;

const int DIMW = 500;

struct Data
{
    int n;
};

void init(Data& d)
{
    d.n = 10;
}


void draw(Data& d)
{
    color(255,0,0);
    rectangle(  10, 10, DIMW/2 -30, DIMW/2-30 );

    color(0,255,0);
    rectangleFill(  DIMW/2 + 30, 10, DIMW-10, DIMW/2-30 );

    color(0,0,255);
    circleFill(  250, 250, 20);
    color(255,255,128);
    circle(  250, 250, 30);

    color(255,255,0);
    line( DIMW/2+30, DIMW/2+30, DIMW-10, DIMW-10);

    color(255,0,255);
    grid( 10, DIMW-10, DIMW/2-30, DIMW/2+30, 3, 5);

    triangle( 20,20, 40,30, 30, 50);
    triangleFill( 50,50, 80,60, 70, 90);

    int p[5][2] = { {100,100}, {120,100}, {140,130}, {130,150}, {120, 170} };
    polygon(p, 5);

    int p2[5][2] = { {150,150}, {170,150}, {190,180}, {180,200}, {170, 220} };
    polygonFill(p2,5);

}


int main(int , char** )
{
    Data dat;
    bool stop=false;
	winInit("Tutorials", DIMW, DIMW);
	init(dat);
    backgroundColor( 100, 50, 200 );
	while( !stop )
    {
        winClear();
        draw(dat);
        stop = winDisplay();
    }
    winQuit();
	return 0;
}
