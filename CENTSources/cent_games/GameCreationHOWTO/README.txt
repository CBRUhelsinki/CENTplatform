Exemplary CENT games are created with OpenGL and Qt. 
You need:
- Microsoft Visual Studio 2010
- Qt libraries 4.8.0 for Windows (VS 2010) - You find this on http://qt.nokia.com/downloads

There is an API for game developers, providing all needed communication from CENT to the game, and back.
This is an example game for CENT Application.
In this game the player controls the ball by thoughts. The ball rises and changes colors.

Tree of folders:
+---SimpleBallGamePlugin					- Main Game Folder
¦	+---SimpleBallGamePlugin.pro 			- Project file
¦	+---Gameconfiguration.pri	 			- Config file to set paths to the games files
¦	+---src									- Sources and headers file for the game
	¦	+---SimpleBallGameLogic.cpp			- Source file for game logic
	¦	+---SimpleBallGameLogic.h			- Header file for game logic
	¦	+---SimpleBallGamePlugin.cpp		- Source file for main game class
	¦	+---SimpleBallGamePlugin.h			- Header file for main game class
	¦	+---SimpleBallGameSummary.cpp		- Source file for main game class
	¦	+---SimpleBallGameSummary.h			- Header file for main game class
	¦	+---sources.pri						- Config file to set paths to the games files
+---Common									- Additional classes for the game
¦	+---CentDataTypes.h						- Header is structure for data from OpenViBE game scenario
¦	+---GamePluginConf.pri					- Pri file - to set paths to the games files
¦	+---simpleshapedrawer.cpp				- Source file for simple class which is a drawer for the objects in OpenGL
¦	+---simpleshapedrawer.h					- Source file for simple class which is a drawer for the objects in OpenGL
¦	+---Connect.cpp							- Source file to connect Qt signals and slots
¦	+---Connect.h							- Header file to connect Qt signals and slots
¦	+---Interfaces							- Interface for game
	¦	+---ICentGamePlugin.h				- Interface for create new games in CENT Application

First take a look at ICentGamePlugin.h, then at SimpleBallGamePlugin.h and SimpleBallGamePlugin.cpp where the game is implemented.
All files have comments.

A dll file with game copy to CENT\plugins directory, also you can add tutorial to CENT\plugins\Tutorials\[GAMENAME]_tutorial with index.html and any additional files in it. 
CENT should automatically find the files and add the game to application. 

You can also provide a [GAMENAME].ini file with default game time with something like "GameDurationTime=300" in it, this one says that the game will end after 300 seconds.