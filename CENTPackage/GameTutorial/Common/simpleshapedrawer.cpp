#include "simpleshapedrawer.h"
#include <QGLWidget>
#include <math.h>

void SimpleShapeDrawer::DrawEmptyCircle(float cx, float cy, float r, int numSegments,const OpenGLColor & color)
{
	DrawCircle(GL_LINE_LOOP, cx, cy, r, numSegments, color);
}

void SimpleShapeDrawer::DrawFilledCircle(float cx, float cy, float r, int numSegments,const OpenGLColor& color)
{
	DrawCircle(GL_TRIANGLE_FAN, cx, cy, r, numSegments, color);
}

void SimpleShapeDrawer::DrawFilledRect(int x, int y, int width, int height, const OpenGLColor& color)
{
	DrawRect(GL_QUADS, x, y, width, height, color);
}

void SimpleShapeDrawer::DrawEmptyRect(int x, int y, int width, int height, const OpenGLColor& color)
{
	DrawRect(GL_LINE_LOOP, x, y, width, height, color);
}

void SimpleShapeDrawer::DrawEmptyTriangle(int x, int y, int height, int width, const OpenGLColor& color)
{
	DrawTriangle(GL_LINE_LOOP, x, y, height, width, color);
}
void SimpleShapeDrawer::DrawFilledTriangle(int x, int y, int height, int width, const OpenGLColor& color)
{
	DrawTriangle(GL_TRIANGLE_FAN, x, y, height, width, color);
}


void SimpleShapeDrawer::DrawCircle(int method, float cx, float cy, float r, int numSegments,const OpenGLColor& color)
{
	float theta = 2 * 3.1415926 / float(numSegments);
	float tangetial_factor = tanf(theta);//calculate the tangential factor

	float radial_factor = cosf(theta);//calculate the radial factor

	float x = r;//we start at angle = 0

	float y = 0;

	glBegin(method);
	glColor3f(color.r, color.g, color.b);
	for(int ii = 0; ii < numSegments; ii++)
	{
		glVertex2f(x + cx, y + cy);//output vertex

		//calculate the tangential vector
		//remember, the radial vector is (x, y)
		//to get the tangential vector we flip those coordinates and negate one of them

		float tx = -y;
		float ty = x;

		//add the tangential vector

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		//correct using the radial factor

		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();
}

void SimpleShapeDrawer::DrawRect(int method, int x, int y, int width, int height, const OpenGLColor& color)
{
	glBegin(method);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glVertex2f(x, y);
	glEnd();
}

void SimpleShapeDrawer::DrawTriangle(int method, int x, int y, int height, int width, const OpenGLColor& color)
{
	int X[] = { x - width/2
			   , x + width/2
			   , x
			   };
	int Y[] = { y + height/2
			   , y - height/2
			   , y - height/2
		};

	glBegin(method);
	glColor3f(color.r, color.g, color.b);
	for(int i = 0;i < 3; i++)
	{
		glVertex2f(X[i], Y[i]);
	}
	glVertex2f(X[0], Y[0]);
	glEnd();
}
