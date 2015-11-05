#ifndef PATIENT_DATA_CENTER_CONFIG_H
#define PATIENT_DATA_CENTER_CONFIG_H


namespace PatientDataCenterConstants
{
	//Patient questionary data tags
	const QString PATIENT_NAME_SETTING           ("Patient name");
	const QString HOURS_SLEPT_SETTING            ("Hours slept");
	const QString HOURS_SINCE_LAST_SLEEP_SETTING ("Hours since last sleep");
	const QString MOTIVATION_SETTING             ("Motivation");
	const QString MOOD_SETTING                   ("Mood");
	const QString EXITEMENT_SETTING              ("Excitement");
	const QString SESSION_DATE_SETTING           ("Session date");


	// Games locations
	const QString GAMES_DIR                   ("games");
	const QString ONE_GAME_DIR                ("game_%1_%2");//game_[game name]_[YYYYMMDDHHSS]
	const QString ONE_GAME_SUMMARY_FILENAME   ("game_summary.txt");
	const QString ONE_GAME_PARAMETERS_FILENAME("game_parameters.txt");
	const QString SESSION_SCORE_FILENAME      ("session_score.txt");
	const QString SESSION_SUMMARY_FILENAME    ("session_summary.txt");

	// Games formatters 
	const QString GAME_SCORE_FORMATTER   ("%1");
	const QString SUMMARY_SCORE_FORMATTER("%1\n");

	// Game parameters tags
	const QString GAME_BETA_COEFF_USED  = "beta_coefficient";
	const QString GAME_THETA_COEFF_USED = "theta_coefficient";

	// Game summary tags
	const QString GAME_SCORE_SETTING = "game_score";
	const QString GAME_NAME_SETTING  = "game_name";

	// Session Summary tags
//	const QString SESSION_DATE_SETTING        = "session_date";
	const QString SESSION_SCORE_SETTING       = "session_average_score";
	const QString SESSION_EFFORT_SETTING      = "Effort";
	const QString SESSION_FRUSTRATION_SETTING = "Frustration";

	// Common formatters
	const QString DATE_FORMATTER("yyyyMMddHHmmss");

	// File names
	const QString PATIENT_DATA_FILENAME    ("patient_condition.txt");
	const QString BASELINE_GDF_FILENAME    ("baseline.gdf");
	const QString GAME_SUMMARY_FILENAME    ("game_summary.txt");
	const QString SESSION_SUMMARY          ("session_summary.txt");

	//Required registry keys
	const QString MY_DOCUMENTS_REGISTRY_KEY("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders");
	const QString MY_DOCUMENTS_REGISTRY_VALUE("Personal");

	// File locations
	const QString BASE_DIR                           ("CENT");
	const QString PATIENTS_DIR                       ("Patients");
	const QString IEP_DIR                            ("IEP");
	const QString DEFAULT_BASELINE_DIR               ("default_baseline");
	const QString PATIENT_DIRECTORY_FORMATTER        ("%1");// [patient name]
	const QString PATIENT_SESSION_DIRECTORY_FORMATTER("session_%1");// session_[YYYYMMDDHHSS]
	const QString GAMES_DIRECTORY                    ("games");
	const QString GAME_DIRECTORY_FORMATTER           ("game_%1_%2");// game_[game name]_[YYYYMMDDHHSS]
	const QString GAME_GDF_FILENAME                  ("game.gdf");

	const QString UNIMPLEMENTED("Not yet implemented;");

	// OpenVibe configuration xml tags
	const QString OV_CONFIG_OPENING_TAG  ("<OpenViBE-SettingsOverride>");
	const QString OV_CONFIG_CLOSING_TAG  ("</OpenViBE-SettingsOverride>");
	const QString OV_SETTING_OPENING_TAG ("<SettingValue>");
	const QString OV_SETTING_CLOSING_TAG ("</SettingValue>");
	const QString OV_SETTING_FORMATTER   (OV_SETTING_OPENING_TAG + "%1" + OV_SETTING_CLOSING_TAG);//[the value of setting]

	// OpenVibe configuration file names
	const QString OV_CONGIG_DIR           ("Config");
	const QString BETA_LEVEL_FILENAME     ("betabase.cfg");
	const QString THETA_LEVEL_FILENAME    ("thetabase.cfg");
	const QString BETA_COEFF_FILENAME     ("BetaCoeff.cfg");
	const QString THETA_COEFF_FILENAME    ("ThetaCoeff.cfg");
}
#endif //PATIENT_DATA_CENTER_CONFIG_H
