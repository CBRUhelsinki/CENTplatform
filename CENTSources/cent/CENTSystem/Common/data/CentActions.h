#ifndef CENT_ACTIONS_H
#define CENT_ACTIONS_H

namespace CENT
{
	enum ActionType
	{
		Undefined = 0,
		ChangeState,
		SetCurrentGame,
		RunOVDesigner,
		StopOVDesigner,
		SaveQuestionary,
		SetStimulationButton,
		ParseBetaThetaLevelValues,	//TODO: could become more generic in the future, for parsing other OV cfg files
		SaveCoeffValues,
		SaveEvaluation,
		UseDefaultBaseline,
		ShowSettings,
		EndSession
	};

	namespace ActionConfig
	{
		// index of a scenario parameter in parameter list
		const int PARAM_SCENARIO_INDEX = 0;

		const int TRIGGER_ACTION_PARAM_COUNT = 0;
		const int RUN_DESIGNER_PARAM_COUNT = 1;
		const int SET_CURRENT_GAME_PARAM_COUNT = 1;
		const int SAVE_EVALUATION_PARAM_COUNT = 1;
		const int SET_STIMULATION_BUTTON_PARAM_COUNT = 2;
		const int SAVE_COEFF_VALUES_PARAM_COUNT = 2;
	}
}
#endif // CENT_ACTIONS_H
