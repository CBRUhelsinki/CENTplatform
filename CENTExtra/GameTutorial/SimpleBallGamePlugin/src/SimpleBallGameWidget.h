#ifndef SIMPLE_BALL_GAME_WIDGET_H
#define SIMPLE_BALL_GAME_WIDGET_H

#include <QPoint>

#include "GLWidget.h"

class SimpleBallGameWidget : public GLWidget
{
	Q_OBJECT
public:
	explicit SimpleBallGameWidget(QWidget* parent = 0);
	virtual ~SimpleBallGameWidget();

public slots:
	void setBallPosition(const QPointF& point);

protected:
	virtual void initializeGL();
	virtual void paintGL();


private:
	unsigned int m_backgroundTexture;
	unsigned int m_ballTexture;
	QPointF      m_ballPoint;
};

#endif // SIMPLE_BALL_GAME_WIDGET_H