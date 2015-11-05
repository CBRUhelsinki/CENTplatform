#include "EmptyBallGameWidget.h"

#include "simpleshapedrawer.h"

namespace
{
	const SimpleShapeDrawer::OpenGLColor LIGHTEN_COLOR = {1.0, 1.0, 0.0};
	const SimpleShapeDrawer::OpenGLColor SHADOW_COLOR = {0.0, 1.0, 0.0};
}

EmptyBallGameWidget::EmptyBallGameWidget(QWidget* parent)
	: GLWidget(parent)
	, m_ballLightened(false)
{
}

EmptyBallGameWidget::~EmptyBallGameWidget()
{
}

void EmptyBallGameWidget::setBallLightened(bool lightened)
{
	m_ballLightened = lightened;
}

void EmptyBallGameWidget::initializeGL()
{
	GLWidget::initializeGL();
}

void EmptyBallGameWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -32.0);

	if (m_ballLightened)
	{
		SimpleShapeDrawer::DrawFilledCircle(0,0,10,100,LIGHTEN_COLOR);
	}
	else
	{
		SimpleShapeDrawer::DrawEmptyCircle(0,0,10,100,SHADOW_COLOR);
	}
}
