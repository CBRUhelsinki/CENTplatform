#ifndef POWERLEVELBAR_H
#define POWERLEVELBAR_H

#include <QWidget>

class QwtThermo;

class PowerLevelBar : public QWidget
{
	Q_OBJECT
public:
	explicit PowerLevelBar(int pipeWidth = 20, QWidget *parent = 0);

public slots:
	void setPipeWidth(int w);
	void setColors(const QColor& negative, const QColor& positive);
	void setRange(double min, double max);
	void setValue(double val);

protected:
	QwtThermo* createQwtThermo(int pipeWidth, double min, double max);

private:
	QwtThermo* m_thermoPositive;
	QwtThermo* m_thermoNegative;

};

#endif // POWERLEVELBAR_H
