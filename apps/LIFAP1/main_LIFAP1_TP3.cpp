#include <Grapic.h>
#include <math.h>

using namespace grapic;

const int DIMW = 501;


void carrecercle(int N, int Rgrand, int Rpetit)
{
    float x,y, r;
    float i;

    // color(R, V, B)
    color(  58,    58,    58);

    for(i=0;i<N;i++)
    {
        r = i*2.0*M_PI/N;

        x = 250+Rgrand*cos(r);
        y = 250+Rgrand*sin(r);
        circle( x, y, Rpetit);
    }
}


void degrRec()
{
    int i;
    for(i=0;i<=255;i++)
    {
        //rectangleFill(  )
        color(i,0,0);
        rectangleFill(2*i,0,i*2+1,DIMW);
    }
}

void degrCercle()
{
    int i;
    for(i=0;i<=255;i++)
    {
        color(i,0,0);
        circleFill( 250, 250, 255-i);
    }
}


void horloge(int Rgrand, int angleDeg)
{
    const int N = 12;
    float x,y, r;
    int i;

    color(  58,    158,    58);
    for(i=0;i<N;i++)
    {
        r = i*2.0*M_PI/N;

        x = 250+Rgrand*cos(r);
        y = 250-Rgrand*sin(r);
        print( x, y, (i+2)%12 + 1  );
    }

    //angleDeg = angleDeg-90;
    float angleRad = angleDeg * 2.0*M_PI / 360;
    x = 250+Rgrand*cos(angleRad);
    y = 250-Rgrand*sin(angleRad);
    line( 250, 250, x,y);
}


void chrono()
{
    int i;
    for(i=-90;;i++)
    {
        winClear ();
        horloge(100, i);
        winDisplay();
        delay(10);
    }
}


void damier()
{
    Image f = image("data/pacman/fantome.png");
    Image p = image("data/pacman/pacman.png");

    const int N=20;
    int i,j;
    int l,h;
    l = DIMW/N;
    h = DIMW/N;
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
        {
            if (i%2==j%2)
                image_draw( p, i*l,j*h, l, h);
            else
                image_draw( f, i*l,j*h, l, h);
        }
}


int main(int , char**)
{
    winInit("TD",DIMW, DIMW) ;
    int i;

    backgroundColor( 0,0,0);
    damier();


    pressSpace() ;
    winQuit();
    return 0;
}

