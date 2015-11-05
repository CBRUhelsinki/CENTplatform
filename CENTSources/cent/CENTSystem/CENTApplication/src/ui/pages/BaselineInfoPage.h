#ifndef BASELINE_INFO_PAGE_PAGE_H
#define BASELINE_INFO_PAGE_PAGE_H

#include "ICentPage.h"

class QCheckBox;
class QDoubleSpinBox;
class QLabel;
class QPushButton;
class QSpinBox;

class BaseLinePlot;
class PowerSpectrumData;

class BaselineInfoPage : public ICentPage
{
	Q_OBJECT
public:
	explicit BaselineInfoPage(QWidget* parent = 0);
	virtual ~BaselineInfoPage();

signals:
	void reRecord();
	void next();
	void gameTimeoutChanged(int minutes);

public:
	virtual void activate();
	virtual void deactivate();

	double betaCoeffValue();
	double thetaCoeffValue();

	void resetSession();

public slots:
	// Sets data which will be displayed by the plot widget
	void setDisplayData(const PowerSpectrumData& data);

	void setBetaThetaLevels(double beta, double theta);

private slots:
	void onGameTimeChange();

private:
	void createLayout();
	void preparePlot(int channel, int sampleCount, const PowerSpectrumData& data);

private:
	QLabel*         m_betaLevelLabel;
	QLabel*         m_thetaLevelLabel;
	QDoubleSpinBox* m_spinCoefBeta;
	QDoubleSpinBox* m_spinCoefTheta;
	QPushButton*    m_reRecordButton;
	QPushButton*    m_nextButton;
	QCheckBox*      m_gameTimeoutCheckBox;
	QSpinBox*       m_spinGameDuration;


	BaseLinePlot* m_plot;
	double* m_x;
	double* m_y;
	int m_size;
};

#endif // BASELINE_INFO_PAGE_PAGE_H
