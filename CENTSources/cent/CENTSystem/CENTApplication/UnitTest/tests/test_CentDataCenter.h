#ifndef TEST_CENT_DATA_CENTER_H
#define TEST_CENT_DATA_CENTER_H

#include <QObject>

class test_CentDataCenter: public QObject
{
	Q_OBJECT
public:
	explicit test_CentDataCenter(QObject* parent = 0)
		: QObject(parent)
	{
	}


private slots:
	void construction();
	void savePatientSession();
	void savePatientSessionGameData();//TODO
	void setCurrentUser();//TODO
};

#endif // TEST_CENT_DATA_CENTER_H