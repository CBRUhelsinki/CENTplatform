#include "test_Cipher.h"
#include "Cipher.h"

#include <QTest>

class Cipher_testable: public Cipher
{
public:
	using Cipher::append;
	using Cipher::prepend;
	using Cipher::reverse;
};

test_Cipher::test_Cipher(QObject* parent)
	: QObject(parent)
{
}

test_Cipher::~test_Cipher()
{
}

void test_Cipher::construction()
{
	Cipher_testable cipher;
	Cipher_testable* cipherPtr = new Cipher_testable();
	delete cipherPtr;
}

void test_Cipher::encryption()
{
	Cipher_testable cipher;

	QString encryptOutput, decryptOutput;
	QFETCH(QString, input);

	encryptOutput = cipher.encrypt(input);

	QVERIFY(encryptOutput != input);
	QVERIFY(encryptOutput.length() != input.length());

	decryptOutput = cipher.decrypt(encryptOutput);

	QCOMPARE(decryptOutput, input);
}

void test_Cipher::encryption_data()
{
	QTest::addColumn<QString>("input");

	QTest::newRow("row_1") << "";
	QTest::newRow("row_2") << "a";
	QTest::newRow("row_3") << "abcd";
	QTest::newRow("row_4") << "john_doe";
}

void test_Cipher::prepend()
{
	Cipher_testable cipher;

	QFETCH(QString, input);
	QFETCH(int, count);

	QString tmp = input;
	cipher.prepend(count, tmp);
	QCOMPARE(tmp.length(), input.length() + count);
	QCOMPARE(tmp.mid(count), input);
}

void test_Cipher::prepend_data()
{
	QTest::addColumn<QString>("input");
	QTest::addColumn<int>("count");

	QTest::newRow("row_1") << "" << 3;
	QTest::newRow("row_2") << "a" << 4;
	QTest::newRow("row_3") << "abcd" << 0;
	QTest::newRow("row_4") << "john_doe" << 5;
}

void test_Cipher::append()
{
	Cipher_testable cipher;

	QFETCH(QString, input);
	QFETCH(int, count);

	QString tmp = input;
	cipher.append(count, tmp);
	QCOMPARE(tmp.length(), input.length() + count);

	tmp.chop(count);
	QCOMPARE(tmp, input);
}

void test_Cipher::append_data()
{
	QTest::addColumn<QString>("input");
	QTest::addColumn<int>("count");

	QTest::newRow("row_1") << "" << 3;
	QTest::newRow("row_2") << "a" << 4;
	QTest::newRow("row_3") << "abcd" << 0;
	QTest::newRow("row_4") << "john_doe" << 5;
}

void test_Cipher::reverse()
{
	Cipher_testable cipher;
	QFETCH(QString, input);

	cipher.reverse(input);
	QTEST(input, "output");
}

void test_Cipher::reverse_data()
{
	QTest::addColumn<QString>("input");
	QTest::addColumn<QString>("output");

	QTest::newRow("row_1") << "" << "";
	QTest::newRow("row_2") << "a" << "a";
	QTest::newRow("row_3") << "ab" << "ba";
	QTest::newRow("row_3") << "abcd" << "dcba";
	QTest::newRow("row_4") << "john_doe" << "eod_nhoj";
}
