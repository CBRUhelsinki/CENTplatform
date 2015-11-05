#ifndef ICENT_USER_DATA_MANAGER_H
#define ICENT_USER_DATA_MANAGER_H

#include <QObject>

namespace CentUser
{
	struct Data;
}

class ICentUserDataManager: public QObject
{
	Q_OBJECT
public:
	explicit ICentUserDataManager(QObject* parent = 0) 
		: QObject(parent) 
	{}

	virtual ~ICentUserDataManager() {}

public:
	virtual int addUser(CentUser::Data& data) = 0;
	virtual bool editUser(CentUser::Data& data) = 0;
	virtual bool removeUser(const int id) = 0;

	virtual CentUser::Data user(const int id) = 0;
	virtual QList<CentUser::Data> users() = 0;

signals:
	void userEdited(CentUser::Data& data);
	void userAdded(CentUser::Data& data);
	void userRemoved(int id);
};

#endif // ICENT_USER_DATA_MANAGER_H
