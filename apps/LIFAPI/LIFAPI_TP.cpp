// apps/LIFAPI/LIFAPI_TP.cpp
#include<Grapic.h>

using namespace grapic;


int main(int , char**)
{
	winInit("vide",500,500);
	backgroundColor(120,70,200);
	color(220,70,100);
	winClear();
	rectangleFill(200,200, 300, 300);
	pressSpace();
	winQuit();
	return 0;
}
