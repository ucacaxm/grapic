#include <Grapic.h>
#include <cmath>
#include <iostream>

using namespace std;
using namespace grapic;


const int DIMW = 500;

int main(int , char** )
{
	winInit("Simple Drawing", 500, 500);                    // Create the windows of size 500x500 with his title

	backgroundColor( 255, 255, 255);
    winClear();                                         // Clear the windows

    color(234,129,34);                                  // Define the default color of the pen
    rectangle( DIMW/2-50, DIMW/2-50, DIMW/2+50, DIMW/2+50 );    // draw a rectangle from (xmin,ymin) to (xmax,ymax)
    winDisplay();                                       // Really display the previous drawing

    pressSpace();                                                // The user has to press the 'space' key

    winQuit();                                              // Quit and close everything
	return 0;
}
