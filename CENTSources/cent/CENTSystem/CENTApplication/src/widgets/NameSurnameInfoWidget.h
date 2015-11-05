#ifndef NAME_SURNAME_INFO_WIDGET_H
#define NAME_SURNAME_INFO_WIDGET_H

#include "IInfoWidget.h"

#include <QWidget>
#include <QObject>

class QLineEdit;

namespace CentPatient
{
	struct Data;
}

class NameSurnameInfoWidget : public QWidget, public IInfoWidget
{
	Q_OBJECT
public:
	explicit NameSurnameInfoWidget(QWidget* parent = 0);
	virtual ~NameSurnameInfoWidget();

	virtual void getQuestionaryData(CentPatient::Data& data);
	virtual void reset();
	virtual void triggerValidityCheck();
	virtual bool checkValid();

signals:
	void isValid(bool valid);

private slots:
	void onChange();

private:
	void createConnections();
	void createLayout();

private:
	QLineEdit* m_name;
	QLineEdit* m_lastName;
};

#endif //NAME_SURNAME_INFO_WIDGET_H