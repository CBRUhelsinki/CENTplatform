#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>

class QLabel;
class QPushButton;
class QSettings;

class SettingsWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SettingsWidget(QSettings& settings, QWidget *parent = 0);
	
signals:
	void backupPathChanged();

protected:
	virtual void showEvent(QShowEvent *);
	virtual void paintEvent(QPaintEvent* event);

private slots:
	void onChangeClicked();

private:
	void setupLayout();

private:
	Q_DISABLE_COPY(SettingsWidget)

	QSettings& m_settings;

	QLabel*    m_backupLabel;
};

#endif // SETTINGSWIDGET_H
