#include "test_DynamicDataModel.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QTest>

#include "DynamicDataModel.h"

void test_DynamicDataModel::construction()
{
	DynamicDataModel dataModelStack;

	DynamicDataModel* dataModelHeap = new DynamicDataModel;
	delete dataModelHeap;
	dataModelHeap = 0;
}

void test_DynamicDataModel::simpleDataSync()
{
	DynamicDataModel dataModel;

	QLineEdit line1, line2;
	line1.setObjectName("testLineEdit");
	line2.setObjectName("testLineEdit");

	dataModel.addToModel(&line1);
	dataModel.addToModel(&line2);

	line1.setText("newText");

	QCOMPARE(line1.text(), line2.text());
}

void test_DynamicDataModel::recursiveDataSync()
{
	//SetUp widgets
	QSpinBox* spinA1 = new QSpinBox;
	QSpinBox* spinB1 = new QSpinBox;
	QSpinBox* spinA2 = new QSpinBox;
	QSpinBox* spinB2 = new QSpinBox;

	spinA1->setObjectName("spin_1_Value");
	spinB1->setObjectName("spin_1_Value");
	spinA2->setObjectName("spin_2_Value");
	spinB2->setObjectName("spin_2_Value");

	QVBoxLayout* layoutA = new QVBoxLayout;
	QVBoxLayout* layoutB = new QVBoxLayout;
	layoutA->addWidget(spinA1);
	layoutA->addWidget(spinA2);

	layoutB->addWidget(spinB1);
	layoutB->addWidget(spinB2);

	QWidget widgetA, widgetB;
	widgetA.setLayout(layoutA);
	widgetB.setLayout(layoutB);

	//test case
	DynamicDataModel dataModel;
	dataModel.addToModel(&widgetA);
	dataModel.addToModel(&widgetB);

	spinA1->setValue(66);
	spinB2->setValue(99);

	QCOMPARE(spinA1->value(), spinB1->value());
	QCOMPARE(spinA2->value(), spinB2->value());
}
