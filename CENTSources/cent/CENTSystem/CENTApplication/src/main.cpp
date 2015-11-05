#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QPoint>
#include <QSettings>
#include <QSharedMemory>

#include "CentController.h"
#include "CentConstants.h"
#include "CentMainWindow.h"
#include "Connect.h"
#include "QDebugRedirect.h"
#include "SelectCapDriverDialog.h"
#include "CentDataTypes.h"

#include "version.h"

namespace
{
	const QString STYLESHEET_PATH = ":/qss/CentGlobal.qss";
}

void centerWidget(const QRect& rect, QWidget& w)
{
	int x = rect.left() + (rect.width() - w.width()) / 2;
	int y = rect.top() + (rect.height() - w.height()) / 2;
	w.move(x,y);
}

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#ifndef _USE_OLD_IOSTREAMS

using namespace std;

#endif

// maximum mumber of lines the output console should have

static const WORD MAX_CONSOLE_LINES = 500;

//TODO: move it into a separate file
void RedirectIOToConsole()

{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;

	// allocate a console for this app
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
	ios::sync_with_stdio();
}

int main(int argc, char* argv[])
{
#ifdef _DEBUG // very usefull to see VRPN warnings and errors
	RedirectIOToConsole();
#endif

	QApplication app(argc, argv);
	app.setApplicationName("Cent Application");
	app.setApplicationVersion(CENT_VERSION_STRING);

	//---- Check for another instance code snippet ----
	QSharedMemory shared("EF906A7C-4A46-4F7D-8FA9-1C08A5F7FD4B");

	if( !shared.create( 512, QSharedMemory::ReadWrite) )
	{
		QMessageBox msgBox;
		msgBox.setText( QObject::tr("Can't start more than one instance of the application.") );
		msgBox.setIcon( QMessageBox::Critical );
		msgBox.exec();

		qWarning() << "Can't start more than one instance of the application.";

		exit(0);
	}
	else
	{
		qDebug() << "Application started successfully.";
	}
	//---- END OF Check for another instance code snippet ----

	// TODO: this is workaround for unloading plugins game which used OpenGL
#ifdef _DEBUG
	::LoadLibraryA("QtOpenGLd4.dll");
#else
	::LoadLibraryA("QtOpenGL4.dll");
#endif

	// stylesheet
	QFile file(STYLESHEET_PATH);
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());

	qApp->setStyleSheet(styleSheet);

	// Redirect output from qDebug to a log file
	qInstallMsgHandler(QDebugMessageHandler);
	qWarning() << "Cent Application Started, version: " << CENT_VERSION_STRING;	//warning used to show up in release builds too


	QSettings settings(QSettings::SystemScope, CENT::Settings::CENT_ORGANIZATION_NAME, CENT::Settings::CENT_APPLICATION_NAME);

	SelectCapDriverDialog selectCapDriverDialog;
	if(selectCapDriverDialog.exec() != QDialog::Accepted)
	{
		exit(CENT::NotSelectedCapDrivers);
	}

	CentMainWindow patientWindow("Patient");
	CentMainWindow researcherWindow("Therapist");

	CENT::connect(&researcherWindow, SIGNAL(closeClicked()), &app, SLOT(quit()));
	CENT::connect(&patientWindow, SIGNAL(closeClicked()), &app, SLOT(quit()));

	QDesktopWidget *desktop = QApplication::desktop();
	if(desktop->screenCount() > 1)
	{
		bool researcherSet = false, patientSet = false;
		for(int i = 0; i < desktop->screenCount(), !(researcherSet && patientSet); i++)
		{
			QRect rect = desktop->screenGeometry(i);
			if(i == desktop->primaryScreen())
			{
				centerWidget(rect, researcherWindow);
				researcherSet = true;
			}
			else if(! patientSet)
			{
				centerWidget(rect, patientWindow);
				patientSet = true;
			}
		}
	}

	CentController centController(settings, researcherWindow, patientWindow);
	researcherWindow.raise();// We want, at least for testing, researcher window as a top one on one monitor configurarion
	return app.exec();
}
