#include "VRPNServerThread.h"

#include <QDebug>
#include <QTimer>
#include <QThread>

#include "Connect.h"
#include "vrpn_Button.h"
#include "vrpn_connection.h"

namespace
{
	const char* BUTTON_SERVER_NAME = "cent_button_vrpn";
	const int BUTTON_SERVER_PORT = 3884; //TODO: Document it better with comment that it is reserved by "SofTrack Metering"
	const int NUMBER_OF_BUTTONS = 2;
	const unsigned long SLEEP_TIME = 100;

	
}

class myButtonServer : public vrpn_Button_Server
	{
	public:
		myButtonServer(const char * name, vrpn_Connection* conn, int num)
			: vrpn_Button_Server(name, conn, num) {}
		virtual ~myButtonServer() {}

		void reportState()
		{
			if (d_connection)
			{
				for (int i = 0; i < num_buttons; i++)
				{
					char	msgbuf[1000];
					vrpn_int32	len = encode_to(msgbuf,i, buttons[i]);
					if (d_connection->pack_message(len, timestamp, change_message_id, d_sender_id, msgbuf, vrpn_CONNECTION_RELIABLE))
					{
						qCritical("myButtonServer: can't write message: tossing\n");
					}
				}
			}
			else
			{
				qWarning("No valid connection");
			}
		}

		virtual void mainloop()
		{
			server_mainloop();
		}

		unsigned char button(int which)
		{
			if(which < 0 || which > num_buttons)
			{
				return -1;
			}
			return buttons[which];
		}
	};

VRPNServerThread::VRPNServerThread(QObject* parent)
	: QThread(parent)
	, m_connection(0)
	, m_buttonServer(0)
	, m_stopRequested(false)
{
}

VRPNServerThread::~VRPNServerThread()
{
	m_stopRequested = true;
	wait();
}

void VRPNServerThread::run()
{
	m_connection = new vrpn_Connection_IP(BUTTON_SERVER_PORT);
	m_buttonServer = new myButtonServer(BUTTON_SERVER_NAME, m_connection, NUMBER_OF_BUTTONS);

	while(! m_stopRequested)
	{
		m_buttonServer->reportState();
		m_buttonServer->mainloop();
		m_connection->mainloop();
		msleep(SLEEP_TIME);
	}

	delete m_buttonServer;
	m_buttonServer = 0;

	delete m_connection;
	m_connection = 0;
}


void VRPNServerThread::setButtonState(unsigned int which, int state)
{
	if(m_buttonServer && !m_stopRequested)
	{
		m_buttonServer->set_button(which, state);
	}
}
