// GLUT/Open_GL Sprite
#include "stdafx.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <freeglut.h>
#include <cstdlib>
#include <iostream>
#include "Car.h"
#include "Coin.h"

using namespace std;
// Function prototype for loading texture method
GLuint glmLoadTextureBMP(char *);

GLuint glmLoadTextureBMP(char * fname)
{
	wchar_t* wString = new wchar_t[256]; // Convert char[] string to LPSTR/wchar
	MultiByteToWideChar(CP_ACP, 0, fname, -1, wString, 256);
	HANDLE hBitMap = LoadImage(0, wString, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	BITMAP bitmap;
	GetObject(hBitMap, sizeof(BITMAP), &bitmap);
	int size = bitmap.bmHeight*bitmap.bmWidth*((bitmap.bmBitsPixel / 8) + 1);
	BYTE *lpBits = new BYTE[size];
	int PaddedWidth = (bitmap.bmWidth + 3) & (~3); // Round width up to next multiple of 4
	GetBitmapBits((HBITMAP)hBitMap, size, lpBits);
	BYTE r, g, b, a;
	int templ, tempr;

	for (int j = 0; j < size; j += 4) // Magenta RGBA=(255,0,255) tunrs transparent
	{
		if ((lpBits[j + 2] == 255) && (lpBits[j + 1] == 0) && (lpBits[j] == 255))
		{
			/*Red*/lpBits[j + 0] = 0; /*Green*/lpBits[j + 1] = 0; /*Blue*/lpBits[j + 2] = 0; /*Alpha*/lpBits[j + 3] = 0;
		}
		else
		{
			// Reverse BGRA to RGBA and force A=255 (Alpha 100%)
			/*Red*/r = lpBits[j + 0]; /*Green*/g = lpBits[j + 1]; /*Blue*/b = lpBits[j + 2]; /*Alpha*/a = lpBits[j + 3];
			a = 255; // 100% of the sprite (0% of the background)
			/*Red*/lpBits[j + 0] = b; /*Green*/lpBits[j + 1] = g; /*Blue*/lpBits[j + 2] = r; /*Alpha*/lpBits[j + 3] = a;
		}
	}

	BYTE rgb;  // Flip texture vertical (inefficient but only done on initalisation)
	for (int j = 0; j < bitmap.bmHeight / 2; j++)
	{
		for (int i = 0; i < PaddedWidth; i++)
		{
			for (int k = 0; k < 4; k++)
			{
				templ = 4 * (i + (j*PaddedWidth));    // Address of pixel at top
				tempr = 4 * (i + ((bitmap.bmHeight - j - 1)*PaddedWidth));	// Address of pixel at bottom
				rgb = lpBits[tempr + k];
				lpBits[tempr + k] = lpBits[templ + k];
				lpBits[templ + k] = rgb;
			}
		}
	}
	GLuint textureID;
	glGenTextures(1, &textureID);  // Create 1 texture
	// "Bind" the newly created texture address with an ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Turn on and configure texture mapping, texture copied into OpenGL/Graphics card 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.bmWidth, bitmap.bmHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)lpBits);
	// Magnification filter (texel larger than the pixel)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Minification filter (texel smaller than the pixel) _
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// Free system memory used to store texture (it is now been transfered to the graphics card)
	delete[]lpBits;
	return textureID;
}

// ******************* DEFINE OBJECTS *******************
Car player;
Car NPC;
Car NPC2;
Car NPC3;
Car NPC4;
Car NPC5;

Coin coin1;
Coin coin2;
Coin coin3;
Coin coin4;
Coin coin5;
Coin coin6;
Coin coin7;
Coin coin8;

// ******************* TEXTURES ******************* 
GLuint Tex1;
GLuint Tex2;
GLuint TexTrack;
GLuint coinTex;

// Score data and method
char score_text[20];

static void text(int x, int y, char *string)
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(x, y);
	int len, i;
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]); // Other fonts possible
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);  // Switch back to white
}

