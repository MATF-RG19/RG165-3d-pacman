#include "level.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>
#include <stdbool.h>

#define eatcolor 0.333,0.112,1.000
#define playercolor 1.000, 0.549, 0.000
#define playersize 3
#define gh1 0.000, 1.000, 1.000
#define gh2 1.000, 0.078, 0.706
#define gh3 0.000, 0.933, 0.463
#define maxright square*13
#define maxup square*16


bool keystates[256];        //koje dugme je pritisnuto
int map[15][18]={
    {0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0},
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
    {0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0},

};   //niz koji cuva kakvo je dato polje (solid=0,normal=1,coin=2,supercoin=3,teleport=4)

int kopija[15][18]={
    {0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0},
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
    {0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0},

};

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
int lives;		//broj zivota
float immune;		//imunitet za debuging
float eat;		//pacman jede duhove
char dir;       //strana u koju se pacman krece
float size;     //velicina
int remaining;  //broj coina koji je ostao kako bi se pobedilo u igri
}Pacman;

Pacman player;		//globalna promenljiva u kojoj se cuvaju podaci o igracu

//struktura za duha
typedef struct Ghost{
float x;		//pozicija po x koordinati
float y;		//pozicija po y koordinati
float velocity;		//trenutna brzina kretanja
float respawn;          //za koliko ce se duh oziveti
int id;             //id duha
float size;         //velicina duha
int moving;       //indikator kretanja
int dir;            //smer u kome se duh krece
}Ghost;

Ghost ghost1,ghost2,ghost3;

static void move(Ghost *ghost);//funkcija kretanja duha
static void draw_ghost(Ghost *ghost);   //iscrtavanje duha
static void initializeGhost(Ghost *ghost);   //inicijalizacija duha
static void find_move(Ghost *ghost);        //odabir sledeceg polja za kretanje duha
static void choose_new_target(Ghost *ghost);    //izaberi random polje u kvadrantu pacmana


static void new_game(void){

    int i,j;
    player.remaining=0;
	//Player inicijalizacija
	player.x=7.5*square;
	player.y=5.5*square;
	player.velocity=18/60.0;
	player.lives=3;
	player.immune=0;
	player.eat=0;
    player.dir='n';
	player.size=1.5;

    //prebroj novcice u mapi
        for(i=0;i<=14;i++){
        for(j=0;j<=18;j++){
            if(map[i][j]== 2)player.remaining++;
        }
    }

    //resetuj mapu

    for(i=0;i<=14;i++){
        for(j=0;j<=18;j++){
            map[i][j]=kopija[i][j];
        }
    }

    //inicijalizacija duhova
    ghost1.id=1;
    ghost2.id=2;
    ghost3.id=3;
    initializeGhost(&ghost1);
    initializeGhost(&ghost2);
    initializeGhost(&ghost3);
}

static void game_over(void); //funkcija koja se poziva ako nemas vise zivota
static void win(void);

static void moveUp(void);	//deklaracija funkcija kretanja
static void moveLeft(void);
static void moveDown(void);
static void moveRight(void);

int collision (float x, float y); //funkcija detekcije tipa kolizije
static void after_collision(int x, int y, int typeOfCollision);
//funkcija koja preduzima akciju nakon kolizije u zavisnosti od tipa kolizije

static void immunity(int t); //funkcija koja pokazuje da pacman moze da jede duhove
static void eat_a_coin(void);   //funkcija koja se poziva kada pojede novcic

float distance (float x1, float y1, float x2, float y2);    //funkcija razdaljine dve tacke


