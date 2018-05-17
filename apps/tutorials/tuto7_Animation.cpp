#include <Grapic.h>
using namespace grapic;

const int DIMW = 500;

struct Data
{
    int x,y;
    float angle;
    Image im;
};

void update(Data& d)
{
    const float vitesse = 20.f;
    float temps = elapsedTime();
    d.angle = vitesse * temps;

    d.x = temps * cos(d.angle);
    d.y = temps * sin(d.angle);
}

void init(Data& d)
{
    update(d);
    d.im = image("data/mines/mine.png");
}

void draw(Data& d)
{
    color(255,255,0);
    circleFill( DIMW/2 + d.x, DIMW/2+d.y, 10);
    image_draw( d.im, 250, 100, 64, 64, d.angle, 2);
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
        update(dat);
    }
    winQuit();
	return 0;
}
