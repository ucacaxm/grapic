#include <Grapic.h>
using namespace grapic;

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
    for(i=0;i<d.n;i++)
    {
        color(20*i, 255-20*i, 128);
        rectangle( DIMW/2 - 10*i, DIMW/2 - 10*i, DIMW/2 + 10*i, DIMW/2 + 10*i);
    }
}

int main(int , char** )
{
    Data dat;
    Menu m;
    bool stop=false;
	winInit("Tutorials", DIMW, DIMW);
	init(dat);
    backgroundColor( 100, 50, 200 );

    menu_add( m, "Question 1");
    menu_add( m, "Question 2");
    menu_add( m, "Question 3");
    menu_add( m, "Question 4");
    //menu_add_toggle( m, "Choix ")

	while( !stop )
    {
        winClear();
        menu_draw(m, 5,5, 100, 102);
        switch(menu_has_changed(m))
        {
            case 0:
                std::cout << "Selecting 0-th item" << std::endl;
                break;
            case 1:
                std::cout << "Selecting 1-th item" << std::endl;
                break;
            case 2:
                std::cout << "Selecting 2-th item" << std::endl;
                break;
            case 3:
                std::cout << "Selecting 3-th item" << std::endl;
                break;
            default:
                break;
        }

        switch(menu_select(m))
        {
            case 0 : dat.n = 5; draw(dat); break;
            case 1 : dat.n = 15; draw(dat); break;
            case 2 : dat.n = 10; draw(dat); break;
            case 3 : dat.n = 20; draw(dat); break;
            default: dat.n=0; draw(dat); break;
        }
        stop = winDisplay();
    }
    winQuit();
	return 0;
}
