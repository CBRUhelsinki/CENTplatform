#ifndef CENT_MAIN_WINDOW_H
#define CENT_MAIN_WINDOW_H

#include <QMainWindow>

class QLabel;
class QStackedWidget;

class CentMainWindow: public QMainWindow
{
	Q_OBJECT
public:
	explicit CentMainWindow(const QString& title, QWidget* parent = 0, Qt::WindowFlags flags = 0);
	virtual ~CentMainWindow();

public:
	bool addPage(QWidget* page);
	bool activatePage(QWidget* page);

	QString headerText();
	bool headerVisible();

public slots:
	void setHeaderText(const QString& text);
	void setHeaderVisible(bool visible);

signals:
	void closeClicked();
	
private:
	void createLayout();
	void createMenu();

protected:
	void closeEvent(QCloseEvent* closeEvent);
	
private:
	QString         m_title;
	QLabel*         m_headerLabel;
	QStackedWidget* m_stackedWidget;
};

#endif // CENT_MAIN_WINDOW_H

