#include <Grapic.h>
#include <iostream>
#include <cmath>

using namespace std;
using namespace grapic;

const int WIN_DIM_X = 600;
const int WIN_DIM_Y = 500;




int main(int , char** )
{
    bool stop=false;
    winInit("Demo", WIN_DIM_X, WIN_DIM_Y);
    setKeyRepeatMode(true);

    Plot p1, p2;
    plot_setSize(p2, 1000);                     // For the graph/plot p2 the number of value will be 1000 maximum
    float x,y,y2;
    x = 0.f;
    while( !stop )                              // The application is running in a loop and call the draw function all the time.
    {
        //x = elapsedTime();
        x += 0.01f;
        y = cos(x);
        y2 = 0.5*sin(x);

        plot_add(p1, x, y, 0);                     // add a dot in the graph p1, curve number 0(=the default)
        plot_add(p1, x, y2,1);                  // add a dot in the graph p1, curve number 1

        plot_add(p2, x, y, 0);                     // add a dot in the graph p2, curve0;  if 1000 values are already stored in p2, the lowest is removed
        plot_add(p2, x, y2,1);                  // add a dot in the graph p2, curve1;  if 1000 values are already stored in p2, the lowest is removed


        backgroundColor( 55, 0, 0);
        winClear();

        color( 255, 0,0);
        plot_draw( p1, 20, 20, WIN_DIM_X-20, WIN_DIM_Y/2-20 );                      // draw the graph p1
        plot_draw( p2, 20, WIN_DIM_Y/2+20, WIN_DIM_X-20, WIN_DIM_Y-20, false );         // draw the graph p2 on the top of the window

        stop = winDisplay();
    }
    winQuit();
    return 0;
}
