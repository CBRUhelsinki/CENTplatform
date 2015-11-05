#include "SessionNotesLauncher.h"

#include "Connect.h"
#include <QAction>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcess>

namespace
{
	const QString NOTEPAD_EXE_NAME       = "notepad.exe";
	const QString SESSION_NOTES_FILENAME = "session_notes.txt";
}

SessionNotesLauncher::SessionNotesLauncher(QObject* parent)
	: QObject(parent)
	, m_currentSessionPath()
	, m_sessionNotesAction(0)
	, m_notepadProcess(new QProcess())
{
}

SessionNotesLauncher::~SessionNotesLauncher()
{
	delete m_notepadProcess;
}

void SessionNotesLauncher::setSessionNotesAction(QAction* action)
{
	if(m_sessionNotesAction)
	{
		CENT::disconnect(m_sessionNotesAction, SIGNAL(triggered()), this, SLOT(launchNotepad()));
	}
	m_sessionNotesAction = action;
	CENT::connect(m_sessionNotesAction, SIGNAL(triggered()), this, SLOT(launchNotepad()));
}

void SessionNotesLauncher::setNewSessionPath(const QString& path)
{
	m_currentSessionPath = path;
	if(m_notepadProcess->state() == QProcess::Running)
	{
		qDebug() << "sendign close signal to open notepad process";
		m_notepadProcess->close(); // we don't wan't more than one notepad open not to confuse the therapist
	}
	if(m_sessionNotesAction)
	{
		if(path.isEmpty())
		{ // sesion has ended
			m_sessionNotesAction->setEnabled(false);
		}
		else
		{
			m_sessionNotesAction->setEnabled(true);
		}
	}
	else
	{
		qCritical() << "No action set before setting session path";
	}
}

void SessionNotesLauncher::launchNotepad()
{
	if(m_notepadProcess->state() == QProcess::Running)
	{
		qWarning() << "User tried to open the same file twice";
	}
	else
	{
		QDir dir;
		if(!dir.exists(m_currentSessionPath))
		{
			qCritical() << "Session path incorrect";
			return;
		}
		QString filename(m_currentSessionPath + QDir::separator() + SESSION_NOTES_FILENAME);
		QFile file(filename);
		if(!file.exists())
		{
			if(!file.open(QIODevice::WriteOnly))
			{
				qCritical() << "Failed to create session_notes.txt file";
				return;
			}
			file.write("Session notes:\n");
			file.close();
		}
		QStringList params;
		params << filename;
		m_notepadProcess->start(NOTEPAD_EXE_NAME, params);
	}
}

bool SessionNotesLauncher::canEndSession()
{
	bool canEnd = true;
	if(m_notepadProcess->state() == QProcess::Running)
	{
		canEnd = false;
	}
	return canEnd;
}
