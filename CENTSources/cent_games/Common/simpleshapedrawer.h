#ifndef SIMPLE_SHAPE_DRAWER_H
#define SIMPLE_SHAPE_DRAWER_H

class SimpleShapeDrawer
{
public:
	typedef struct OpenGLColor
	{
		float r;
		float g;
		float b;
	}OpenGLColor;

	static void DrawFilledCircle(float cx, float cy, float r, int numSegments, const OpenGLColor& color);
	static void DrawEmptyCircle(float cx, float cy, float r, int numSegments, const OpenGLColor& color);

	static void DrawFilledRect(int x, int y, int width, int height, const OpenGLColor& color);
	static void DrawEmptyRect(int x, int y, int width, int height, const OpenGLColor& color);

	static void DrawEmptyTriangle(int x, int y, int height, int width, const OpenGLColor& color);
	static void DrawFilledTriangle(int x, int y, int height, int width, const OpenGLColor& color);

private:	
	static void DrawCircle(int method, float cx, float cy, float r, int numSegments,const OpenGLColor& color);
	static void DrawRect(int method, int x, int y, int width, int height, const OpenGLColor& color);
	static void DrawTriangle(int method, int x, int y, int height, int width, const OpenGLColor& color);
};

#endif // SIMPLE_SHAPE_DRAWER_H
