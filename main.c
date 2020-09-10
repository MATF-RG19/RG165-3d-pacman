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
#define norm 0,1,0

char remains[20];
char numLives[10];
char pauseMsg1[30]="THE GAME IS PAUSED";
char pauseMsg2[30]="PRESS 'P' TO PLAY";
int won=0; int follow=0; int lost=0;
double topX=7.5*square,topY=9.5*square,topZ=18*square,top1=7.5*square, top2=9.5*square, top3=0;
double positionX,positionY,positionZ,tarX,tarY,tarZ;
float jump_animation=0.0;

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

int brojac=5;
int paused=0;
int pausable=60;    //ne moze se pauzirati prvu sekundu
static void pause(void);

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
float z;        //pozicija po z koordinati
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
	player.velocity=0.25;
	player.lives=3;
	player.immune=0;
	player.eat=0;
    player.dir='n';
	player.size=1.5;
	player.z=player.size;
	paused=1;
	won=0;
	lost=0;

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
static void player_jump(void);

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
static void setTopCamera(void);     //funkcija za setovanje kamere odozgo (2d prikaz)

int main(int argc, char **argv)
{

    new_game();
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE |GLUT_DEPTH);

    setTopCamera();

    /* Kreira se prozor. */
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition((1920-1280)/2, (1080-720)/2);
    glutCreateWindow("Pacman");


    /* Registruju se callback funkcije. */
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
	if(follow!=1)gluLookAt(positionX,positionY,positionZ,tarX,tarY,tarZ,norm);
	if(follow==1) gluLookAt(player.x,player.y-3*square,4.5*square,player.x,player.y,0, norm);

	sprintf(remains,"Coins remaining: %d",player.remaining);
	sprintf(numLives,"Lives: %d",player.lives);
    drawBitmapText(remains,17*square,1*square+1,0);
    drawBitmapText(numLives,17*square,1*square-1,0);

    if(paused==1 && won!=1){
    if(lost!=1){
    drawBigTextY(pauseMsg1,17*square,12*square,0);
    drawBigTextY(pauseMsg2,17*square,12*square-4,0);
    }
    drawBigText("Pacman moves by pressing",17*square,8*square,0);
    drawBigText("'I' - Up",19*square,7*square,0);
    drawBigText("'J'- Left     'K'- Down     'L'- Right",15.5*square,6*square,0);
    drawBigText("You can restart the game",17*square,15*square,0);
    drawBigText("By pressing 'N'",17*square,14*square,0);
    drawBigText("Camera controls: ",-10*square,12*square,0);
    drawBigText("Top down(2d)- 'R' ",-10*square,11*square,0);
    drawBigText("Follow player(3d)- 'V' ",-10*square,10*square,0);

    }
    if(paused==1 && won>=1){
        player_jump();
        drawBigTextY("Congratulations, you win",17*square,12*square,0);
        drawBigTextY("Press 'N' to restart the game",17*square,12*square-4,0);
    }

    if(paused==1 && lost==1){
        drawBigTextY("Bad luck, you lost",17*square,12*square,0);
        drawBigTextY("Press 'N' to restart the game",17*square,12*square-4,0);
    }

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

    if(pausable>0)pausable--;

	//keypresses
	if (keystates['i'] || keystates['I']) player.dir='u';
    if (keystates['k'] || keystates['K']) player.dir='d';
	if (keystates['j'] || keystates['J']) player.dir='l';
	if (keystates['l'] || keystates['L']) player.dir='r';
	if (keystates[27]) exit(0);
    if (keystates['n'] || keystates['N']) new_game();
    if (keystates['x'] || keystates['X']) player.immune=5.0;
    if (keystates['p'] || keystates['P']) pause();

    //camera controls
    if (keystates['w'] || keystates['W']) positionY+=0.1;
    if (keystates['a'] || keystates['A']) positionX-=0.1;
    if (keystates['s'] || keystates['S']) positionY-=0.1;
    if (keystates['d'] || keystates['D']) positionX+=0.1;

    if (keystates['f'] || keystates['F']) tarX-=0.1;
    if (keystates['g'] || keystates['G']) tarY-=0.1;
    if (keystates['h'] || keystates['H']) tarX+=0.1;
    if (keystates['t'] || keystates['T']) tarY+=0.1;

    if (keystates['4']) positionZ-=0.1;
    if (keystates['5']) positionZ+=0.1;

    if (keystates['r'] || keystates['R']){setTopCamera();follow=0;}
    if (keystates['v'] || keystates['V'])follow=1;

    if(keystates['m'])printf("\n%f %f %f, %f %f %f\n",positionX,positionY,positionZ,tarX,tarY,tarZ);

    if(paused==0){
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
	glTranslatef(player.x,player.y,player.z);
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
	//printf("Moving right position :%.2f,%.2f by :%f \n",player.x, player.y, player.velocity);
	if((vrednost=collision(player.x,player.y))) if((int)((player.y+player.size*0.8)/square) == (int)((player.y-player.size*0.8)/square))player.x+=player.velocity;

}

static void moveLeft(void){

	int vrednost;
	//printf("Moving left position :%.2f,%.2f by :%f \n",player.x, player.y, player.velocity);
	if((vrednost=collision(player.x,player.y))) if((int)((player.y+player.size*0.8)/square) == (int)((player.y-player.size*0.8)/square))player.x-=player.velocity;

}

static void moveUp(void){

    int vrednost;
	//printf("Moving up position :%.2f,%.2f by :%f \n",player.x, player.y, player.velocity);
	if((vrednost=collision(player.x,player.y))) if((int)((player.x+player.size*0.8)/square) == (int)((player.x-player.size*0.8)/square))player.y+=player.velocity;

}

static void moveDown(void){
    int vrednost;
	//printf("Moving down position :%.2f,%.2f by :%f \n",player.x, player.y, player.velocity);
	if((vrednost=collision(player.x,player.y))) if((int)((player.x+player.size*0.8)/square) == (int)((player.x-player.size*0.8)/square))player.y-=player.velocity;

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
            eat_a_coin();
            if(player.remaining==0)win();
            break;

        case 3:
            map[x][y]=1;
            immunity(3);
            break;

        case 4:
            if(x==0) player.x=14*square+square/3;
            if(x==14) player.x=square/1.5;
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
    pause();
    lost=1;
}

static void win(void){
    pause();
    won=1;
    jump_animation=4;
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
                player.immune=1.0;
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
    //printf("\n findmove called ghost%d \n",ghost->id);
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

static void pause(void){
    if(pausable<=0){
        if(paused==1){paused=0; pausable=30;follow=1;
        }
        else {paused=1; pausable=30; follow=0;}
    }
}

static void setTopCamera(void){
    positionX=topX;
    positionY=topY;
    positionZ=topZ;
    tarX=top1;
    tarY=top2;
    tarZ=top3;
}

static void player_jump(void){
    if(jump_animation>=0){
        player.x=19*square;
        player.y=9*square;
        player.size=2.5;
        if(jump_animation>3.0 || (jump_animation<2.0 && jump_animation>1.0)){player.z+=0.1;jump_animation-=1.0/60;}
        else{player.z-=0.1;jump_animation-=1.0/60;}
        }
    else jump_animation=4;
    }
