#include "Cipher.h"

#include <QTime>

namespace
{
	const int PREPEND_CHAR_COUNT = 3;
	const int APPEND_CHAR_COUNT  = 2;
	const QString AVAILABLE_CHARACTERS = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
}

Cipher::Cipher()
{
	qsrand(QTime::currentTime().msec());
}

Cipher::~Cipher()
{
}

QString Cipher::decrypt(const QString& sourceString)
{
	QString output = sourceString.mid(PREPEND_CHAR_COUNT);
	output.chop(APPEND_CHAR_COUNT);
	reverse(output);

	QByteArray byteArray;
	byteArray.append(output);

	output = byteArray.fromBase64(byteArray);

	return output;
}

QString Cipher::encrypt(const QString& sourceString)
{
	QByteArray byteArray;
	byteArray.append(sourceString);
	QString output = byteArray.toBase64();

	reverse(output);
	prepend(PREPEND_CHAR_COUNT, output);
	append(APPEND_CHAR_COUNT, output);

	return output;
}

void Cipher::append(int characterCount, QString& sourceString)
{
	int index = 0;
	for (int i = 0; i < characterCount; ++i)
	{
		index = qrand() % AVAILABLE_CHARACTERS.length();
		sourceString.append(AVAILABLE_CHARACTERS.at(index));
	}
}

void Cipher::prepend(int characterCount, QString& sourceString)
{
	int index = 0;
	for (int i = 0; i < characterCount; ++i)
	{
		index = qrand() % AVAILABLE_CHARACTERS.length();
		sourceString.prepend(AVAILABLE_CHARACTERS.at(index));
	}
}

void Cipher::reverse(QString& sourceString)
{
	const int length = sourceString.length();
	const int half = length / 2;
	QChar ch;

	for (int i = 0; i < half; ++i)
	{
		ch = sourceString.at(i);
		sourceString[i] = sourceString[length - 1 - i];
		sourceString[length - 1 - i] = ch;
	}
}

