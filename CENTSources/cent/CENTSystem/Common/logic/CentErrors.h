#ifndef CENT_ERRORS_H
#define CENT_ERRORS_H

typedef enum ErrorCodes
{
	DefaultError = -200,
	ParamCount,
	NoExecutor,
	InternalFatal,
	GameUnavailable,
	GameManagerUnavailable,
	VrpnServerUnavailable,
	DataCenterUnavailable,
	SessionNotesLauncherUnavailable,
	SessionNotesRunning,
	WriteConfigFailed,
	BaselineCopyError,
	ArgumentsError,
	SaveFileFailed,

	Success = 0
}CentErrorCodes;
#endif // CENT_ERRORS_H
