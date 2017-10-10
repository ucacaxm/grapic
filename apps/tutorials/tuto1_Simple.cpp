

#include <Grapic.h>
#include <cmath>
using namespace grapic;



int main(int , char** )
{
	winInit("Simple Drawing", 500, 500);                    // Create the windows of size 500x500 with his title

    backgroundColor( 100, 80, 200, 255 );                   // Define the background color used by winClear

    fontSize(40);                                           // Change the size of the font used by the 'print' function
    int i;
    for(i=0;i<20;++i)
    {
        winClear();                                         // Clear the windows
        color(234,129,34);                                  // Define the default color of the pen
        rectangleFill( i*10, i*10, i*10+100, i*10+100 );    // draw a rectangle from (xmin,ymin) to (xmax,ymax)
        color(120,200,34);
        print( 500-i*10, i*10, "Oh!");                      // Print the text "Oh!" with the bottom left corner in the 2 coordinates
        winDisplay();                                       // Really display the previous drawing
        delay(50);                                          // Delay 50ms to let the user seeing the drawing
    }
    pressSpace();                                                // The user has to press the 'space' key

    backgroundColor( 200, 80, 200, 255 );                   // Define the background color used by winClear
    for(i=0;i<20;++i)
    {
        winClear();                                         // Clear the windows
        color(234,129,34);                                  // Define the default color of the pen
        rectangleFill( 500-i*10, i*10, 500-i*10+100, i*10+100 );    // draw a rectangle from (xmin,ymin) to (xmax,ymax)
        color(120,200,34);
        print( i*10, i*10, "Ah!");                          // Print the text "Ah!" with the bottom left corner in the 2 coordinates
        winDisplay();                                       // Really display the previous drawing
        delay(50);                                          // Delay 50ms to let the user seeing the drawing
    }
    pressSpace();
    winQuit();                                              // Quit and close everything
	return 0;
}
