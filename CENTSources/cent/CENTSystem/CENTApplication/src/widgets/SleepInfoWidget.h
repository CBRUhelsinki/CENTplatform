#ifndef SLEEP_INFO_WIDGET_H
#define SLEEP_INFO_WIDGET_H

#include "IInfoWidget.h"

#include <QWidget>
#include <QObject>

class QSpinBox;

namespace CentPatient
{
	struct Data;
}

class SleepInfoWidget : public QWidget, public IInfoWidget
{
	Q_OBJECT
public:
	explicit SleepInfoWidget(QWidget* parent = 0);
	virtual ~SleepInfoWidget();

	virtual void getQuestionaryData(CentPatient::Data& data);
	virtual void reset();
	virtual void triggerValidityCheck();
	virtual bool checkValid();

signals:
	void isValid(bool valid);

private:
	void createConnections();
	void createLayout();

private:
	QSpinBox* m_hoursSleptSpinBox;
	QSpinBox* m_hoursSinceSleepSpinBox;
};

#endif //SLEEP_INFO_WIDGET_H