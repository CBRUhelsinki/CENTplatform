#ifndef SUMMARY_DATA_H
#define SUMMARY_DATA_H

#include <QTime>

struct SummaryData
{
	QTime m_time;
	int   m_input;

	SummaryData(const QTime& currentTime, int input)
	{
		m_time = currentTime;
		m_input = input;
	}
};


#endif
