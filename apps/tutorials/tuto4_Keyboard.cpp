#include <Grapic.h>
#include <iostream>
using namespace grapic;
using namespace std;

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

    if (isKeyPressed(SDLK_UP))     if (d.n < 20) d.n++;     // Increase the number of rectangle when the key UP is pressed
    if (isKeyPressed(SDLK_DOWN))   if (d.n > 1) d.n--;      // Decrease the number of rectangle when the key DOWN is pressed

    if (isKeyPressed('z')) { print(5,5,"z"); cout<<"z"<<endl; }
    if (isKeyPressed(SDLK_e)) { print(5,5,"e"); cout<<"e"<<endl; }
    if ((isKeyPressed(SDLK_1) || (isKeyPressed(SDLK_KP_1)))) { print(50,50,"1"); cout<<"1"<<endl; }

    if (isKeyPressed('a'))     if (d.n < 20) d.n++;     // Increase the number of rectangle when the key UP is pressed
    if (isKeyPressed('w'))     if (d.n > 1) d.n--;      // Decrease the number of rectangle when the key DOWN is pressed

    for(i=1;i<=d.n;i++)
    {
        color(20*i, 255-20*i, 28);
        rectangle( DIMW/2 - 10*i, DIMW/2 - 10*i, DIMW/2 + 10*i, DIMW/2 + 10*i);
    }
}

int main(int , char** )
{
    Data dat;
    bool stop=false;
	winInit("Tutorials", DIMW, DIMW);
    //setKeyRepeatMode(false);                    // Repeat mode is used for game-like application. False for the mode where the key pressed is not repeated
    setKeyRepeatMode(true);                    // Repeat mode is used for game-like application. False for the mode where the key pressed is not repeated
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
