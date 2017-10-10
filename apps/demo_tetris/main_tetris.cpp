

#include <Grapic.h>
#include <cmath>
#include <iostream>
using namespace std;
using namespace grapic;


const int SIZE_X = 8;
const int SIZE_Y = 16;
const int SIZE_SPRITE_X = 32;
const int SIZE_SPRITE_Y = 32;
const int SIZE_BLOCK = 3;


struct Data
{
	int level[SIZE_X][SIZE_Y];
	int block[SIZE_BLOCK ][SIZE_BLOCK ];
	int pos_x, pos_y;
	float temps;
	float speed;
	int lost;
};


void createBlock(Data& d)
{
    int i,j;
    for(i=0;i<SIZE_BLOCK ;++i)
        for(j=0;j<SIZE_BLOCK ;++j)
        {
            d.block[i][j] = irand(0,1);
            //if (i==0) d.block[i][j]=1;
        }
    d.pos_x = SIZE_X/2;
    d.pos_y = SIZE_Y-3;
    d.speed = 0.f;
    //cout<<"create blocks\n";
}

void rotateBlockLeft(Data& d)
{
    int i,j;
    int t[SIZE_BLOCK ][SIZE_BLOCK ];
    for(i=0;i<SIZE_BLOCK ;++i)
        for(j=0;j<SIZE_BLOCK ;++j)
            t[i][j] = d.block[i][j];

    for(i=0;i<SIZE_BLOCK ;++i)
        for(j=0;j<SIZE_BLOCK ;++j)
            d.block[SIZE_BLOCK -1-j][i] = t[i][j];
}

void draw_rect(int i, int j, int c=1)
{
    color(c*100, 255-c*20, 10*c);
    rectangleFill( i*SIZE_SPRITE_X+2, j*SIZE_SPRITE_Y+2, (i+1)*SIZE_SPRITE_X-2, (j+1)*SIZE_SPRITE_Y-2);
}

void drawLevel(Data& d)
{
    int i,j;
    for(i=0;i<SIZE_X;++i)
        for(j=0;j<SIZE_Y;++j)
        {
            if (d.level[i][j]) draw_rect(i,j,1);

        }
}

void drawBlocks(Data& d)
{
    int i,j;
    for(i=0;i<SIZE_BLOCK ;++i)
        for(j=0;j<SIZE_BLOCK ;++j)
        {
            if (d.block[i][j])
                draw_rect(i+d.pos_x,j+d.pos_y,1);
        }
}

bool blocksValid(Data& d, int pos_x, int pos_y)
{
    int i,j;
    for(i=0;i<SIZE_BLOCK;++i)
        for(j=0;j<SIZE_BLOCK;++j)
        {
            if (d.block[i][j])
            {
                if ((i+pos_x<0) || (i+pos_x>=SIZE_X)) return false;
                if ((j+pos_y<0) || (j+pos_y>=SIZE_Y)) return false;
                if (d.level[i+pos_x][j+pos_y])
                    return false;
            }
        }
    return true;
}

bool blocksTransfer(Data& d)
{
    int i,j;
    for(i=0;i<SIZE_BLOCK ;++i)
        for(j=0;j<SIZE_BLOCK ;++j)
        {
            if (d.block[i][j])
            {
                d.level[i+d.pos_x][j+d.pos_y] = 1;
            }
        }
    return true;
}

bool checkLine(Data& d, int l)
{
    int i;
    for(i=0;i<SIZE_X;++i)
        if (!d.level[i][l]) return false;
    return true;
}

void suppressLine(Data& d, int line)
{
    cout<<"suppress "<<line<<endl;
    int x,l;
    for(l=line+1;l<SIZE_Y;++l)
        for(x=0;x<SIZE_X;++x)
            d.level[x][l-1] = d.level[x][l];
}

void checkLines(Data& d)
{
    int l;
    for(l=0;l<SIZE_Y;++l)
        if (checkLine(d,l))
        {
            suppressLine(d,l);
            l--;
        }
}


void init(Data& d)
{
    int i,j;
    for(i=0;i<SIZE_X;++i)
        for(j=0;j<SIZE_Y;++j)
        {
            d.level[i][j] = 0;
            //if (j==0) d.level[i][j] = 1;
            //if (j==1) d.level[i][j] = 1;
        }
    createBlock(d);
    d.temps = elapsedTime();
    d.lost = false;

    fontSize(20);

    checkLines(d);
}


void draw(Data& d)
{
    if (d.lost)
    {
        color(255,0,0);
        print(10, 100, "YOU HAVE LOST");
        print(10, 50, "press 'r' to retry");
        if (isKeyPressed(SDLK_r)) init(d);
        return ;
    }

	if (isKeyPressed(SDLK_LEFT))
    {
        if (blocksValid(d,d.pos_x-1,d.pos_y)) d.pos_x--;
        d.speed = 500;
    }
	if (isKeyPressed(SDLK_UP))
    {
        rotateBlockLeft(d);
        d.speed = 500;
    }
	if (isKeyPressed(SDLK_RIGHT))
    {
        if (blocksValid(d,d.pos_x+1,d.pos_y)) d.pos_x++;
        d.speed = 500;
    }
	if (isKeyPressed(SDLK_DOWN))
    {
        bool cont = true;
        while(cont)
        {
            if (blocksValid(d,d.pos_x,d.pos_y-1)) d.pos_y--;
            else
            {
                if (d.pos_y==SIZE_Y-2)
                {
                    d.lost = true;
                    return;
                }
                blocksTransfer(d);
                checkLines(d);
                createBlock(d);
                cont = false;
            }
        }
        d.speed = 1000;
    }

    float t = elapsedTime();
    if (t-d.temps>d.speed)
    {
        if (blocksValid(d,d.pos_x,d.pos_y-1)) d.pos_y--;
        else
        {
            if (d.pos_y==SIZE_Y-2)
            {
                d.lost = true;
                return;
            }
            blocksTransfer(d);
            checkLines(d);
            createBlock(d);
        }
        d.temps = t;
    }


    backgroundColor( 100, 80, 200, 255 );
    winClear();
    color(115, 90, 210);
    grid( 0,0,SIZE_X * SIZE_SPRITE_X-1, SIZE_Y * SIZE_SPRITE_Y-1, SIZE_X, SIZE_Y);
    drawLevel(d);
    drawBlocks(d);
    winDisplay();
    d.speed = 1;
}






int main(int , char** )
{
    Data dat;
	winInit("Tetris", SIZE_X*SIZE_SPRITE_X, SIZE_Y*SIZE_SPRITE_Y);
	setKeyRepeatMode(false);
	init(dat);
	while( !winHasFinished() )
    {
        draw(dat);
    }
    winQuit();
	return 0;
}