int main(int argc, char **argv)
{

    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE |GLUT_DEPTH);

    /* Kreira se prozor. */
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition((1920-1280)/2, (1080-720)/2);
    glutCreateWindow("Pacman");


    /* Registruju se callback funkcije. */
    new_game();
    printf("\n REMAINING: %d \n",player.remaining);
    //printf("Ghost1: %.2f %.2f ",ghost1.x,ghost1.y);

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

    //provera da li pacman moze da jede duhove
    if(player.eat>0) player.eat-=1.0/60;

    //provera da li je immune
    if(player.immune>0) player.immune-=1.0/60;

    //GHOST FUNKCIJE
    if(ghost1.respawn>0)ghost1.respawn-=1.0/60;
    if(ghost2.respawn>0)ghost2.respawn-=1.0/60;
    if(ghost3.respawn>0)ghost3.respawn-=1.0/60;

    move(&ghost1);
    move(&ghost2);
    move(&ghost3);



    //keypresses
	if (keystates['i'] || keystates['I']) player.dir='u';
    if (keystates['k'] || keystates['K']) player.dir='d';
	if (keystates['j'] || keystates['J']) player.dir='l';
	if (keystates['l'] || keystates['L']) player.dir='r';
	if (keystates[27]) exit(0);
    if (keystates['n'] || keystates['N']) new_game();
    if (keystates['x'] || keystates['X']) player.immune=5.0;

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

	draw_ground();
	draw_player();
    draw_obstacles();
    if(ghost1.respawn<=0)draw_ghost(&ghost1);
    if(ghost2.respawn<=0)draw_ghost(&ghost2);
    if(ghost3.respawn<=0)draw_ghost(&ghost3);
}

static void draw_player(void){
glPushMatrix();
	if(player.eat>0) glColor3f(eatcolor);
        else glColor3f(playercolor);
    if(player.immune>0) glColor3f(1.0,0.1,0.1);
	glTranslatef(player.x,player.y,player.size);
	glutSolidSphere(player.size,10,10);
glPopMatrix();
}

static void draw_ghost(Ghost *ghost){
glPushMatrix();
    if(ghost->id==1){
        glColor3f(gh1);
    }
    else if(ghost->id==2) {
        glColor3f(gh2);
    }
    else {
        glColor3f(gh3);
    }
    glTranslatef(ghost->x,ghost->y,2);
	glutSolidSphere(ghost->size,10,10);
glPopMatrix();
}

static void moveRight(void){
    int vrednost;
	printf("Moving right position :%.2f,%.2f by :%f \n",player.x, player.y, player.velocity);
	if((vrednost=collision(player.x,player.y))) player.x+=player.velocity;

}

static void moveLeft(void){

	int vrednost;
	printf("Moving left position :%.2f,%.2f by :%f \n",player.x, player.y, player.velocity);
	if((vrednost=collision(player.x,player.y))) player.x-=player.velocity;

}

static void moveUp(void){

    int vrednost;
	printf("Moving up position :%.2f,%.2f by :%f \n",player.x, player.y, player.velocity);
	if((vrednost=collision(player.x,player.y))) player.y+=player.velocity;

}

static void moveDown(void){
    int vrednost;
	printf("Moving down position :%.2f,%.2f by :%f \n",player.x, player.y, player.velocity);
	if((vrednost=collision(player.x,player.y))) player.y-=player.velocity;

}


static void draw_obstacles(void){
    int i=0;
    int j=0;
    for(i=0;i<=14;i++){
        for(j=0;j<=18;j++){
            if(map[i][j]==0)draw_block(i,j);
            else if(map[i][j]==2)draw_coins(i,j);
            else if(map[i][j]==3)draw_supercoins(i,j);
            else if(map[i][j]==4)draw_teleport(i,j);
        }
    }
}

int collision (float x, float y){
    if(player.dir=='n') return 0;
    float next;
    int tile_x=x/square;
    int tile_y=y/square;
    switch(player.dir){
        case 'u':
            next=(player.y+player.velocity+player.size)/square;
            tile_y=next;
            after_collision(tile_x, tile_y, map[tile_x][tile_y]);
            return map[tile_x][tile_y];
            break;

        case 'd':
            next=(player.y-player.velocity-player.size)/square;
            tile_y=next;
            if(next<0)return 0;
            after_collision(tile_x, tile_y, map[tile_x][tile_y]);
            return map[tile_x][tile_y];
            break;

        case 'l':
            next=(player.x-player.velocity-player.size)/square;
            tile_x=next;
            //printf("tile:%d \t next:%f",tile_x,player.x-player.velocity-player.size);
            after_collision(tile_x, tile_y, map[tile_x][tile_y]);
            return map[tile_x][tile_y];
            break;

        case 'r':
            next=(player.x+player.velocity+player.size)/square;
            tile_x=next;
            //printf("\n MAP:%d x:%d y:%d",map[tile_x][tile_y],tile_x,tile_y);
            after_collision(tile_x, tile_y, map[tile_x][tile_y]);
            return map[tile_x][tile_y];
            break;
    }
    return 1;

}


