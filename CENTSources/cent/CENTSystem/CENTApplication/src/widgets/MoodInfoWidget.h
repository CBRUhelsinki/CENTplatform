#ifndef MOOD_INFO_WIDGET_H
#define MOOD_INFO_WIDGET_H

#include "IInfoWidget.h"
#include "MultiButtonWidgetBase.h"

#include <QWidget>
#include <QList>

class QPushButton;
class QSignalMapper;
class QHBoxLayout;

namespace CentPatient
{
	struct Data;
}

class MoodInfoWidget : public MultiButtonWidgetBase, public IInfoWidget
{
	Q_OBJECT
public:
	explicit MoodInfoWidget(QWidget* parent = 0);
	virtual ~MoodInfoWidget();

	virtual void getQuestionaryData(CentPatient::Data& data);
	virtual void reset();
	virtual void triggerValidityCheck();
	virtual bool checkValid();

private:
	void createLayout();
	virtual void createConnections();
	virtual void decorateButton(int buttonNumber, QPushButton& button);

};

#endif //MOOD_INFO_WIDGET_H
