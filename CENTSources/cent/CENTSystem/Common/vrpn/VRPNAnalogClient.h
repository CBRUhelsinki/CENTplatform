#ifndef VRPN_ANALOG_CLIENT_H
#define VRPN_ANALOG_CLIENT_H

#include <QObject>
#include "CentDataTypes.h"
#include "vrpn_Analog.h"

class VRPNAnalogClient: public QObject, public vrpn_Analog_Remote
{
	Q_OBJECT
public:
	explicit VRPNAnalogClient(const QString& serverName, bool onlyOnChanges, QObject* parent = 0);
	virtual ~VRPNAnalogClient();

	const QString& serverName() const;
	CENT::ErrorCode lastError();
signals:
	void dataReceived(CentData::AnalogData data); //TODO: try to find a way not to copy it here

private:
	friend class VRPNAnalogClientPrivate;
	VRPNAnalogClientPrivate* d; //has a public void analogCallback(vrpn_ANALOGCB analog);
};

#endif // VRPN_ANALOG_CLIENT_H