static void after_collision(int x, int y, int typeOfCollision){

    switch(typeOfCollision){
        case 2:
            map[x][y]=1;
            player.remaining--;
            if(player.remaining==0)win();
            break;

        case 3:
            map[x][y]=1;
            immunity(3);
            break;

        case 4:
            if(x==0) player.x=14*square+square/2;
            if(x==14) player.x=square/2;
            break;
    }
}

static void immunity(int t){
    player.eat=t;
    //animacija
}

static void eat_a_coin(void){
    //animacija
}

static void game_over(void){

}

static void win(void){

}


static void move(Ghost *ghost){
    int tilePx,tilePy;
    int tileGx,tileGy;
    tilePx=player.x/square;
    tilePy=player.y/square;
    tileGx=ghost->x/square;
    tileGy=ghost->y/square;

    //test for collision if tiles are the same as of player
    //only if respawn is <=0.0

    if(ghost->respawn<=0){
    if(tilePx==tileGx && tilePy==tileGy){
        if(player.immune<=0){
            if(player.eat>0) initializeGhost(ghost);
            else{
                player.lives-=1;
                if(player.lives==0) game_over();
                else {
                    player.x=7.5*square;
                    player.y=5.5*square;
                }}
            }
        }}
    //find next square to move if not moving already
    if(ghost->moving<=0 && ghost->respawn<=0) find_move(ghost);
    else{
        if(ghost->dir==0) ghost->x-= ghost->velocity;
        if(ghost->dir==1) ghost->x+= ghost->velocity;
        if(ghost->dir==2) ghost->y+= ghost->velocity;
        if(ghost->dir==3) ghost->y-= ghost->velocity;
        if(ghost->moving>0)ghost->moving-= 1;
    }


}
static void initializeGhost(Ghost *ghost){
    switch(ghost->id){
        case 1:
            ghost->x=6.5*square;
            ghost->y=11.5*square;
            ghost->velocity=12.0/60;
            ghost->respawn=1.0;
            ghost->size=1.65;
            ghost->dir=5;
            ghost->moving=0;
            break;

        case 2:
            ghost->x=2.5*square;
            ghost->y=11.5*square;
            ghost->velocity=12.0/60;
            ghost->respawn=3.0;
            ghost->size=1.65;
            ghost->dir=5;
            ghost->moving=0;
            break;

        case 3:
            ghost->x=6.5*square;
            ghost->y=13.5*square;
            ghost->velocity=12.0/60;
            ghost->respawn=5.0;
            ghost->size=1.65;
            ghost->dir=5;
            ghost->moving=0;
            break;
    }
}



static void find_move(Ghost *ghost){
    printf("\n findmove called ghost%d \n",ghost->id);
    srand(time(0));
    int possibility[4]={0,0,0,0}; //0-left 1-right 2-up 3-down

    int Gx,Gy;
    Gx= ghost->x / square;
    Gy= ghost->y / square;
    int i=0;
    if(map[Gx-1][Gy]>0 && map[Gx-1][Gy]<4) possibility[0]=1;
    if(map[Gx+1][Gy]>0 && map[Gx+1][Gy]<4) possibility[1]=1;
    if(map[Gx][Gy+1]>0 && map[Gx][Gy+1]<4) possibility[2]=1;
    if(map[Gx][Gy-1]>0 && map[Gx][Gy-1]<4) possibility[3]=1;

    int broj=0; int direction=5;
    for(i=0;i<4;i++){
        if(possibility[i]==1) {direction=i;broj++;}
    }

    if(broj==1) ghost->dir=direction;
    else{
        int plausable=0;
        int random=rand();
        random+=ghost->id;
        random=random% broj +1;
        for(i=0;i<4;i++)
        {
            if(possibility[i]==1)plausable++;
            if(plausable==random){
            ghost->dir=i;
            break;
            }

        }
    }
    ghost-> moving = square / ghost->velocity; //broj frameova da predje square

}

float distance (float x1, float y1, float x2, float y2){
    float a;
    float b;
    a=(x1- x2) * (x1 - x2);
    b=(y1- y2) * (y1 - y2);

    return sqrt(a*a + b*b);
}
