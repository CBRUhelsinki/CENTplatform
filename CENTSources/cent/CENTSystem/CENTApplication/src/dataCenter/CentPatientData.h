#ifndef CENT_PATIENT_DATA_H
#define CENT_PATIENT_DATA_H

#include <QString>
#include <QDateTime>
#include <QMetaType>
#include <QDateTime>

namespace CentPatient
{
	const int INVALID = -99;

	struct Data
	{
		QString    m_userName;
		int        m_hoursSlept;
		int        m_hoursSinceLastSleep;
		int        m_motivation;
		int        m_mood;
		int        m_excitement;
		QDateTime      m_date;


		Data()
		{
			m_userName = "John Doe";//<todo> add login page
			m_hoursSlept = INVALID;
			m_hoursSinceLastSleep = INVALID;
			m_motivation = INVALID;
			m_mood  = INVALID;
			m_excitement = INVALID;
			m_date = QDateTime::currentDateTime();
		}

		Data(const QString& userName, int hoursSlept, int hoursSinceLastSleep,
			 int motivation, int mood, int excitement, QDateTime date)
		{
			m_userName = userName;
			m_hoursSlept = hoursSlept;
			m_hoursSinceLastSleep = hoursSinceLastSleep;
			m_motivation = motivation;
			m_mood = mood;
			m_excitement = excitement;
			m_date = date;
		}
	};

	struct GameData
	{
		QString m_gameName;
		QDateTime m_gameDate;
		//TODO: game configuration parameters

		GameData()
		{
			m_gameName = "";
		}

		GameData(const QString& gameName, QDateTime gameDate)
		{
			m_gameName = gameName;
			m_gameDate = gameDate;
		}
	};

	struct SummaryData
	{
		double m_gameScore;

		SummaryData()
		{
			m_gameScore = static_cast<double>(INVALID);
		}

		SummaryData(double gameScore)
		{
			m_gameScore = gameScore;
		}
	};

	struct EvaluationData
	{
		EvaluationData()
		{
			m_performance = static_cast<double>(INVALID);
			m_stress = static_cast<double>(INVALID);
		}

		EvaluationData(int performance, int stress)
		{
			m_performance = performance;
			m_stress = stress;
		}

		int m_performance;
		int m_stress;
	};
}

Q_DECLARE_METATYPE(CentPatient::Data);
Q_DECLARE_METATYPE(CentPatient::GameData);
Q_DECLARE_METATYPE(CentPatient::SummaryData);
Q_DECLARE_METATYPE(CentPatient::EvaluationData);

#endif // CENT_PATIENT_DATA_H
