#ifndef VRPN_DIGITAL_CLIENT_H
#define VRPN_DIGITAL_CLIENT_H

#include <QObject>
#include "CentDataTypes.h"
#include "vrpn_Button.h"

class VRPNDigitalClient: public QObject, public vrpn_Button_Remote
{
	Q_OBJECT
public:
	explicit VRPNDigitalClient(const QString& serverName, QObject* parent = 0);
	virtual ~VRPNDigitalClient();

	const QString& serverName() const;
	CENT::ErrorCode lastError();

signals:
	void dataReceived(CentData::DigitalData data); //TODO: try to find a way not to copy it here

private:
	friend class VRPNDigitalClientPrivate;
	VRPNDigitalClientPrivate* d; //has a public void digitalCallback(vrpn_BUTTONCB button);
};

#endif // VRPN_DIGITAL_CLIENT_H

