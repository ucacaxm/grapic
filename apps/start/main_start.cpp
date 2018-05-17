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
    d.n ++;
    for(i=0;i<d.n;i++)
    {
        color(20*i, 255-20*i, 128);
        rectangle( DIMW/2 - 10*i, DIMW/2 - 10*i, DIMW/2 + 10*i, DIMW/2 + 10*i);
    }
}

int main(int , char** )
{
    World dat;
    bool stop=false;
	winInit("MyProg", DIMW, DIMW);
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
