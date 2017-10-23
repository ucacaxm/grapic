#include <Grapic.h>
#include <math.h>

using namespace grapic;

const int DIMW = 500;


int main(int , char**)
{
    winInit("exercice 3",DIMW, DIMW) ;
    winClear ()  ;
    rectangle( 10, 10, 100, 100);
    winDisplay();
    pressSpace() ;
    winQuit();
    return 0;
}

