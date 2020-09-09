#include "level.h"
#include <GL/glut.h>

void draw_ground(void){
	//draw ground
	glPushMatrix();
		glBegin(GL_POLYGON);
		glColor3f(1.0,1.0,1.0);
		glVertex3f(1*square,0,0);
        glVertex3f(1*square,18*square,0);
        glVertex3f(14*square,18*square,0);
        glVertex3f(14*square,0,0);
		glEnd();
	glPopMatrix();

	
    //draw boundries
    
    //vertikalne
    /*
	glPushMatrix();
		glColor3f(wallcolor);
		glTranslatef(square/2,17.0/2*square+square,square/2);
		glScalef(square,17*square,square);
		glutSolidCube(1);
	glPopMatrix();
    
    glPushMatrix();
		glColor3f(wallcolor);
		glTranslatef(14*square+square/2,17.0/2*square+square,square/2);
		glScalef(square,17*square,square);
		glutSolidCube(1);
	glPopMatrix();
    
    //horizontalne
    
    glPushMatrix();
		glTranslatef(15.0/2*square,square/2,square/2);
		glRotatef(90,0,0,1);
		glScalef(square,15*square,square);
		glutSolidCube(1);
	glPopMatrix();
    
    
    glPushMatrix();
		glTranslatef(15.0/2*square,18*square+square/2,square/2);
		glRotatef(90,0,0,1);
		glScalef(square,15*square,square);
		glutSolidCube(1);
	glPopMatrix();
    */
    
}

void draw_coins(int i, int j){
                    glPushMatrix();
                    glColor3f(coincolor);
                    glTranslatef(square*i+square/2,square*j+square/2,1);
                    glutSolidCone(1, 1 ,30, 30);
                glPopMatrix();
                
                glPushMatrix();
                    glColor3f(coincolor);
                    glTranslatef(square*i+square/2,square*j+square/2,1);
                    glRotatef(180,0,1,0);
                    glutSolidCone(1, 1 ,30, 30);;
                glPopMatrix();
}

void draw_supercoins(int i, int j){
                    glPushMatrix();
                    glColor3f(1.0,0.2,0.2);
                    glTranslatef(square*i+square/2,square*j+square/2,1);
                    glutSolidCone(1, 1 ,30, 30);
                glPopMatrix();
                
                glPushMatrix();
                    glColor3f(1.0,0.2,0.2);
                    glTranslatef(square*i+square/2,square*j+square/2,1);
                    glRotatef(180,0,1,0);
                    glutSolidCone(1, 1 ,30, 30);;
                glPopMatrix();
}

void draw_block(int i, int j){
                glPushMatrix();
                    glColor3f(wallcolor);
                    glTranslatef(square*i+square/2,square*j+square/2,square/2);
                    glScalef(square,square,square);
                    glutSolidCube(1);
                glPopMatrix();
}

void draw_teleport(int i, int j){
                glPushMatrix();
                    glColor3f(0.3,0.3,0.3);
                    glTranslatef(square*i+square/2,square*j+square/2,square/2);
                    glScalef(square,square,square);
                    glutSolidCube(1);
                glPopMatrix();
}
