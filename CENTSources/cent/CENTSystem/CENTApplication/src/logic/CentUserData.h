#ifndef CENT_USER_DATA_H
#define CENT_USER_DATA_H

#include <QString>
#include <QDateTime>
#include <QMetaType>

namespace CentUser
{
	const int INVALID = -99;

	enum Sex
	{
		Male = 0,
		Female,
		InvalidSex
	};

	enum Handedness
	{
		Left = 0,
		Right,
		InvalidHandedness
	};

	struct Data
	{
		int        m_id;
		QString    m_name;
		QString    m_surname;
		int        m_age;
		Sex        m_sex;
		Handedness m_handedness;
		QDateTime  m_edited;
		float      m_ratio;
		float      m_percentage;


		Data()
		{
			m_id         = INVALID;
			m_age        = INVALID;

			m_sex        = InvalidSex;
			m_handedness = InvalidHandedness;
			
			m_ratio      = INVALID;
			m_percentage = INVALID;
		}

		Data(const int id, const QString& name, const QString& surname,
			 const int age, Sex sex, Handedness handedness, const QDateTime& dateTime,
			 const float ratio, const float percentage)
		{
			m_id         = id;
			m_name       = name;
			m_surname    = surname;
			m_age        = age;
			m_sex        = sex;
			m_handedness = handedness;
			m_edited     = dateTime;
			m_ratio      = ratio;
			m_percentage = percentage;
		}
	};
}

Q_DECLARE_METATYPE(CentUser::Data);

#endif // CENT_USER_DATA_H

