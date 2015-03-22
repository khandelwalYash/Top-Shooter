	#include <iostream>
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>

	#include <GL/glut.h>

	#include <cmath>
	#include "imageloader.h"
	#include "Top.h"
	#include "vec3f.h"
	#define pi 3.14
	using namespace std;


class Terrain {

private:
		int w; //Width
		int l; //Length
		float** hs; //Heights
		Vec3f** normals;
		bool computedNormals; //Whether normals is up-to-date

public:
		Terrain(int w2, int l2);
		~Terrain();
		int width();
		int length(); 
		void setHeight(int x, int z, float y);
		float getHeight(int x, int z);
		void computeNormals();
		Vec3f getNormal(int x, int z);
	
};
//Represents a terrain, by storing a set of heights and normals at 2D locations

Terrain::Terrain(int w2, int l2) {
	w = w2;
	l = l2;
	
	hs = new float*[l];
	for(int i = 0; i < l; i++) {
		hs[i] = new float[w];
	}
	
	normals = new Vec3f*[l];
	for(int i = 0; i < l; i++) {
		normals[i] = new Vec3f[w];
	}
	
	computedNormals = false;
}
		
Terrain::~Terrain() {
	for(int i = 0; i < l; i++) {
		delete[] hs[i];
	}
	delete[] hs;
	
	for(int i = 0; i < l; i++) {
		delete[] normals[i];
	}
	delete[] normals;
}

int Terrain::width() {
	return w;
}
		
int Terrain::length() {
	return l;
}
		
		//Sets the height at (x, z) to y
void Terrain::setHeight(int x, int z, float y) {
	hs[z][x] = y;
	computedNormals = false;
}

//Returns the height at (x, z)
float Terrain::getHeight(int x, int z) {
	return hs[z][x];
}

//Computes the normals, if they haven't been computed yet
void Terrain::computeNormals() {
	if (computedNormals) {
		return;
	}
	
	//Compute the rough version of the normals
	Vec3f** normals2 = new Vec3f*[l];
	for(int i = 0; i < l; i++) {
		normals2[i] = new Vec3f[w];
	}
	
	for(int z = 0; z < l; z++) {
		for(int x = 0; x < w; x++) {
			Vec3f sum(0.0f, 0.0f, 0.0f);
			
			Vec3f out;
			if (z > 0) {
				out = Vec3f(0.0f, hs[z - 1][x] - hs[z][x], -1.0f);
			}
			Vec3f in;
			if (z < l - 1) {
				in = Vec3f(0.0f, hs[z + 1][x] - hs[z][x], 1.0f);
			}
			Vec3f left;
			if (x > 0) {
				left = Vec3f(-1.0f, hs[z][x - 1] - hs[z][x], 0.0f);
			}
			Vec3f right;
			if (x < w - 1) {
				right = Vec3f(1.0f, hs[z][x + 1] - hs[z][x], 0.0f);
			}
			
			if (x > 0 && z > 0) {
				sum += out.cross(left).normalize();
			}
			if (x > 0 && z < l - 1) {
				sum += left.cross(in).normalize();
			}
			if (x < w - 1 && z < l - 1) {
				sum += in.cross(right).normalize();
			}
			if (x < w - 1 && z > 0) {
				sum += right.cross(out).normalize();
			}
			
			normals2[z][x] = sum;
		}
	}
	
	//Smooth out the normals
	const float FALLOUT_RATIO = 0.5f;
	for(int z = 0; z < l; z++) {
		for(int x = 0; x < w; x++) {
			Vec3f sum = normals2[z][x];
			
			if (x > 0) {
				sum += normals2[z][x - 1] * FALLOUT_RATIO;
			}
			if (x < w - 1) {
				sum += normals2[z][x + 1] * FALLOUT_RATIO;
			}
			if (z > 0) {
				sum += normals2[z - 1][x] * FALLOUT_RATIO;
			}
			if (z < l - 1) {
				sum += normals2[z + 1][x] * FALLOUT_RATIO;
			}
			
			if (sum.magnitude() == 0) {
				sum = Vec3f(0.0f, 1.0f, 0.0f);
			}
			normals[z][x] = sum;
		}
	}
	
	for(int i = 0; i < l; i++) {
		delete[] normals2[i];
	}
	delete[] normals2;
	
	computedNormals = true;
}

