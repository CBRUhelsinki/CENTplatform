#ifndef SIMPLE_PAGE_PAGE_H
#define SIMPLE_PAGE_PAGE_H

#include "ICentPage.h"

#include <QList>

class QLabel;
class QPushButton;
class QTextBrowser;
class QWidget;

class SimplePage : public ICentPage
{
	Q_OBJECT
public:
	explicit SimplePage(const QString& title, QWidget* parent = 0);
	virtual ~SimplePage();

signals:
	void buttonClicked(int index);

public:
	virtual void activate() {};
	virtual void deactivate() {};

	void setContent(const QString& content, Qt::Alignment alignment);
	QString content();
	void setBtnVisible(int index, bool visible);
	void setBtnText(int index,const QString& text);
	bool btnVisible(int index);
	QString btnText(int index);

	QString headerText();

	void setFooterText(const QString& text);

private:
	int countContentHeitht();
	void createLayout();

private:
	QTextBrowser* m_contentBrowser;
	QList<QPushButton*> m_buttons;
	QWidget* m_topWidget;
	QWidget* m_bottomWidget;
	QWidget* m_buttonsWidget;

	QLabel* m_footerLabel;
	QString  m_title;
};

#endif // SIMPLE_PAGE_PAGE_H
