#ifndef CIPHER_H
#define CIPHER_H

#include <QString>

class Cipher
{
	friend class Cipher_testable;
public:
	Cipher();
	~Cipher();

public:
	// Returns decrypted string
	QString decrypt(const QString& sourceString);

	// Returns encrypted string
	QString encrypt(const QString& sourceString);

private:
	// Appends some magic characters at the end of the source
	void append(int characterCount, QString& sourceString);
	
	// Appends some magic characters at the begining of the source
	void prepend(int characterCount, QString& sourceString);

	// Reverse string
	void reverse(QString& sourceString);
};

#endif // CIPHER_H

