#ifndef TEST_DYNAMIC_DATA_MODEL_H
#define TEST_DYNAMIC_DATA_MODEL_H

#include <QObject>

class test_DynamicDataModel : public QObject
{
	Q_OBJECT
public:
	test_DynamicDataModel(QObject* parent = 0)
		: QObject(parent)
	{}

private slots:
	void construction();
	void simpleDataSync();
	void recursiveDataSync();
};



#endif // TEST_DYNAMIC_DATA_MODEL_H