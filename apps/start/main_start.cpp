#include <Grapic.h>
using namespace grapic;

const int DIMW = 500;

struct World
{
    int n;
};

void init(World& d)
{
    d.n = 10;
}

void draw(World& d)
{
    int i;
    d.n = int((1.f+cos(elapsedTime()))*10.f);
    for(i=0;i<d.n;i++)
    {
        color(20*i, 255-20*i, 128);
        rectangle( DIMW/2 - 10*i, DIMW/2 - 10*i, DIMW/2 + 10*i, DIMW/2 + 10*i);
//        circle( DIMW/2, DIMW/2, 10*i);
    }
//    for(i=0;i<200;i++)
//        circle( DIMW/2, DIMW/2, i);
}

int main(int , char** )
{
    World dat;
    bool stop=false;
	winInit("MyProg", DIMW, DIMW);
	winSetPosition( DIMW, -1, 0, -1, false);
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
