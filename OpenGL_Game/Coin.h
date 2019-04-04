#pragma once
#include "stdafx.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <freeglut.h>
#include <cstdlib>
#include <iostream>
using namespace std;

class Coin
{
	public: float x;
	public: float y;
	public: GLuint tex;
public: float speed = 0;

	public: void Coin::create(float x, float y, GLuint tex);
	public: void Coin::draw();
};

void Coin::create(float x_, float y_, GLuint tex_)
{
	x = x_;
	y = y_;
	tex = tex_;


}

void Coin::draw()
{
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTranslatef(0, 16, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
				glVertex2i(x, y);
			glTexCoord2f(1, 0);
				glVertex2i(x+16, y);
			glTexCoord2f(1, 1);
				glVertex2i(x+16, y+16);
			glTexCoord2f(0, 1);
				glVertex2i(x, y+16);
		glEnd();
	glPopMatrix();

}