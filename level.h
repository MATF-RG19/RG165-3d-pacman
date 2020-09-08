#ifndef LEVEL_H
#define LEVEL_H

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#define PI 3.14159265358979323846	
#define wallcolor 0.282, 0.820, 0.800
#define wallsize 2
#define square 4
#define coincolor 1.0, 0.843, 0.0

void draw_walls(void); //iscrtava zidove
void draw_coins(int i, int j); //iscrtava coine
void draw_supercoins(int i, int j); //supercoin


#endif
