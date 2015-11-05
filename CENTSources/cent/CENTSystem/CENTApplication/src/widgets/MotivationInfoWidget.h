#ifndef MOTIVATION_INFO_WIDGET_H
#define MOTIVATION_INFO_WIDGET_H

#include "IInfoWidget.h"
#include "MultiButtonWidgetBase.h"

#include <QWidget>
#include <QList>

class QPushButton;
class QSignalMapper;
class QSpacerItem;

namespace CentPatient
{
	struct Data;
}

class MotivationInfoWidget : public MultiButtonWidgetBase, public IInfoWidget
{
	Q_OBJECT
public:
	explicit MotivationInfoWidget(QWidget* parent = 0);
	virtual ~MotivationInfoWidget();

	virtual void getQuestionaryData(CentPatient::Data& data);
	virtual void reset();
	virtual void triggerValidityCheck();
	virtual bool checkValid();

private:
	void createLayout();
	virtual void createConnections();
	virtual void decorateButton(int buttonNumber, QPushButton& button);

};

#endif //MOTIVATION_INFO_WIDGET_H