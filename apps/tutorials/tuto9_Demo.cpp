#include <Grapic.h>
#include <iostream>
#include <cmath>
using namespace std;
using namespace grapic;

const int WIN_DIM_X = 600;
const int WIN_DIM_Y = 500;

struct Data                         // An instance of this structure is passed to each functions: init, draw and animate
{
    int n;                          // Here we store the number of demo
    int demo;                       // Which demo is running
    int x,y;
    Image im;                       // An image
    Image im2;                       // An image
};
void demo0()
{
    int i,j;
    const int l = 50;
    for(i=0;i<10;++i)
        for(j=0;j<10;++j)
        {
            color( 255*i*10/100, 255*j*10/100, 0);
            rectangleFill( i*l+1, j*l+1, (i+1)*l-1, (j+1)*l-1 );
        }
}
void demo1()
{
    int x,y;
    mousePos(x,y);
    color(255,123,34);                                   // Get the mouse position in (x,y)
    if ((x>310) && (y>400) && (x<450) && (y<550))                 // Check if the mouse is in a square (10,400)(50,600)
    {
        fontSize(32);                                           // Change the default size of the font
        print(310,400, "Grand");                          // Print text, left corner of the text is the pixel (10,400)
    }
    else
    {
        fontSize(12);
        print( 310, 400, "Petit");
    }
}
void demo2()
{
    int i,n = static_cast<int>(10 + 10 * cos(5.f*elapsedTime()));
    for (i = 0; i < n; ++i)                                     // Draw a set of rectangles one inside each others
    {
        color( 10 * i, 220 - 10 * i, 150 );
        rectangle(150+5 * i, 150+5 * i, 400 - 5 * i, 400 - 5 * i);
    }
}
void demo3()
{
    float angle;
    float rayon = 100;
    int i,n = 20;
    for (i = 0; i < n; ++i)                                     // Draw a set of rectangles one inside each others
    {
        color( 10 * i, 10 * i, 50 );
        angle = 2.f * M_PI*i/ n;
        rectangle(  250+rayon * cos(angle)-10, 250+rayon * sin(angle)-10, 250+rayon * cos(angle)+10, 250+rayon * sin(angle)+10 );
    }
    color(255,0,255);
    circle( 250, 430, 50);

    color(255,255,0);
    circleFill( 250, 70, 50);
}
void demo4()
{
    int i;
    int n=250;
    for(i=0;i<n;i++)
    {
        color(i,i,i);
        rectangleFill( 2*i,0,2*i+1,500);
    }
}
void demo5(Data& d)
{
    color(255, 0, 0);
    grid(0, 0, 500, 500, 8, 8);                               // Draw a grid in the rectangle (250,250)(500,500) with 8 columns and 8 rows
    if (isKeyPressed(SDLK_a))      if (d.x > 0) d.x--;    // Move the previous rectangle
    if (isKeyPressed(SDLK_LEFT))   if (d.x > 0) d.x--;    // Move the previous rectangle
    if (isKeyPressed(SDLK_RIGHT))  if (d.x < 499) d.x++;
    if (isKeyPressed(SDLK_DOWN))   if (d.y > 0) d.y--;
    if (isKeyPressed(SDLK_UP))     if (d.y < 499) d.y++;
    image_draw(d.im, d.x, d.y);
    int x, y;
    mousePos(x, y);
    color(0, 100, 255);
    rectangleFill( x-10, y-10, x+10, y+10);               // Draw a rectangle around the Data (d.x, d.y) which is store in the parameter data of the function
    if (isMousePressed(SDL_BUTTON_LEFT))
    {
        int x, y;
        mousePos(x, y);
        color(255, 0, 100);
        if ((x>250) && (y<250))
            rectangleFill(250, 0, 500, 250);
    }
}
void demo6(Data& d)
{
    int i,j;
    for(i=0;i<10;i++)
        for(j=0;j<10;j++)
        {
            if ( (i+j)%2==0) image_draw( d.im, i*50, j*50, 50, 50);
            else image_draw( d.im2, i*50, j*50, 50, 50);
        }
}
void demo7()
{
    int i;
    float a,b;
    print( 10, WIN_DIM_Y-50, elapsedTime());
    for(i=0;i<12;++i)
    {
        a = -(-0.5f*M_PI + 2.f*M_PI*float(i+1)/12);
        color(25,245,23);
        print( 250+100*cos(a), 250+100*sin(a), i+1);

        b = -(-0.5f*M_PI + 2.f*M_PI*elapsedTime()/12);
        color(255,45,23);
        line(250,250,250+80*cos(b), 250+80*sin(b));
    }
}
int caseToPixel(Data& d, int c)
{
    return (d.n-c) * WIN_DIM_Y/d.n;
}
void menu(Data& d)
{
    if (isMousePressed(SDL_BUTTON_LEFT))
    {
        int x, y;
        mousePos(x, y);
        if (x>WIN_DIM_X-100)
        {
            d.demo = d.n -1-y / (WIN_DIM_Y/d.n);
        }
    }
    fontSize(20);                                           // Change the default size of the font
    color(255, 0, 0);                                           // Change the default color (the color of the pen)
    grid(WIN_DIM_X-100,0,WIN_DIM_X-1,WIN_DIM_Y, 1, d.n);
    color(0,255,124);
    rectangleFill( WIN_DIM_X-99, caseToPixel(d,d.demo)+1, WIN_DIM_X-2, caseToPixel(d,d.demo+1)-2 );
    color(0, 0, 0);                                           // Change the default color (the color of the pen)
    int i;
    for(i=0;i<d.n;++i)
    {
        print(WIN_DIM_X-100+10, caseToPixel(d,i+1)+20, "Demo");
        print(WIN_DIM_X-100+70, caseToPixel(d,i+1)+20, i);
    }
}

void init(Data& d)                                                      // The init function is called once at the beginning to initialize the Data.
{
    d.n = 8;
    d.demo = 4;
    d.x = 250;
    d.y = 250;
    d.im = image("data/pacman/pacman.png", true, 255, 255, 255, 255);          // Load an image "data/grapic.bmp and store it in d.im
    if (!image_isInit(d.im))                                            // If the image is not found, try in an other directory
        d.im = image("../data/pacman/pacman.png", true, 255,255,255,255 );
    assert( image_isInit(d.im) );
    d.im2 = image("data/pacman/fantome.png", true, 255, 255, 255, 255);          // Load an image "data/grapic.bmp and store it in d.im
    if (!image_isInit(d.im2))                                            // If the image is not found, try in an other directory
        d.im = image("../data/pacman/fantome.png", true, 255,255,255,255 );
    assert( image_isInit(d.im2) );
}
void draw(Data& d)
{
    menu(d);
    switch(d.demo)
    {
        case 0: demo0(); break;
        case 1: demo1(); break;
        case 2: demo2(); break;
        case 3: demo3(); break;
        case 4: demo4(); break;
        case 5: demo5(d); break;
        case 6: demo6(d); break;
        case 7: demo7(); break;
    }
}
int main(int , char** )
{
    Data dat;
    bool stop=false;
    winInit("Demo", WIN_DIM_X, WIN_DIM_Y);
    setKeyRepeatMode(true);
    init(dat);
    while( !stop )                              // The application is running in a loop and call the draw function all the time.
    {
        backgroundColor( 100, 80, 200, 255 );
        //backgroundColor( 255, 255, 255, 255 );
        winClear();
        draw(dat);
        stop = winDisplay();
    }
    winQuit();
    return 0;
}
