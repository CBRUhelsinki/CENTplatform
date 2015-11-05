#ifndef TUTORIAL_PAGE_H
#define TUTORIAL_PAGE_H

#include "ICentPage.h"

class QComboBox;
class QTextEdit;
class QWebView;

class TutorialPage: public ICentPage
{
	Q_OBJECT
public:
	explicit TutorialPage(QWidget* parent = 0);
	virtual ~TutorialPage();

signals:
	void backClicked();
	void nextClicked();

public:
	virtual void activate() {};
	virtual void deactivate() {};

	void setTutorial(const QString& fileName);

private:
	void createLayout();

private:
	Q_DISABLE_COPY(TutorialPage)

	QComboBox* m_gameComboBox;
	QWebView* m_tutorialWebView;
};
#endif // TUTORIAL_PAGE_H

