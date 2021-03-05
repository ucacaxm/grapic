#include <Grapic.h>
#include <iostream>
#include <cmath>
using namespace std;
using namespace grapic;

const int WIN_DIM_X = 600;
const int WIN_DIM_Y = 500;


void fillImage(Image& im)
{
    int i,j;
    im = image(WIN_DIM_X,WIN_DIM_Y);

    for(i=0; i<WIN_DIM_X; i++)
    {
        for(j=0; j<WIN_DIM_Y; j++)
        {
            if ( (i/10) %3 ==0)
                image_set(im, i, j, 255, 0, 0, 255);
            else if ( (i/10) %3 ==1)
                image_set(im, i, j, 0, 255, 0, 255);
            else
                image_set(im, i, j, 0, 0, 255, 255);
        }
    }
    image_savePNG(im, "test.png");
}


void draw(Image im)
{
    image_draw(im, 20, 20, WIN_DIM_X-40, WIN_DIM_Y-40);
    color(255,0,0);
    print(WIN_DIM_X/2 -150, 0, "Image is filled by image_set");
}


int main(int , char** )
{
    bool stop=false;
    winInit("Demo", WIN_DIM_X, WIN_DIM_Y);
    Image im;
    image_printInfo(im);

    fillImage(im);
    image_printInfo(im);

    while( !stop )                              // The application is running in a loop and call the draw function all the time.
    {
        backgroundColor( 100, 80, 200, 255 );
        winClear();
        draw(im);
        stop = winDisplay();
    }
    winQuit();
    return 0;
}