//Returns the normal at (x, z)
Vec3f Terrain::getNormal(int x, int z) {
	if (!computedNormals) {
		computeNormals();
	}
	return normals[z][x];
}

	int target[5][2] = {
					{30,30},
					{45,30},
					{30,10},
					{20,50},
					{50,10}

				};
	Top top;
	int Score = 0;
	int cam_pos = 1;
	int count = 0;
	int flag=0;
	int flag_key = 2;
	float _angle = 60.0f;
	Terrain* _terrain;
	float rotate_angle=0.0f;
	int windowWidth,windowHeight;
	Terrain* loadTerrain(const char* filename, float height) {
	Image* image = loadBMP(filename);
	Terrain* t = new Terrain(image->width, image->height);
	for(int y = 0; y < image->height; y++) {
		for(int x = 0; x < image->width; x++) {
			unsigned char color =
				(unsigned char)image->pixels[3 * (y * image->width + x)];
			float h = height * ((color / 255.0f) - 0.5f);
			t->setHeight(x, y, h);
		}
	}

	delete image;
	t->computeNormals();
	return t;
	}

	void cleanup() {
	delete _terrain;
	}

void printNumber(int sc) { 
					char string[10]; 
					strcpy(string,"Score: "); 
					glColor3f(1.0,1.0,0.0); 
					glRasterPos3f(0,0,5); 
					for(int j=0;j<8;j++) 
						glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[j]);
					glColor3f(0,0,0); 
					if(sc==0) 
						return; 
					printNumber(sc/10); 
					glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, '0'+sc%10); }
				

void my_Reset()
{
	top.shoot=0;
	top.vel_z = top.vel_y = top.vel_x =0;
	top.lattu_x = 2; top.lattu_z = 2;

	}


void motionTop(){

	if(top.shoot == 1 && top.lattu_z > 2 && top.lattu_z < 59){
		top.lattu_z = top.lattu_z - top.vel_z*0.05;
		top.lattu_z = top.lattu_z - top.vel_z*0.05;
	}
	else if(top.shoot == 1 && top.lattu_z <= 2){
		top.lattu_z = 3;
		top.vel_z = top.vel_z*(-1);
	}
	else if(top.shoot == 1 && top.lattu_z >= 59){
		top.lattu_z = 58;
		top.vel_z = top.vel_z*(-1);
	}

	top.lattu_x = top.lattu_z - top.vel_x*0.05;

	if(top.shoot == 1 && top.lattu_x > 2 && top.lattu_x < 59){
		top.lattu_x = top.lattu_x - top.vel_x*0.05;
	}
	else if( top.shoot == 1 && top.lattu_x <= 2){
		top.lattu_x = 3;
		top.vel_x = top.vel_x*(-1);
	}
	else if(top.shoot == 1 && top.lattu_z >= 59){
		top.lattu_x = 58;
		top.vel_x = top.vel_x*(-1);
	}


	}

	void handleKeypress(unsigned char key, int x, int y) {
		switch (key) {
			case 27: //Escape key
				cleanup();
				exit(0);
			
				 
		}
		if(key > 50)
		flag=1-flag;
		if(key == '1')
			flag_key = 1;
		if(key == '2')
			flag_key = 2;
		if(key == '3')
			flag_key = 3;
		if(key == '4')
			flag_key = 4;
		if(key == '5')
			flag_key = 5;

		if (key == 32 ) //SPACE key
			top.shoot = 1;
			


			if(key == 'z') 
			cam_pos+=1;
			if(key == 'x')
			cam_pos-=5;


	}



