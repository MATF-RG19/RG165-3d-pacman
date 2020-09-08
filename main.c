#include "level.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>
#include <stdbool.h>

#define playercolor 1.000, 0.549, 0.000
#define playersize 3
#define maxright square*13
#define maxup square*16
 

bool keystates[256];        //koje dugme je pritisnuto
int map[15][18]={
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,3,2,2,2,2,0,2,2,3,2,2,0,2,2,2,2,3},
    {0,2,0,0,0,2,0,2,0,2,0,2,0,2,0,0,0,2},
    {0,2,0,2,2,2,0,2,0,2,0,2,0,2,2,2,0,2},
    {0,2,0,2,0,2,2,2,0,2,0,2,2,2,0,2,0,2},
    {0,3,2,2,0,2,0,0,0,2,0,0,0,2,0,2,2,3},
    {0,0,0,2,0,1,1,1,1,3,1,1,1,1,0,2,0,0},
    {0,2,2,2,0,1,0,3,0,0,0,3,0,1,0,2,2,2},
    {0,0,0,2,0,1,1,1,1,3,1,1,1,1,0,2,0,0},
    {0,3,2,2,0,2,0,0,0,2,0,0,0,2,0,2,2,3},
    {0,2,0,2,0,2,2,2,0,2,0,2,2,2,0,2,0,2},
    {0,2,0,2,2,2,0,2,0,2,0,2,0,2,2,2,0,2},
    {0,2,0,0,0,2,0,2,0,2,0,2,0,2,0,0,0,2},
    {0,3,2,2,2,2,0,2,2,3,2,2,0,2,2,2,2,3},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    
};   //niz koji cuva kakvo je dato polje (solid,coin,supercoin)



static void mapa(void);  //funkcija koja pravi mapu
int brojac=0;

/* Funkcija initalize() vrsi OpenGL inicijalizaciju. */
static void initialize(void);

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_keyboardup(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void timer(int time);

//Draw funkcije
static void draw_level(void);	//pocetno iscrtavanje nivoa
static void draw_player(void);	//iscrtavanje pacmana
static void draw_obstacles(void);   //iscrtaj prepreke

//struktura za playera
typedef struct Pacman{
float x;		//pozicija po x koordinati
float y;		//pozicija po y koordinati
float velocity;		//trenutna brzina kretanja
int lifes;		//broj zivota
int imune;		//imunitet za debuging 
int eat;		//pacman jede duhove
char dir;       //strana u koju se pacman krece
float size;     //velicina
}Pacman;		

Pacman player;		//globalna promenljiva u kojoj se cuvaju podaci o igracu



static void new_game(void){
	//Player inicijalizacija
	player.x=4*square;
	player.y=4*square;
	player.velocity=18/60.0;
	player.lifes=3;
	player.imune=0;
	player.eat=0;
    player.dir='n';
	player.size=1.5;
}


static void moveUp(void);	//deklaracija funckija kretanja
static void moveLeft(void);
static void moveDown(void);
static void moveRight(void);



int collision (float x, float y){
    if(player.dir=='n') return 0;
    float next;
    int tile_x=x/square;
    int tile_y=y/square;
    switch(player.dir){
        case 'u':
            next=(player.y+player.velocity+player.size)/square;
            tile_y=next;
            return map[tile_x][tile_y];
            break;
            
        case 'd':
            next=(player.y-player.velocity-player.size)/square;
            tile_y=next;
            if(next<0)return 0;
            return map[tile_x][tile_y];
            break;
            
        case 'l':
            next=(player.x-player.velocity-player.size)/square;
            tile_x=next;
            //printf("tile:%d \t next:%f",tile_x,player.x-player.velocity-player.size);
            return map[tile_x][tile_y];
            break;
            
        case 'r':
            next=(player.x+player.velocity+player.size)/square;
            tile_x=next;
            //printf("\n MAP:%d x:%d y:%d",map[tile_x][tile_y],tile_x,tile_y);
            return map[tile_x][tile_y];
            break;
    }
    return 1;
    
}

int main(int argc, char **argv)
{
    //mapa();
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE |GLUT_DEPTH);

    /* Kreira se prozor. */
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition((1920-1280)/2, (1080-720)/2);
    glutCreateWindow("Pacman");
    
    
    /* Registruju se callback funkcije. */
    new_game();
    glutKeyboardFunc(on_keyboard);		//key down
    glutKeyboardUpFunc(on_keyboardup);		//key up
    glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutTimerFunc(0,timer,0);
	initialize();
    
    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void on_display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clear last frame
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //reset coordinate system
	//gluLookAt(50,-30,72,50,50,0,0,1,0);	//normal camera
    gluLookAt(50,50,100,50,50,0,0,1,0); //top camera
	//gluLookAt(0,17*square,100, 0,17*square,0 ,0,1,0);

	draw_level();
   	glutSwapBuffers();	
}

