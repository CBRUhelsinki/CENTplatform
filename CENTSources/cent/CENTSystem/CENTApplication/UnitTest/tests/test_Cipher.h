#ifndef TEST_CIPHER_H
#define TEST_CIPHER_H

#include <QObject>

class test_Cipher: public QObject
{
	Q_OBJECT
public:
	explicit test_Cipher(QObject* parent = 0);
	virtual ~test_Cipher();

private slots:
	void construction();
	void encryption();
	void encryption_data();
	void prepend();
	void prepend_data();
	void append();
	void append_data();
	void reverse();
	void reverse_data();
};

#endif // TEST_CIPHER_H
