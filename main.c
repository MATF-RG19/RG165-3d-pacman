#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>
#include <stdbool.h>

#define PI 3.14159265358979323846	
#define wallcolor 0.282, 0.820, 0.800
#define wallsize 2
#define playercolor 1.000, 0.549, 0.000
#define playersize 5
#define maxright 98.0
#define maxup 98.0


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
static void draw_walls(void); 	//iscrtavanje zidova
static void draw_player(void);	//iscrtavanje pacmana

//struktura za playera
typedef struct Pacman{
float x;		//pozicija po x koordinati
float y;		//pozicija po y koordinati
float velocity;		//trenutna brzina kretanja
int lifes;		//broj zivota
int imune;		//imunitet za debuging 
int eat;		//pacman jede duhove
char direction;}Pacman;	//strana u koju se pacman krece	

Pacman player;		//globalna promenljiva u kojoj se cuvaju podaci o igracu


static void new_game(void){
	//Player inicijalizacija
	player.x=50.0;
	player.y=50.0;
	player.velocity=33.0/60;
	player.lifes=3;
	player.imune=0;
	player.eat=0;
	
}

bool keystates[256];


static void moveUp(void);	//deklaracija funckija kretanja
static void moveLeft(void);
static void moveDown(void);
static void moveRight(void);

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
	gluLookAt(50,-30,72,50,50,0,0,1,0);	
	
	//draw
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
	if (keystates['i'] || keystates['I']) moveUp();
	if (keystates['k'] || keystates['K']) moveDown();
	if (keystates['j'] || keystates['J']) moveLeft();
	if (keystates['l'] || keystates['L']) moveRight();
	if (keystates[27]) exit(0);
}

static void draw_level(void){
	
	draw_walls();
	
	draw_player();
	
	

}

static void draw_player(void){
glPushMatrix();
	glColor3f(playercolor);
	glTranslatef(player.x,player.y,2.5);
	glutSolidSphere(5,10,10);
glPopMatrix();
}

static void draw_walls(void){
	//draw ground
	glPushMatrix();
		glBegin(GL_QUADS);
		glColor3f(1.0,1.0,0.0);
		glVertex3f(100,100,0);
		glVertex3f(0,100,0);
		glVertex3f(0,0,0);
		glVertex3f(100,0,0);		
		glEnd();
	glPopMatrix();

	//draw boundries
	glPushMatrix();
		glColor3f(wallcolor);
		glTranslatef(0,51,0);
		glScalef(1,50,10);
		glutSolidCube(wallsize);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(50,0,0);
		glRotatef(90,0,0,1);
		glScalef(1,51,10);
		glutSolidCube(wallsize);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(50,100,0);
		glRotatef(90,0,0,1);
		glScalef(1,51,10);
		glutSolidCube(wallsize);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(100,51,0);
		glScalef(1,50,10);
		glutSolidCube(wallsize);
	glPopMatrix();
}

static void moveRight(void){

	printf("Moving right position :%.2f,%.2f by :%f \n",player.x,player.y,player.velocity);
	if(player.x<(maxright-playersize)) player.x+=player.velocity;
	player.direction=' ';
}

static void moveLeft(void){

	
	printf("Moving left position :%.2f,%.2f by :%f \n",player.x,player.y,player.velocity);
	if(player.x>playersize) player.x-=player.velocity;
	player.direction=' ';
}

static void moveUp(void){

	printf("Moving up position :%.2f,%.2f by :%f \n",player.x,player.y,player.velocity);
	if(player.y<(maxup-playersize)) player.y+=player.velocity;
	player.direction=' ';
}

static void moveDown(void){

	printf("Moving down position :%.2f,%.2f by :%f \n",player.x,player.y,player.velocity);
	if(player.y>playersize) player.y-=player.velocity;
	player.direction=' ';
}

