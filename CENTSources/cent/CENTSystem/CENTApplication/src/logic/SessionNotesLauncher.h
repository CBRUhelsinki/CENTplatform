#ifndef SESSION_NOTES_LAUNCHER_H
#define SESSION_NOTES_LAUNCHER_H

#include <QObject>

class QAction;
class QProcess;

class SessionNotesLauncher : public QObject
{
	Q_OBJECT
public:
	explicit SessionNotesLauncher(QObject* parent = 0);
	virtual ~SessionNotesLauncher();

	void setSessionNotesAction(QAction* action);
	bool canEndSession();

public slots:
	void setNewSessionPath(const QString& path);

private slots:
	void launchNotepad();

private:
	QString   m_currentSessionPath;
	QAction*  m_sessionNotesAction;
	QProcess* m_notepadProcess;
};
#endif // SESSION_NOTES_LAUNCHER_H