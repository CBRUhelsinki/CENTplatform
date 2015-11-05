#ifndef CONSOLE_OUTPUT_H
#define CONSOLE_OUTPUT_H

#include <QWidget>
#include <QMap>
#include <QPointer>

class QTabWidget;
class QTextBrowser;

/*
 * This is implementation if Console popup window. It displays output from programs in tabs.
 * There is one tab per program, so it supports multiple applications at once. Every tab has
 * "Clear" button to clear current log
 */
class ConsoleOutput : public QWidget
{
	Q_OBJECT

public:
	explicit ConsoleOutput(QWidget* parent);

public slots:
	// Display message from program. It tab is not created yet for the program, it will be created,
	// otherwise reaused
	void print(QString msg, QString programTitle);

protected:
	virtual QSize sizeHint() const;

private:
	QTabWidget* m_tabs;
	QMap<QString, QTextBrowser*> m_browsers;
	QPointer<QWidget> m_parent;

	// Find or create a tab for given title
	QTextBrowser* tab(const QString& title);
};

#endif

