#ifndef SESSION_FEEDBACK_PAGE_H
#define SESSION_FEEDBACK_PAGE_H

#include "ICentPage.h"

class BarChartWidget;
class QLabel;

class SessionFeedbackPage: public ICentPage
{
	Q_OBJECT
public:
	explicit SessionFeedbackPage(QWidget* parent = 0);
	virtual ~SessionFeedbackPage();

public:
	virtual void activate() {};
	virtual void deactivate() {};

	virtual QString headerText();

public slots:
	void setData(const QList< QPair<QString, double> > &data);
	void setScore(const double score);
	void setScoreLabelText(const QString& text);

signals:
	void nextClicked();

private:
	void createLayout();

private:
	BarChartWidget* m_barChart;
	QLabel*         m_scoreLabel;
	QString         m_scoreText;
};


#endif // SESSION_FEEDBACK_PAGE_H
