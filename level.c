#include "level.h"
#include <GL/glut.h>

void draw_walls(void){
	//draw ground
	glPushMatrix();
		glBegin(GL_POLYGON);
		glColor3f(1.0,1.0,0.0);
		glVertex3f(1*square,0,0);
        glVertex3f(1*square,17*square,0);
        glVertex3f(14*square,17*square,0);
        glVertex3f(14*square,0,0);
		glEnd();
	glPopMatrix();

	
    //draw boundries
    
    //vertikalne
	glPushMatrix();
		glColor3f(wallcolor);
		glTranslatef(square/2,17.0/2*square,square/2);
		glScalef(square,17*square,square);
		glutWireCube(1);
	glPopMatrix();
    
    glPushMatrix();
		glColor3f(wallcolor);
		glTranslatef(14*square+square/2,17.0/2*square,square/2);
		glScalef(square,17*square,square);
		glutWireCube(1);
	glPopMatrix();
    
    //horizontalne
    glPushMatrix();
		glTranslatef(15.0/2*square,-square/2,square/2);
		glRotatef(90,0,0,1);
		glScalef(square,15*square,square);
		glutWireCube(1);
	glPopMatrix();
    
    
    glPushMatrix();
		glTranslatef(15.0/2*square,17*square+square/2,square/2);
		glRotatef(90,0,0,1);
		glScalef(square,15*square,square);
		glutWireCube(1);
	glPopMatrix();
    
    
}

void draw_coins(void){
    
}


