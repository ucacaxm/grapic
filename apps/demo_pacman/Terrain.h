#ifndef _TERRAIN_H
#define _TERRAIN_H

typedef struct {
	int dimx;
	int dimy;
	char **tab;
} Terrain;

void terInit(Terrain *);
void terLibere(Terrain *);

int terEstPositionPersoValide(const Terrain *, const int x, const int y);
char terGetXY(const Terrain *, const int x, const int y);
void terSetXY(const Terrain *, const int x, const int y, const char);

int getDimX(const Terrain *);
int getDimY(const Terrain *);

#endif
