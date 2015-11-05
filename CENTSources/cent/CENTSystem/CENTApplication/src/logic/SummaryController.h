#ifndef SUMMARY_CONTROLLER_H
#define SUMMARY_CONTROLLER_H

#include <QObject>
#include <QTime>

#include "CentDataTypes.h"
#include "SummaryData.h"

class SummaryController: public QObject
{
	Q_OBJECT
public:
	explicit SummaryController(QObject* parent = 0);
	virtual ~SummaryController();

public:
	const QList<SummaryData>& userInputData() const;
	const QList<SummaryData>& expectedInputData() const;
	const double sessionTime() const;

	//ToDo:: remove it later
	const double score();

signals:
	void summaryChanged();

public slots:
	void onGameStarted();
	void onGameFinished();

	void onExpectedInput(const CentData::DigitalData& data);
	void onUserInput(const CentData::DigitalData& data);

private:
	void resetTimeStamps();
	void resetInputData();
	void addData(QList<SummaryData>& dataContainer, const CentData::DigitalData& data);

private:
	QTime m_gameStarted;
	QTime m_gameFinished;
	bool  m_duringGame;

	QList<SummaryData> m_expectedInputData;
	QList<SummaryData> m_userInputData;
	double             m_sessionTime;
};

#endif // SUMMARY_CONTROLLER_H
