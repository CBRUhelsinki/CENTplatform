#ifndef SELECTCAPDRIVERDIALOG_H
#define SELECTCAPDRIVERDIALOG_H

#include "CentDataTypes.h"

#include <QDialog>
#include <QMap>

class QSettings;
class QLabel;
class QLineEdit;
class QPushButton;

class SelectCapDriverDialog : public QDialog
{
	Q_OBJECT
public:
	SelectCapDriverDialog(QWidget* parent = 0);
	virtual ~SelectCapDriverDialog();

private:
	void initialize();

private slots:
	void onSetCapDriver();
	void onRadioChanged();

private:
	QSettings*                             m_settings;
	CentData::CapDriverData                m_currentCapDriver;
	QMap<QString, CentData::CapDriverData> m_mapCapDrivers;
	QPushButton*                           m_okButton;
	QLabel*                                m_macLabel;
	QLineEdit*                             m_macField;
	QLabel*                                m_macWarrningLabel;
};

#endif // SELECTCAPDRIVERDIALOG_H
