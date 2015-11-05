#ifndef ABOUT_WIDGET_H
#define ABOUT_WIDGET_H

#include <QWidget>

class AboutWidget : public QWidget
{
	Q_OBJECT
public:
	explicit AboutWidget(const QString& content, QWidget* parent = NULL);
	virtual ~AboutWidget();

protected:
	virtual void paintEvent(QPaintEvent* event);
};
#endif // ABOUT_WIDGET_H
