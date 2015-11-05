#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QDialog>
#include <QString>

class QCheckBox;
class QLabel;
class Settings;

class SettingsWidget : public QDialog
{
	Q_OBJECT
public:
	SettingsWidget(Settings* setting, QWidget* parent = 0);
	virtual ~SettingsWidget();

	virtual QSize sizeHint() { return QSize(320, 200); };

private slots:
	void openFile();
	void blankMode();

private:
	QCheckBox* m_checkBox;
	QLabel*    m_filePathLabel;
	Settings*  m_settings;
};
#endif