static void on_keyboard(unsigned char key, int x, int y)
{	
	keystates[key]=true;
}
static void on_keyboardup(unsigned char key, int x, int y)
{
	keystates[key]=false;
}

static void initialize(void){
	glClearColor(0.0,0.0,0,1.0);
	glEnable(GL_DEPTH_TEST);
}

static void on_reshape(int width, int height){
	//viewport -- rectangle that displays stuff
	glViewport(0,0,width,height);
	

	//projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//resets the model view matrix

	gluPerspective(60,(float)width/height,1,1000);
	glMatrixMode(GL_MODELVIEW);
		
}

static void timer(int time){
	glutPostRedisplay();	
	glutTimerFunc(1000/60,timer,0);  //60 fps per second
    
	if (keystates['i'] || keystates['I']) player.dir='u';
    if (keystates['k'] || keystates['K']) player.dir='d';
	if (keystates['j'] || keystates['J']) player.dir='l';
	if (keystates['l'] || keystates['L']) player.dir='r';
	if (keystates[27]) exit(0);
    if (keystates['q'] || keystates['Q']) gluLookAt(0,17*square,100, 0,17*square,0 ,0,0,1);
    
    switch(player.dir){
            case 'r':
                moveRight();
                break;
            
            case 'l':
                moveLeft();
                break;
            
            case 'u':
                moveUp();
                break;
                
            case 'd':
                moveDown();
                break;
            
            default:
                break;
        }
                
    
    

}

static void draw_level(void){
	
	draw_walls();
	draw_player();
    draw_obstacles();
}

static void draw_player(void){
glPushMatrix();
	glColor3f(playercolor);
	glTranslatef(player.x,player.y,player.size);
	glutSolidSphere(player.size,10,10);
glPopMatrix();
}


static void moveRight(void){

	printf("Moving right position :%.2f,%.2f by :%f \n",player.x, player.y, player.velocity);
	if(collision(player.x,player.y)) player.x+=player.velocity;
	
}

static void moveLeft(void){

	
	printf("Moving left position :%.2f,%.2f by :%f \n",player.x, player.y, player.velocity);
	if(collision(player.x,player.y)) player.x-=player.velocity;
	
}

static void moveUp(void){

	printf("Moving up position :%.2f,%.2f by :%f \n",player.x, player.y, player.velocity);
	if(collision(player.x,player.y)) player.y+=player.velocity;
	
}

static void moveDown(void){

	printf("Moving down position :%.2f,%.2f by :%f \n",player.x, player.y, player.velocity);
	if(collision(player.x,player.y)) player.y-=player.velocity;
	
}

static void mapa(void){
    
    int i=0;
    int j=0;
    for(i=0;i<=14;i++){
        for(j=0;j<=17;j++)map[i][j]=1;
    }
    
    
    for(j=0;j<=17;j++){
        map[0][j]=0;
    }
    
    for(j=0;j<=17;j++){
        map[14][j]=0;
    }
    
    
    for(i=0;i<=14;i++){
        map[i][17]=0;
    }
    
    
}

static void draw_obstacles(void){
    int i=0;
    int j=0;
    for(i=0;i<=14;i++){
        for(j=0;j<=18;j++){
            if(map[i][j]==0){
                glPushMatrix();
                    glColor3f(wallcolor);
                    glTranslatef(square*i+square/2,square*j+square/2,square/2);
                    glScalef(square,square,square);
                    glutSolidCube(1);
                glPopMatrix();
            }
            
            else if(map[i][j]==2)draw_coins(i,j);
            else if(map[i][j]==3)draw_supercoins(i,j);
        }
    }
}