void handleKeypress2(int key, int x, int y) {


if (key == GLUT_KEY_UP)
    top.power_length=top.power_length+1;
if (key == GLUT_KEY_DOWN)
	top.power_length= top.power_length-1;

if( key == GLUT_KEY_LEFT && top.shoot==0)
	top.lattu_z+=1;
if(key == GLUT_KEY_RIGHT && top.shoot==0)
	top.lattu_z-=1;

if(top.lattu_z>=58)
		top.lattu_z=58;

if(top.lattu_z<=2)
		top.lattu_z=2;
/*
	if(top.shoot == 0){
	    if (key == GLUT_KEY_LEFT && top.lattu_x > 3)
	        top.lattu_z = top.lattu_x - 0.3;
	    if (key == GLUT_KEY_RIGHT && top.lattu_x < 57) 
	        top.lattu_z = top.lattu_z + 0.3;
	    /*if (key == GLUT_KEY_UP && posZ > 3)
	        posZ = posZ - 0.3;
	    if (key == GLUT_KEY_DOWN && posZ < 57) 
	        posZ = posZ + 0.3;
	      */  

	    /*
	    if (key == GLUT_KEY_UP && top.vel_z < 8)
	        velZ = velZ + 1;
	    if (key == GLUT_KEY_DOWN && top.vel_z > 1)
	        velZ = velZ - 1;
	    posY = _terrain->getHeight(posX, posZ)+2;
	    */
	

	//}
}

