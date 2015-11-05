#ifndef EMPTY_BALL_GAME_WIDGET_H
#define EMPTY_BALL_GAME_WIDGET_H

#include <QPoint>

#include "GLWidget.h"

class EmptyBallGameWidget : public GLWidget
{
	Q_OBJECT
public:
	explicit EmptyBallGameWidget(QWidget* parent = 0);
	virtual ~EmptyBallGameWidget();

public slots:
	void setBallLightened(bool lightened);

protected:
	virtual void initializeGL();
	virtual void paintGL();


private:
	bool  m_ballLightened;
};

#endif // EMPTY_BALL_GAME_WIDGET_H
