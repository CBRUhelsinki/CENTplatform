#ifndef Q_DEBUG_REDIRECT_H
#define Q_DEBUG_REDIRECT_H

#include <QtDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QSettings>

#include "PatientDataCenter.h"

#ifdef WIN32
 #include "windows.h"
#endif // WIN32

namespace
{
	const QString CENT_LOG_FILE_NAME("CENTLog.txt");
	const QString CENT_LOG_DIR("logs");

	const QString DEBUG_MESSAGE_FORMAT("Debug: %1");
	const QString WARNING_MESSAGE_FORMAT("Warning: %1");
	const QString CRITICAL_MESSAGE_FORMAT("Critical: %1");
	const QString FATAL_MESSAGE_FORMAT("Fatal: %1");

	//TODO: temporary
	const QString SEP(QDir::separator());
	const QString BASE_DIR ("CENT");
}

static QString s_logFilePath;

void QDebugMessageHandler(QtMsgType type, const char *msg)
{
	bool f_abort = false;
	QString txt;
	switch (type)
	{
#ifndef NDEBUG
	case QtDebugMsg:
		txt = DEBUG_MESSAGE_FORMAT.arg(msg);
		break;
#endif
	case QtWarningMsg:
		txt = WARNING_MESSAGE_FORMAT.arg(msg);
		break;

	case QtCriticalMsg:
		txt = CRITICAL_MESSAGE_FORMAT.arg(msg);
		break;

	case QtFatalMsg:
		txt = FATAL_MESSAGE_FORMAT.arg(msg);
		f_abort = true;
		break;

	default:
		return;
		break;
	}
	txt += "\n";

	if(s_logFilePath.isEmpty())
	{
		//read my documents from registry
		QString myDocuments = PatientDataCenter::getDataParentDir();

		QString centDir(myDocuments + SEP + BASE_DIR);
		QString centLogFileDir(centDir + SEP + CENT_LOG_DIR);

		QDir dir = QDir::current();

		//create directories
		if(! dir.exists(centDir))
		{
			dir.mkdir(centDir);
		}

		if(! dir.exists(centLogFileDir))
		{
			dir.mkdir(centLogFileDir);
		}

		s_logFilePath = centLogFileDir + SEP + CENT_LOG_FILE_NAME;
	}
	//write data
	QFile outFile(s_logFilePath);
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << txt;

#ifdef WIN32
	OutputDebugStringA(txt.toStdString().c_str());
#endif //WIN32
	
	outFile.close();
	if(f_abort)
	{
		abort();
	}
}

#endif