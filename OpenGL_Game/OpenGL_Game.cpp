// GLUT/Open_GL Sprite
#include "stdafx.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <freeglut.h>
#include <cstdlib>
#include <iostream>
using namespace std;

// Function prototype for loading texture method

GLuint Tex1;
GLuint Tex2;
GLuint glmLoadTextureBMP(char *);

// Global variables

int frame = 1; 

bool wkey = false;
bool skey = false;
bool akey = false;
bool dkey = false;

static void key_down(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
			wkey = true;
			break;
		case 's': 
			skey = true;
			break;
		case 'a': 
			akey = true;
			break;
		case 'd': 
			dkey = true;
			break;
		case 'q': glutLeaveMainLoop () ; break; // (Stop!)
		default: break;
	}
} 

static void key_up(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
			wkey = false;
			break;
		case 's': 
			skey = false; 
			break;
		case 'a': 
			akey = false;
			break;
		case 'd': 
			dkey = false;
			break;
		default: break;
	}

}

static void car(float x, float y, float ang, GLuint tex) {
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	float ox = x + 16;
	float oy = y + 30;
	
	glTranslatef(ox, oy, 0.0); // 3. Translate to the object's position.
	glRotatef(ang, 0.0, 0.0, 1.0); // 2. Rotate the object.
	glTranslatef(-ox, -oy, 0.0); // 1. Translate to the origin.
	glBegin(GL_QUADS);
		glTexCoord2f(frame / 19.0, 0);
		glVertex2i(x, y);
		glTexCoord2f((frame + 1) / 19.0, 0);
		glVertex2i(x + 32, y);
		glTexCoord2f((frame + 1) / 19.0, 1);
		glVertex2i(x + 32, y + 60);
		glTexCoord2f(frame / 19.0, 1);
		glVertex2i(x, y + 60);
	glEnd();
	glPopMatrix();

}

float px = 100;
float py = 100;
float theta = 0;
float speed = 0;

static void car_control()
{
	// FORWARDS
	if (wkey == true)
	{
		speed += 0.05;
		if (speed > 0.5)
		{
			speed = 0.8;
		}
	}

	// BACKWARDS
	if (skey == true)
	{
		speed -= 0.05;
		if (speed < -0.5)
		{
			speed = -0.8;
		}
	}

	// SPEED  DAMPENER ZEROER
	if (speed > -0.01 && speed < 0.01)
	{
		speed = 0;
	}
	
	if (speed != 0 && wkey == false || skey == false)
	{
		speed *= 0.992;
	}

	// TURN LEFT
	if (akey == true)
	{
		if (speed > 0.2)
		{
			theta += 0.7;
		}
		else if (speed < -0.2)
		{
			theta -= 0.7;
		}
	}

	// TURN RIGHT
	if (dkey == true)
	{
		if (speed > 0.2)
		{
			theta -= 0.7;
		}
		else if (speed < -0.2) 
		{
			theta += 0.7;
		}
	}

	if (theta >= 360)
	{
		theta = 0;
	}

	if (speed != 0) {
		float rot = theta * M_PI / 180;
		
		py += cos(rot) * speed;
		px -= sin(rot) * speed;
	}
}

static void display(void)
{
	

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);     // Enable use of texture uv mapping
	glDisable(GL_DEPTH_TEST);    // Depth testing not required (2D only 1 sprite)
	glDisable(GL_LIGHTING);   // Do not include lighting (yet) 
	glEnable(GL_BLEND);       // Enable Alpha blending of textures
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	car(px, py, theta, Tex2);

	glutSwapBuffers();
	
}

