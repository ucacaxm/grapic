#include <Grapic.h>
#include <math.h>

using namespace grapic;

const int DIMW = 501;




int main(int , char**)
{
    winInit("TD",DIMW, DIMW) ;
    backgroundColor( 0,0,0);
    color( 255,0,0);
    rectangle( 100, 100, 400, 400);
    pressSpace() ;
    winQuit();
    return 0;
}

