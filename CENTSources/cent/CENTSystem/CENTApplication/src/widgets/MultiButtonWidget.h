#ifndef MULTI_BUTTON_WIDGET_H
#define MULTI_BUTTON_WIDGET_H

#include "MultiButtonWidgetBase.h"

#include <QWidget>
#include <QList>

class QLabel;
class QHBoxLayout;
class QPushButton;

class MultiButtonWidget : public MultiButtonWidgetBase
{
	Q_OBJECT
public:
	explicit MultiButtonWidget(int buttonCount, QWidget* parent = 0);
	virtual ~MultiButtonWidget();

	virtual void reset();
	virtual void triggerValidityCheck();

	void setLowScaleText(const QString& text);
	void setHighScaleText(const QString& text);
	void setDescriptionText(const QString& text);

private:
	void createLayout();
	virtual void createConnections();
	virtual void decorateButton(int buttonNumber, QPushButton& button);

	QLabel* m_lowScaleLabel;
	QLabel* m_highScaleLabel;
	QLabel* m_descriptionLabel;
};

#endif // MULTI_BUTTON_WIDGET_H
