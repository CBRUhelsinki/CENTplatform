#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#include <QGLWidget>
#include <QPointer>

class ICentGamePlugin;

class GLWidget: public QGLWidget
{
public:
	explicit GLWidget(QWidget* a_parent = 0);
	virtual ~GLWidget();

public:
	virtual QSize sizeHint() { return QSize(200, 200); };

protected:
	virtual void initializeGL();
	virtual void resizeGL(int a_width, int a_height);
};

#endif // GL_WIDGET_H

