#ifndef CENT_STATE_ENUMS_H
#define CENT_STATE_ENUMS_H


	enum CentState
	{
		ControlStateUndefined = -3,
		ControlStateNext,
		ControlStatePrevious,
		CentNotInitializedState,
		CentWelcomeState,
		CentChooseScenarioState,
		CentCapSetupState,
		CentBaseLineState,
		CentBaseLineInfoState,
		CentGameSelectState,
		CentTutorialState,
		CentGameState,
		CentSummaryState,
		CentFeedbackState,
		CentEvaluationState,
		CentFinishState,
		CentPatientQuestionaryState,

		CentInvalidState
	};

	enum CentUiState
	{
		UiNotInitialized = 0,
		UiWelcomeState,
		UiChooseScenarioState,
		UiCapSetupState,
		UiBaseLineState,
		UiBaseLineInfoState,
		UiGameSelectState,
		UiTutorialState,
		UiGameState,
		UiSummaryState,
		UiEvaluationState,
		UiPatientQuestionaryState
	};

#endif