GLuint glmLoadTextureBMP(char * fname)  
{
    wchar_t* wString=new wchar_t[256]; // Convert char[] string to LPSTR/wchar
    MultiByteToWideChar(CP_ACP, 0, fname, -1, wString, 256);
    HANDLE hBitMap = LoadImage(0,wString, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);  
    
    BITMAP bitmap;
    GetObject(hBitMap,sizeof(BITMAP),&bitmap);
    int size = bitmap.bmHeight*bitmap.bmWidth*((bitmap.bmBitsPixel/8)+1);  	
    BYTE *lpBits = new BYTE[ size ];
    int PaddedWidth = (bitmap.bmWidth  + 3) & (~3); // Round width up to next multiple of 4
    GetBitmapBits((HBITMAP)hBitMap,size,lpBits ); 
    BYTE r,g,b,a;
    int templ,tempr;
    
    for(int j=0;j<size;j+=4) // Magenta RGBA=(255,0,255) tunrs transparent
    {
	if((lpBits[j+2]==255)&&(lpBits[j+1]==0)&&(lpBits[j]==255))  
	{
	 /*Red*/lpBits[j+0]=0; /*Green*/lpBits[j+1]=0; /*Blue*/lpBits[j+2]=0; /*Alpha*/lpBits[j+3]=0; 
	}
	else
	{
        // Reverse BGRA to RGBA and force A=255 (Alpha 100%)
	/*Red*/r=lpBits[j+0]; /*Green*/g=lpBits[j+1]; /*Blue*/b=lpBits[j+2]; /*Alpha*/a=lpBits[j+3]; 
	a=255; // 100% of the sprite (0% of the background)
	/*Red*/lpBits[j+0]=b; /*Green*/lpBits[j+1]=g; /*Blue*/lpBits[j+2]=r; /*Alpha*/lpBits[j+3]=a;
	}
    }

    BYTE rgb;  // Flip texture vertical (inefficient but only done on initalisation)
    for(int j=0;j<bitmap.bmHeight/2;j++)   
    {
	for(int i=0;i<PaddedWidth;i++)
	{
	  for(int k=0;k<4;k++)
	  {
             templ=4*(i+(j*PaddedWidth));    // Address of pixel at top
	     tempr=4*(i+((bitmap.bmHeight-j-1)*PaddedWidth));	// Address of pixel at bottom
	     rgb=lpBits[tempr+k];								
             lpBits[tempr+k]=lpBits[templ+k];
             lpBits[templ+k]=rgb;
	  }
	}
    }
    GLuint textureID;
    glGenTextures(1, &textureID);  // Create 1 texture
    // "Bind" the newly created texture address with an ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Turn on and configure texture mapping, texture copied into OpenGL/Graphics card 
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,bitmap.bmWidth,bitmap.bmHeight,0,GL_RGBA,GL_UNSIGNED_BYTE, (GLvoid *) lpBits);
    // Magnification filter (texel larger than the pixel)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Minification filter (texel smaller than the pixel) _
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // Free system memory used to store texture (it is now been transfered to the graphics card)
    delete []lpBits;
    return textureID;
}

static void idle(int v)
{
	car_control();
	glutPostRedisplay();
	glutTimerFunc(10, idle, 0);
}

int _tmain(int argc, char** argv)      // Entry point of program
{
	glutInit(&argc, argv);         // Start glut
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);  // Create a single buffer with data in RGB format
	glutInitWindowSize(640,480);   // Open a window at (10,10) with size (640x480) called 
	glutInitWindowPosition(10,10); 
	glutCreateWindow("Sprite based game");
	glLoadIdentity(); 
	glMatrixMode(GL_PROJECTION);

	gluOrtho2D(0.0, 640.0, 0.0, 480.0);  // Map OpenGL to Screen crds 1:1, ignore Z, 2D (X,Y)
	Tex1 = glmLoadTextureBMP("Spritesheet.bmp");
	Tex2 = glmLoadTextureBMP("Spritesheet2.bmp");
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(key_down); 
	glutKeyboardUpFunc(key_up);
	//glutIdleFunc(idle);
	glutTimerFunc(10, idle, 0);
	glutDisplayFunc(display);
	glutMainLoop();			    // Start Glut main loop, exit via break
	return 0;
}