void drawTarget(){ glPushMatrix(); 
					glColor3f(1.0,0.0,0.2); 
					  	glutSolidTorus( 0.8, 1.6, 100,100);
				 	  	 glPopMatrix(); 
				 	  	}

	void initRendering() {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_NORMALIZE);
		glShadeModel(GL_SMOOTH);
	}

	void handleResize(int w, int h) {
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
	}


	void drawScene() {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
     	glTranslatef(0.0f, 0.0f, -6.0f);
			     	
			     	glPushMatrix();
					glTranslatef(-1,1.1,-2);
					printNumber(Score);
					glPopMatrix();
					
			

		GLfloat ambientColor[] = {1.4f, 0.4f, 0.4f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
		
		GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
		GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
		
		float scale = 5.0f / max(_terrain->width() - 1, _terrain->length() - 1);

		if(flag_key == 1)
				{
					gluLookAt(
						0,1,0,
						0,-1,0,
						0,0,-1);
				}
				else if(flag_key == 2)
				{
					gluLookAt(
						cam_pos-4,2,0,
						1,0,0,
						0,1,0);
					
				}
				else if(flag_key == 3)
				{
					//glTranslatef((float)(_terrain->width() - 1) / 2,0,(float)(_terrain->length() - 1) / 2);
					gluLookAt(
						(top.lattu_x -30)*scale, _terrain->getHeight(top.lattu_x,top.lattu_z)*scale + 2 , (top.lattu_z-30)*scale,
						(target[count][0]-30)*scale, _terrain->getHeight(target[count][0],target[count][1])*scale, (target[count][1]-30)*scale,
						0,1,0
						);
				}
				else if(flag_key == 4)
				{
					//glTranslatef((float)(_terrain->width() - 1) / 2,0,(float)(_terrain->length() - 1) / 2);
					gluLookAt(
						(top.lattu_x -30)*scale - 0.5, _terrain->getHeight(top.lattu_x,top.lattu_z)*scale + 2 , (top.lattu_z-30)*scale,
						(top.lattu_x -30)*scale, _terrain->getHeight(top.lattu_x,top.lattu_z)*scale + 2 , (top.lattu_z-30)*scale,
						0,1,0
						);
				}
				
				else
				{
					gluLookAt(
						0,cam_pos,0,
						0,-1,0,
						0,0,-1);

				}

				
		glScalef(scale, scale, scale);

		glTranslatef(-(float)(_terrain->width() - 1) / 2,
					 0.0f,
					 -(float)(_terrain->length() - 1) / 2);
//	glTranslatef((float)(_terrain->width() - 1) / 2,0,(float)(_terrain->length() - 1) / 2);
		
	
			
		for(int z = 0; z < _terrain->length() - 1; z++) {
			glColor3f(0,0.4,0);
			//Makes OpenGL draw a triangle at every three consecutive vertices
			glBegin(GL_TRIANGLE_STRIP);
			for(int x = 0; x < _terrain->width(); x++) {
				Vec3f normal = _terrain->getNormal(x, z);
				glNormal3f(normal[0], normal[1], normal[2]);
				glVertex3f(x, _terrain->getHeight(x, z), z);
				normal = _terrain->getNormal(x, z + 1);
				glNormal3f(normal[0], normal[1], normal[2]);
				glVertex3f(x, _terrain->getHeight(x, z + 1), z + 1);
			}

			glEnd();
			
		}

	Vec3f normal=_terrain->getNormal(top.lattu_x,top.lattu_z).normalize();
		glPushMatrix();
		glTranslatef(target[count][0], _terrain->getHeight(target[count][0],target[count][1]), target[count][1]);
		drawTarget();	
		glPopMatrix();

		glPushMatrix();

	glTranslatef(top.lattu_x, _terrain->getHeight(top.lattu_x,top.lattu_z) , top.lattu_z);
	glRotatef(acos(normal[2])*180/pi,-normal[1],normal[0],0);
	glTranslatef(0,0,2);	
		glScalef(3,3,3);
		glRotatef(_angle,0,0,1);
		top.draw_top();	
		glPopMatrix();


		glPushMatrix();
		top.power_level(top.power_length);
		glPopMatrix();

		glPushMatrix();
		top.draw_arrow();
		glPopMatrix();
	
		glEnd();
		glutSwapBuffers();
	}

	void update(int value) {
		_angle += 0.01f;
		if (_angle > 360) {
			_angle -= 360;
		}
		if(top.shoot)
			top.put_friction();
		//motionTop();
		
		if(top.flag==1)
			if(top.shoot)	
				{top.vel_x=0.1*top.power_length;top.flag=0;}

		rotate_angle+=0.2f;
		if(top.shoot==0)
			{top.flag=1;}
		 
	Vec3f normal=_terrain->getNormal(top.lattu_x,top.lattu_z).normalize();
	float cosa=(-normal[1]);
	float sina=sqrt(1- cosa*cosa);
	top.lattu_x+=-0.01*cosa*normal[0]/2*0.02+top.vel_x,top.lattu_z+=-0.01*cosa*normal[2]/2*0.02+top.vel_z,top.lattu_y+=-0.01*sina*sina/2*0.02+top.vel_y;


	if(top.shoot==0)
	{
	if(top.lattu_x>=58)
		top.lattu_x=57;
	if(top.lattu_x < 2)
		top.lattu_x=1.9;
	}
	else
	{
	
	if(top.lattu_z>58 || top.lattu_z < 2)
		my_Reset();
	if(top.lattu_x>58 || top.lattu_x < 2)
		my_Reset();
	
	}
	if(top.shoot)
		top.vel_x+=-0.01*cosa*normal[0],top.vel_z+=-0.01*cosa*normal[2],top.vel_y+=-0.01*(sina*sina);
	//top.vel_z+=top.power_length*0.01;


				if(fabs(top.lattu_x - target[count][0]) < 3 && fabs(top.lattu_z - target[count][1]) < 3 )
					{

						Score += 10;
						count++;
						if(count == 5)
							count = 0;
						top.vel_x = top.vel_z =0;
						top.shoot =0;

					}	
				
		glutPostRedisplay();
		glutTimerFunc(20, update, 0);
	}

	int main(int argc, char** argv) {
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		windowWidth = glutGet(GLUT_SCREEN_WIDTH);
	    windowHeight = glutGet(GLUT_SCREEN_HEIGHT);
	    glutInitWindowSize(windowWidth, windowHeight);
		glutInitWindowPosition(0, 0);
		
		glutCreateWindow("my top shooter - 201302164");
		initRendering();
		_terrain = loadTerrain("heightmap.bmp", 20);
		glutDisplayFunc(drawScene);
		glutKeyboardFunc(handleKeypress);
		glutSpecialFunc(handleKeypress2);
		glutReshapeFunc(handleResize);

		glutTimerFunc(25, update, 0);
		
		glutMainLoop();
		return 0;
	} 	