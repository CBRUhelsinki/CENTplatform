#ifndef SUMMARY_PAGE_H
#define SUMMARY_PAGE_H

#include "ICentPage.h"

class BarChartWidget;
class QLabel;

class SummaryPage: public ICentPage
{
	Q_OBJECT
public:
	enum Mode
	{
		PatientMode = 0,
		TherapistMode
	};

	explicit SummaryPage(Mode mode, QWidget* parent = 0);
	virtual ~SummaryPage();

public:
	virtual void activate() {};
	virtual void deactivate() {};
	QString headerText();

public slots:
	void setData(const QList< QPair<QString, double> > &data);
	void setScore(const double score);
	void setScoreLabelText(const QString& text);

signals:
	void playAgain();
	void nextClicked();
	void changeParams();
	void changeGame();

private:
	void createLayout();

private:
	Mode            m_mode;
	BarChartWidget* m_barChart;
	QLabel*         m_scoreLabel;
	QString         m_scoreText;
};

#endif // SUMMARY_PAGE_H

