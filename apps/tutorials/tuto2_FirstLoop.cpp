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
    int i;
    for(i=0;i<d.n;i++)
    {
        color(20*i, 255-20*i, 128);
        rectangle( DIMW/2 - 10*i, DIMW/2 - 10*i, DIMW/2 + 10*i, DIMW/2 + 10*i);
    }
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
