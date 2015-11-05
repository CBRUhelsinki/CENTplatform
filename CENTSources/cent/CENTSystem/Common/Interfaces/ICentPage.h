#ifndef ICENT_PAGE_H
#define ICENT_PAGE_H

#include <QVariant>
#include <QWidget>

#include <QPaintEvent>
#include <QStylePainter>
#include <QStyleOption>

class ICentPage: public QWidget
{
	Q_OBJECT
public:
	ICentPage(QWidget* parent = 0)
		: QWidget(parent)
	{
		setProperty("ICentPageWidget", true);
	}

	virtual ~ICentPage() {}

	virtual void paintEvent(QPaintEvent* event)
	{
		QStyleOption opt;
		opt.init(this);
		QStylePainter p(this);
		p.drawPrimitive(QStyle::PE_Widget, opt);
		QWidget::paintEvent(event);
	}

public:
	virtual void activate() = 0;
	virtual void deactivate() = 0;

	virtual QString headerText() {return "";}
};

#endif // ICENT_PAGE_H

