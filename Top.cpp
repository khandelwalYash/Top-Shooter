#include <iostream>
#include <stdlib.h>

#include <GL/glut.h>

#include "Top.h"

Top::Top()
{
 	lattu_x = 15;
 	lattu_y = -1;
 	lattu_z = 29;

	vel_x = vel_y = vel_z = 0;
	power_length = 0;
	shoot = 0;
	flag=0;
}

void Top::put_friction()
{
	vel_x*=0.999;
	vel_z*=0.999;


	if(vel_x<0.00001)
			vel_x=0;
	if(vel_z<0.00001)
			vel_z=0;


}

void Top::draw_arrow()
{
		glPushMatrix();
		glScalef(5,5,5);
		glTranslatef(-3,1,5.5);
		glutSolidCone(0.2, 0.4, 10, 2);
		glRotatef(90,1,0,1);
		glPopMatrix();
		glPopMatrix();
}

void Top::draw_top()
{

		glPushMatrix();
		glColor3f(0.3, 0.5, 0.112);
		glutSolidTorus( 0.3,0.5, 100,100);

		glPushMatrix();
		glTranslatef(0,0,-0.3);
		glRotatef(180,0,1,0);
		glutSolidCone(0.2, 0.4, 10, 2);
		glPopMatrix();

		glColor3f(1.0,1.0,0.0);
		glutSolidTorus(0.1,0.8,25,30);
		glColor3f(0.2,0.6,0.4);
		glutWireTorus( 0.1,0.8, 15, 60);

		glColor3f(0.4,0.11,0.25);

		glPushMatrix();
		glTranslatef(0,0,-0.3);
		glutSolidTorus( 0.2,0.2, 25, 30);
		glColor3f(0.112,0.235,0.154);
		glutWireTorus( 0.2,0.2, 15, 60);
		glPopMatrix();

		for(float i=45;i<90;i++)
		{
			glPushMatrix();
			glTranslatef( 0.0 , 0 ,0.5);
			glRotatef(i,0,0,1);
			glRotatef(90,0,1,0);
			glColor3f(0.0, 0.0, 0.0); 
			glScalef(1.2 , 0.2, 0.2);
			glutSolidCube(1.0);
			glColor3f(0.9, 0.8, 0.0); 
			glutWireCube(1.0);
			glPopMatrix();
		}

		glPopMatrix();
}

void Top::power_level(float velocity){
	

	if(velocity>10)
		velocity=10;
	glScalef(1.2,1.2,1.2);
	glTranslatef(-9,1.2f,4.2f);
	glRotatef(270,0,1,0);
	glRotatef(90,1,0,0);
	glScalef(0.6,0.6,0.6);
	for ( float i = 0; (i < velocity) ; i++)
	{
		glColor3f(1.0, 1-i/velocity, 0.0);
		glTranslatef(0, 0, -0.5);
		drawCylinder(0.7);
	}

}

void Top::drawCylinder(float rad){

	for (int i = 0; i < 20; ++i)
	{
		glTranslatef(0,0,-0.05);
	   	glutSolidTorus( 0.3,0.5, 10,10);
	}
	
}

int Top::checkBoundary(float x, float z){

	if (x > 2 && x < 60 && z > 2 && z < 60)
		return 1;
	else
		return 0;
}