static void background() {
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, TexTrack);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex2i(0, 0);
			glTexCoord2f(1, 0);
			glVertex2i(640, 0);
			glTexCoord2f(1, 1);
			glVertex2i(640, 480);
			glTexCoord2f(0, 1);
			glVertex2i(0, 480);
		glEnd();
	glPopMatrix();
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
	background();
	
	// ******************* DRAW OBJECTS *******************
	player.draw();
	NPC.draw();
	NPC2.draw();
	NPC3.draw();
	NPC4.draw();
	NPC5.draw();

	coin1.draw();
	coin2.draw();
	coin3.draw();
	coin4.draw();
	coin5.draw();
	coin6.draw();
	coin7.draw();
	coin8.draw();

	sprintf(score_text, "Score: %d", player.score); // Display text 
	text(530, 30, score_text);

	glutSwapBuffers();
}

static void key_down(unsigned char key, int x, int y)
{
	player.key_down(key, x, y);
};

static void key_up(unsigned char key, int x, int y)
{
	player.key_up(key, x, y);
};

static void idle(int v)
{
	player.car_control();
	
	NPC.car_control();
	NPC2.car_control();
	NPC3.car_control();
	NPC4.car_control();
	NPC5.car_control();
	glutPostRedisplay();
	glutTimerFunc(10, idle, 0);
	
	//NPC.key_down('w', 418, -7);

	// ******************* COLLISIONS *******************
	player.collision(NPC, 30);
	player.collision(NPC2, 30);
	player.collision(NPC3, 30);
	player.collision(NPC4, 30);
	player.collision(NPC5, 30);

	NPC.collision(NPC2, 30);
	NPC.collision(NPC3, 30);
	NPC.collision(NPC4, 30);
	NPC.collision(NPC5, 30);

	NPC2.collision(NPC, 30);
	NPC2.collision(NPC3, 30);
	NPC2.collision(NPC4, 30);
	NPC2.collision(NPC5, 30);

	NPC3.collision(NPC, 30);
	NPC3.collision(NPC2, 30);
	NPC3.collision(NPC4, 30);
	NPC3.collision(NPC5, 30);

	NPC4.collision(NPC, 30);
	NPC4.collision(NPC2, 30);
	NPC4.collision(NPC3, 30);
	NPC4.collision(NPC5, 30);

	NPC5.collision(NPC, 30);
	NPC5.collision(NPC2, 30);
	NPC5.collision(NPC3, 30);
	NPC5.collision(NPC4, 30);

	player.collision(coin1, 18);
	player.collision(coin2, 18);
	player.collision(coin3, 18);
	player.collision(coin4, 18);
	player.collision(coin5, 18);
	player.collision(coin6, 18);
	player.collision(coin7, 18);
	player.collision(coin8, 18);
	
}

void onMouse(int button, int state, int x, int y)
{
	cout <<  x << " " << y << " " << endl;
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
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	Tex1 = glmLoadTextureBMP("Spritesheet3D.bmp");
	Tex2 = glmLoadTextureBMP("Spritesheet23D.bmp");
	TexTrack = glmLoadTextureBMP("track3.bmp");
	coinTex = glmLoadTextureBMP("coin.bmp");
	
	// ******************* CREATE OBJECTS *******************
	player.create(320, 50, 0, 0, Tex1);
	NPC.create(360, 200, 20, 0, Tex2);
	NPC2.create(250, 280, 10, 0, Tex1);
	NPC3.create(150, 280, 0, 0, Tex2);
	NPC4.create(190, 230, 160, 0, Tex1);
	NPC5.create(280, 200, 270, 0, Tex2);

	coin1.create(182, 111, coinTex);
	coin2.create(313, 112, coinTex);
	coin3.create(467, 116, coinTex);
	coin4.create(547, 224, coinTex);
	coin5.create(496, 314, coinTex);
	coin6.create(304, 325, coinTex);
	coin7.create(170, 322, coinTex);
	coin8.create(84, 230, coinTex);


	glutKeyboardFunc(key_down); 
	glutKeyboardUpFunc(key_up);
	glutMouseFunc(onMouse);
	glutTimerFunc(10, idle, 0);
	glutDisplayFunc(display);
	glutMainLoop();			    // Start Glut main loop, exit via break
	return 0;
}