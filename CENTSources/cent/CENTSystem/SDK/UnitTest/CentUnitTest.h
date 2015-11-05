#ifndef CENT_UNIT_TEST_H
#define CENT_UNIT_TEST_H

#include <QList>
#include <QTest>
#include "vld.h"

class QObject;

class CentUnitTest
{
public:
	CentUnitTest();
	~CentUnitTest();

public:
	void addTest(QObject* test);
	int run(int argc, char* argv[]);

private:
	QList<QObject*> m_testList;
};

#endif // CENT_UNIT_TEST_H
