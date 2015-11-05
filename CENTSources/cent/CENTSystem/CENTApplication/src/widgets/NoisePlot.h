#ifndef NOISE_PLOT_H
#define NOISE_PLOT_H

#include "CentDataTypes.h"

#include <QWidget>
#include <QString>

class QwtThermo;

class NoisePlot : public QWidget
{
	Q_OBJECT
public:
	explicit NoisePlot(const QString& label, QWidget* parent = 0);
	virtual ~NoisePlot();
	
public slots:
	void onDataReceived(const CentData::AnalogData& data);
	void onReset();

private:
	void createLayout(const QString& labelText);

private:
	QwtThermo* m_noisePlot;
};

#endif NOISE_PLOT_H
