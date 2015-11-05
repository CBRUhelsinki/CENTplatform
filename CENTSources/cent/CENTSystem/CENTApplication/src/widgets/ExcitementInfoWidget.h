#ifndef EXCITEMENT_INFO_WIDGET_H
#define EXCITEMENT_INFO_WIDGET_H

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

class ExcitementInfoWidget : public MultiButtonWidgetBase, public IInfoWidget
{
	Q_OBJECT
public:
	explicit ExcitementInfoWidget(QWidget* parent = 0);
	virtual ~ExcitementInfoWidget();

	virtual void getQuestionaryData(CentPatient::Data& data);
	virtual void reset();
	virtual void triggerValidityCheck();
	virtual bool checkValid();

private:
	void createLayout();
	virtual void createConnections();
	virtual void decorateButton(int buttonNumber, QPushButton& button);

};

#endif //EXCITEMENT_INFO_WIDGET_H
