#include "CentUnitTest.h"

#include <QObject>


CentUnitTest::CentUnitTest()
{
}

CentUnitTest::~CentUnitTest()
{
	qDeleteAll(m_testList);
}

void CentUnitTest::addTest(QObject* test)
{
	if (test)
	{
		m_testList.append(test);
	}
}

int CentUnitTest::run(int argc, char* argv[])
{
	int result = 0;

	foreach (QObject* test, m_testList)
	{
		result += QTest::qExec(test, argc, argv);
	}

	return result;
}
