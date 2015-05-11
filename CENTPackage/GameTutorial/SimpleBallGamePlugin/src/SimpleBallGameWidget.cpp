#include "SimpleBallGameWidget.h"

#include <QGLWidget>
#include <qdebug.h>

namespace
{
	// Defined max and min position for ball in y axis
	const float MAX_BALL_Y_PX = 55.0;
	const float MIN_BALL_Y_PX = 10.0;
	const float MAX_LOGICAL = 100;

	struct GLColor
	{
		GLfloat red, green, blue;
		void Use() const
		{
			glColor3f(red, green, blue);
		}
	};

	void drawRectangle(int x1, int y1, int x2, int y2)
	{
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0);
		glVertex2d(x1, y1);
		glTexCoord2d(1.0, 0.0);
		glVertex2d(x2, y1);
		glTexCoord2d(1.0, 1.0);
		glVertex2d(x2, y2);
		glTexCoord2d(0.0, 1.0);
		glVertex2d(x1, y2);
		glEnd();
	}
}

SimpleBallGameWidget::SimpleBallGameWidget(QWidget* parent)
	: GLWidget(parent)
	, m_backgroundTexture(0)
	, m_ballTexture(0)
{
}

SimpleBallGameWidget::~SimpleBallGameWidget()
{
}

void SimpleBallGameWidget::setBallPosition(const QPointF& point)
{
	m_ballPoint = point;
}

void SimpleBallGameWidget::initializeGL()
{
	GLWidget::initializeGL();
	QImage background(":/SimpleBallGame/background.png");
	m_backgroundTexture = bindTexture(background);

	QImage ball(":/SimpleBallGame/ball.png");
	m_ballTexture = bindTexture(ball);
}

void SimpleBallGameWidget::paintGL()
{
	glEnable(GL_TEXTURE_2D);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(1, 1, 1);

	// Draw background
	glPushMatrix();
	const float BOX_WIDTH = 130.0f;
	const float BOX_HEIGHT = 84.0f;
	glTranslatef(-BOX_WIDTH / 2, -BOX_HEIGHT / 2, -100.0);
	glBindTexture(GL_TEXTURE_2D, m_backgroundTexture);
	drawRectangle(0.0, 0.0, BOX_WIDTH, BOX_HEIGHT);
	glPopMatrix();

	// Draw ball
	glPushMatrix();
	const float RADIUS = 11.0f;

	// Ball point is in logical units, let's convert it to OpenGL units.
	const float yPixel = ((m_ballPoint.ry()/MAX_LOGICAL) * MAX_BALL_Y_PX) + MIN_BALL_Y_PX;
	glTranslatef(0, -35.0 + yPixel, -90.0);
	glBindTexture(GL_TEXTURE_2D, m_ballTexture);
	drawRectangle(-RADIUS, -RADIUS, RADIUS, RADIUS);
	glPopMatrix();

	glFlush();

	glDisable(GL_TEXTURE_2D);
}