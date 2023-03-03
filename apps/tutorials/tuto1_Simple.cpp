

#include <Grapic.h>
#include <cmath>
using namespace grapic;

int main(int , char** )
{
    winInit("Simple Drawing", 500, 500);                    // Create the windows of size 500x500 with his title

    backgroundColor( 100, 80, 200, 255 );                   // Define the background color used by winClear

    fontSize(40);                                           // Change the size of the font used by the 'print' function

    bool stop = false;
    bool oh = true;
    int i = 0;

    while(!stop)
    {
        winClear();                                                 // Clear the windows

        float frameTime = lastFrameTime();                          // Get time passed in milliseconds
        i += std::min(frameTime, 100.0f);                           // Limit the time difference to avoid gaps if the window is frozen (with pressSpace() for exemple)

        if(oh)                                                      // Checks what we want to draw
        {
            color(234,129,34);                                      // Define the default color of the pen
            rectangleFill(i*0.2, i*0.2, i*0.2+100, i*0.2+100);      // Draw a rectangle from (xmin,ymin) to (xmax,ymax)
            color(120,200,34);
            print(500-i*0.2, i*0.2, "Oh!");                         // Print the text "Oh!" with the bottom left corner in the 2 coordinates
            if(i > 1000)                                            // 1 second has passed (1000 ms)
            {
                pressSpace();                                       // Wait for the user to press space
                i = 0;
                oh = false;                                         // Change state 
            }
        } else 
        {
            color(234,129,34);                                          // Define the default color of the pen
            rectangleFill(500-i*0.2, i*0.2, 500-i*0.2+100, i*0.2+100);  // Draw a rectangle from (xmin,ymin) to (xmax,ymax)
            color(120,200,34);
            print(i*0.2, i*0.2, "Ah!");                                 // Print the text "Ah!" with the bottom left corner in the 2 coordinates
            if(i > 1000)                                                // 1 second has passed (1000 ms)
            {
                pressSpace();                                           // Wait for the user to press space
                break;                                                  // Exit the loop
            }
        }
        stop = winDisplay();                                            // Really display the previous drawings
    }
    winQuit();                                              // Quit and close everything
    return 0;
}
