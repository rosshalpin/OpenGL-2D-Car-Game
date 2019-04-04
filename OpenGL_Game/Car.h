#pragma once
#include "stdafx.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <freeglut.h>
#include <cstdlib>
#include <iostream>

class Car
{
	public: GLuint tex;
	public: float x;
	public: float y;
	public: float theta;
	public: float speed;
	public: bool wkey;
	public: bool skey;
	public: bool akey;
	public: bool dkey;

	public: int frame = 1;

	public: void Car::create(float x_, float y_, float theta_, float speed_, GLuint tex_);
	public: void Car::draw();
	public: void Car::key_down(unsigned char key, int x, int y);
	public: void Car::key_up(unsigned char key, int x, int y);
	public: void Car::car_control();
};

void Car::create(float x_, float y_, float theta_, float speed_, GLuint tex_)
{
	tex = tex_;
	x = x_;
	y = y_;
	theta = theta_;
	speed = speed_;
	wkey = false;
	skey = false;
	akey = false;
	dkey = false;
}

void Car::draw()
{
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	float ox = x + 16;
	float oy = y + 30;

	glTranslatef(ox, oy, 0.0); // 3. Translate to the object's position.
	glRotatef(theta, 0.0, 0.0, 1.0); // 2. Rotate the object.
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

void Car::car_control()
{
	// FORWARDS
	if (wkey == true)
	{
		speed += 0.1;
		if (speed > 1)
		{
			speed = 1;
		}
	}

	// BACKWARDS
	if (skey == true)
	{
		speed -= 0.1;
		if (speed < -1)
		{
			speed = -1;
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

		y += cos(rot) * speed;
		x -= sin(rot) * speed;
	}
}

void Car::key_down(unsigned char key, int x, int y)
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
	case 'q': glutLeaveMainLoop(); break; // (Stop!)
	default: break;
	}
}

void Car::key_up(unsigned char key, int x, int y)
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