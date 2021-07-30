#include "Minion.hpp"
#include "MasterBoard.hpp"
#include "inputLayer.hpp"
#include "mainmenu.h"
#include <iostream>
#include <ctype.h>
#include <fstream>
#include <windows.h>
#include "compie.hpp"

class inputLayer;
class MasterBoard;

 
//Messes up minions!
//Still need to add them after.
//This lumps all the terrain at the end, need to find a way to count neighbors and based on those, distribute tiles.
int mainMenu::scrambleMap(MasterBoard* LoadBoard, inputLayer* InputLayer)
{
	LoadBoard->clearBoard(InputLayer);

	int numberOfHillsAndMountains = BOARD_SIZE / 10;	//Actual amount you want to see.
	int mountainWeight = numberOfHillsAndMountains;		//This will change throughout function
	int numberOfForests = BOARD_SIZE / 10;
	int forestWeight = numberOfForests;
	int plainsWeight = BOARD_SIZE - numberOfHillsAndMountains - numberOfForests;

	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			LoadBoard->Board[x][y].clearTile();
			int mountainChance = (rand() % 100) * mountainWeight;
			int forestChance = (rand() % 100) * forestWeight;
			int plainsChance = (rand() % 100) * plainsWeight;
			if (mountainChance > forestChance && mountainChance > plainsChance)
			{
				if ((rand() % 100) < 50)
				{
					LoadBoard->Board[x][y].symbol = 'M';
					LoadBoard->Board[x][y].description = "Mountain.";
					LoadBoard->Board[x][y].defenseFactor = 1.4;
				}
				else
				{
					LoadBoard->Board[x][y].symbol = '^';
					LoadBoard->Board[x][y].description = "Hill.";
					LoadBoard->Board[x][y].defenseFactor = 1.1;
				}
				mountainWeight--;
			}
			else if (forestChance > plainsChance)
			{
				LoadBoard->Board[x][y].symbol = '+';
				LoadBoard->Board[x][y].description = "Forest.";
				LoadBoard->Board[x][y].defenseFactor = 1.2;
				forestWeight--;
			}
			else plainsWeight--;
		}
	}
	return 0;
}

int mainMenu::setCharacteristics(MasterBoard* LoadBoard)
{
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			switch (LoadBoard->Board[x][y].symbol)
			{
			case('.'):
			{
				LoadBoard->Board[x][y].description = "Clear terrain.";
				LoadBoard->Board[x][y].defenseFactor = 1.1;
				LoadBoard->Board[x][y].Image = { '.','.','.',
													'.','.','.',
													'.','.','.' };
				break;
			}
			case('H'):
			{
				LoadBoard->Board[x][y].description = "City.";
				LoadBoard->Board[x][y].defenseFactor = 1.3;
				LoadBoard->Board[x][y].production = 2000;
				LoadBoard->Board[x][y].Image = { 'H',' ','H',
													'H',' ','H',
													'H',' ','H' };
				break;
			}
			case('m'):
			{
				LoadBoard->Board[x][y].description = "Mine.";
				LoadBoard->Board[x][y].defenseFactor = 1.2;
				LoadBoard->Board[x][y].production = 3000;
				LoadBoard->Board[x][y].Image = { ' ','_',' ',
													'/','n','\\',
													'.','.','.' };
				break;
			}
			case('n'):
			{
				LoadBoard->Board[x][y].description = "Settlement.";
				LoadBoard->Board[x][y].defenseFactor = 1.3;
				LoadBoard->Board[x][y].production = 1000;
				LoadBoard->Board[x][y].Image = { 'n','.','n',
													'.','.','.',
													'n','.','n' };
				break;
			}
			case('h'):
			{
				LoadBoard->Board[x][y].description = "Factory.";
				LoadBoard->Board[x][y].defenseFactor = 1.3;
				LoadBoard->Board[x][y].production = 1000;
				LoadBoard->Board[x][y].Image = { '|','|','|',
													'H','H','H',
													'H','H','H' };
				break;
			}
			case('Q'):
			{
				LoadBoard->Board[x][y].description = "Headquarters.";
				LoadBoard->Board[x][y].defenseFactor = 1.4;
				LoadBoard->Board[x][y].production = 1000;
				LoadBoard->Board[x][y].Image = { '|','*','|',
													'|','H','|',
													'|','H','|' };
				break;
			}
			case('='):
			{
				LoadBoard->Board[x][y].description = "Road.";
				LoadBoard->Board[x][y].defenseFactor = 1.0;
				LoadBoard->Board[x][y].Image = { '=',' ','=',
													'=',' ','=',
													'=',' ','=' };
				break;
			}
			case('^'):
			{
				LoadBoard->Board[x][y].description = "Hill.";
				LoadBoard->Board[x][y].defenseFactor = 1.1;
				LoadBoard->Board[x][y].Image = { '/','\\','.',
													'.','/','\\',
													'/','\\','.' };
				break;
			}
			case('M'):
			{
				LoadBoard->Board[x][y].description = "Mountain.";
				LoadBoard->Board[x][y].defenseFactor = 1.4;
				LoadBoard->Board[x][y].Image = { ' ','^',' ',
													'/','_','\\',
													'.','.','.' };
				break;
			}
			case('+'):		//Would like to have convertible to woodlot by engineer.....maybe
			{
				LoadBoard->Board[x][y].description = "Forest.";
				LoadBoard->Board[x][y].defenseFactor = 1.2;
				LoadBoard->Board[x][y].Image = { '^','^','^',
													'^','^','^',
													'|','|','|' };
				break;
			}
			}
			//Other terrain types go here

		}
	}
	return 0;
}

int mainMenu::gameSave(std::string inputSaveGameName, MasterBoard* boardToPrint)
{
	std::ofstream saveGame(inputSaveGameName);

	//Unique to save_game vs scenario. First save number of players, and then player names (User names):
	saveGame << NUMBEROFPLAYERS << std::endl;
	for (int i = 1; i <= NUMBEROFPLAYERS; i++)
	{
		saveGame << playerNames[i] << std::endl;
	}
	
	//Then save the game turn.
	saveGame << gameTurn << std::endl;
	
	//Then save the map size:
	saveGame << BOARD_WIDTH << std::endl;
	saveGame << BOARD_HEIGHT << std::endl;

	//Then save whos turn it is:
	saveGame << boardToPrint->playerFlag << std::endl;

	//Terrain save:
	//Iterate through board and save the exact symbol.
	//Saving symbol and controller separately for ease of edit and viewing.
	//Only pain point is do mass control-edits, but viewability is protected.
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			saveGame << boardToPrint->Board[x][y].symbol;
		}
		saveGame << std::endl;
	}

	//Iterate through board and save the controller.
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			saveGame << boardToPrint->Board[x][y].controller;

		}
		saveGame << std::endl;
	}

	//Note the number of minions:
	saveGame << boardToPrint->totalNumberOfMinions << std::endl;

	//Go through entire minionRoster and save each value associated with each minion, one line per minion.
	//Need to ensure correctness.
	//FIX FIX FIX - need to not just check for initial NULL, there may be minions beyond that first NULL if a guy was killed and not replaced on
	//The array.
	for (int i = 0; i < GLOBALSUPPLYCAP; i++)
	{
		if (boardToPrint->minionRoster[i] != NULL)
		{
			saveGame << boardToPrint->minionRoster[i]->type << " "
				<< boardToPrint->minionRoster[i]->locationX << " "
				<< boardToPrint->minionRoster[i]->locationY << " "
				<< boardToPrint->minionRoster[i]->team << " "
				<< boardToPrint->minionRoster[i]->seniority << " "
				<< int(boardToPrint->minionRoster[i]->status) << " "
				<< int(boardToPrint->minionRoster[i]->health) << std::endl;
		}
	}
	saveGame.close();
	return 1;
}

//Scenario Load is for new scenarios from a non-saved game.
//Game Load is for saved games, which already have player data.
int mainMenu::gameLoad(MasterBoard* boardToPrint, inputLayer* InputLayer, compie* ComputerPlayer, std::ifstream* saveGame)
{ 
	
	int replaceWithNumberOfPlayers = 0;

	//This is the first data we will see. Need to figure out how to replace NUMBEROFPLAYERS.
	//As of right now this value will not be used.
	*saveGame >> replaceWithNumberOfPlayers;
	
	//Unique to save_game vs scenario. Load player names (User names):
	for (int i = 1; i <= NUMBEROFPLAYERS; i++)
	{
		*saveGame >> playerNames[i];
	}

	//Then load the game turn.
	*saveGame >> gameTurn;

	
	//Although I don't love the name scenarioLoad, this is performing the same action as scenarioLoad so we're using it as good practice.
	scenarioLoad(boardToPrint, InputLayer, ComputerPlayer, saveGame);
	return 0; 

}

//Load scenario game and initialize the board with its contents.
int mainMenu::scenarioLoad(MasterBoard* boardToPrint, inputLayer* InputLayer, compie* ComputerPlayer, std::ifstream* saveGame) {

	//Clear board in case scenario load was called by player menu later in game.
	boardToPrint->clearBoard(InputLayer);
	InputLayer->computerPlayer = ComputerPlayer;
	ComputerPlayer->InputLayer = InputLayer;
		
	std::string line;
	int garb1, garb2;
	
	//First load the map size:
	//Ideally we can create new vector or whatever to have different map size:
	*saveGame >> garb1;
	*saveGame >> garb2;

	//Then load player data:
	//This is the current player whos turn it is:
	*saveGame >> boardToPrint->playerFlag;
	
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{

		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			*saveGame >> boardToPrint->Board[x][y].symbol;
		}
	}
	setCharacteristics(boardToPrint);

	//This is for properties.
	char inputProperties;
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{

		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			*saveGame >> inputProperties;
			boardToPrint->Board[x][y].controller = (int(inputProperties)) - 48;
		}
	}
	//Then load minion data:
	int numberOfMinions;
	*saveGame >> numberOfMinions;
	int health, locationX, locationY, team, seniority, status;
	char type;
	for (int y = 0; y < numberOfMinions; y++)
	{
		*saveGame >> type
			>> locationX
			>> locationY
			>> team
			>> seniority
			>> status
			>> health;
		boardToPrint->createMinion(type, locationX, locationY, team, health, status);
	}

	saveGame->close();
	return 1;
}

int mainMenu::playGame(MasterBoard* boardToPlay, inputLayer* InputLayer, compie* ComputerPlayer, WINDOW* mywindow)
{
	clear();
	addstr("Line of Command\n");
	addstr("Copyright 2021, Park Family Software Laboratory (ParkLab)\n");
	addstr("Press any key to continue.");
	refresh();

	char Input = ' ';

	//Run as long as the user wants. Infinite while loop.
	while (true)		
	{
	
		Input = wgetch(mywindow);

		//If we're still on top menu, do that instead of game/inputLayer.
		if (menuStatus == topmenu)
		{
			printTopMenu();
			topMenuInput(&Input, boardToPlay, InputLayer, ComputerPlayer, mywindow);
		}

		//If we are waiting for remote, do that instead of game/inputLayer.
		if (menuStatus == waitingForRemotePlayer)
		{
			printWaitingScreen();
			waitForRemotePlayer(boardToPlay, InputLayer, ComputerPlayer);
		}
		
		if(menuStatus == playingMap)
		{

			if (InputLayer->status == gameBoard)
			{
				InputLayer->gameBoardInput(&Input, boardToPlay);
			}
			else if (InputLayer->status == minionAction)
			{
				InputLayer->minionInput(&Input, boardToPlay);
			}
			else if (InputLayer->status == menu)
			{
				InputLayer->menuInput(&Input, boardToPlay);
			}
			else if (InputLayer->status == propertyAction)
			{
				InputLayer->propertyMenuInput(&Input, boardToPlay);
			}

			boardToPlay->checkWindow();
			InputLayer->printScreen(boardToPlay);

		}
	}
	return 0;
}

int mainMenu::topMenuInput(char* Input, MasterBoard* boardToPlay, inputLayer* InputLayer, compie* ComputerPlayer, WINDOW* mywindow)
{
	//If user wants to load a map.
	//Need to deal with COUTs. the're inappropriate.
	if (*Input == 'l') 
	{
		topMenuLoad(Input, boardToPlay, InputLayer, ComputerPlayer, mywindow);
	}


	//This is next objective: Figure out "new map", with username, etc.
	if (*Input == 'n')
	{
		topMenuNew(Input, boardToPlay, InputLayer, ComputerPlayer, mywindow);
	}
	
	//Join a remote game
	if (*Input == 'j')
	{
		topMenuJoin(boardToPlay, InputLayer, ComputerPlayer, mywindow);
	}

	return 0;
}

int mainMenu::printTopMenu()
{
	clear();
	addstr("Main Menu\n");
	addstr("Load saved game (l) | Start new game (n) | Join remote game (j) \n");
	refresh();
	return 0;
}

int mainMenu::printWaitingScreen() 
{
	clear();
	addstr("Waiting for other player(s) \n");
	refresh();
	return 0;
}

int mainMenu::topMenuNew(char* Input, MasterBoard* boardToPlay, inputLayer* InputLayer, compie* ComputerPlayer, WINDOW* mywindow)
{	
	//Determine if game is remote or local.
	addstr("Local (l) or remote (r) game?\n");
	gameType = unchosen;
	while (gameType == unchosen)
	{
		*Input = wgetch(mywindow);

		if (*Input == 'l')
		{
			clear();
			addstr("Local game selected.\n");
			refresh();
			gameType = local;
		}
		else if (*Input == 'r') 
		{
			clear();
			addstr("Remote game selected.\n");
			refresh();
			gameType = remote;
		}
	}

	//Load the actual map
	std::ifstream newGameMap;
	char scenarioToLoad[100];
	char* pointToScenarioName = &scenarioToLoad[0];
	bool loadsuccessful = false;

	//Prompt user and load scenario
	while (loadsuccessful == false)
	{
		addstr("Choose which scenario to load (Case sensitive): \n");
		getstr(pointToScenarioName);
		std::string newScenario = scenarioToLoad;
		newGameMap.open(".\\scenarios\\"+ newScenario + ".txt");
		if (newGameMap.is_open())
		{
			addstr("Successfully loaded!\n");
			loadsuccessful = true;
		}
		else
		{
			addstr("Could not load scenario. Please check that it exists and the right spelling was used.\n");

		}
	
	}
	//Actually load scenario. Initialize board, etc.
	scenarioLoad(boardToPlay, InputLayer, ComputerPlayer, &newGameMap);

	//Determine player names for number of players
	//Currently this is 2:
	//We added one to the array, just like treasury, for easy access.
	char inputName[100];
	char outputName[100];
	for (int i = 1; i <= NUMBEROFPLAYERS ; i++) 
	{
		snprintf(&outputName[0], 100, "Input Player %d's name: \n", i);
		addstr(&outputName[0]);
		getstr(&inputName[0]);
		playerNames[i] = inputName;
	}

	//Get session name:
	char inputSessionName[100];
	addstr("Input session name:\n");
	getstr(&inputSessionName[0]);
	sessionName = inputSessionName;
	


	menuStatus = playingMap;
	return 0;
}

int mainMenu::topMenuLoad(char* Input, MasterBoard* boardToPlay, inputLayer* InputLayer, compie* ComputerPlayer, WINDOW* mywindow)
{ 
	//Determine if game is remote or local.
	addstr("Local (l) or remote(r) game?\n");
	gameType = unchosen;
	while (gameType == unchosen)
	{
		*Input = wgetch(mywindow);

		if (*Input == 'l')
		{
			clear();
			addstr("Local game selected.\n");
			refresh();
			gameType = local;
		}
		else if (*Input == 'r')
		{
			clear();
			addstr("Remote game selected.\n");
			refresh();
			gameType = remote;
		}
	}

	//Load the actual save game
	//This garbage is necessary because for some reason, getstr will not play with strings, even if you provide & and [0]
	std::ifstream loadGameSave;
	std::string saveToLoad;
	char scenarioToLoad[100];
	char* pointToSaveName = &scenarioToLoad[0];
	bool loadsuccessful = false;

	//Prompt user and load scenario
	while (loadsuccessful == false)
	{
		addstr("Choose which save game to load (Case sensitive. Do not use _save portion of save.): \n");
		getstr(&pointToSaveName[0]);
		saveToLoad = scenarioToLoad;
		saveToLoad += "_save.txt";
		loadGameSave.open(".\\savegames\\" + saveToLoad);
		if (loadGameSave.is_open())
		{
			addstr("Save game successfully loaded!\n");
			loadsuccessful = true;
		}
		else
		{
			addstr("Could not load save game. Please check that it exists and the right spelling was used.\n");

		}

	}
	//Actually load scenario. Initialize board, etc.
	gameLoad(boardToPlay, InputLayer, ComputerPlayer, &loadGameSave);

	//Get session name:
	char inputSessionName[100];
	addstr("Input session name:\n");
	getstr(&inputSessionName[0]);
	sessionName = inputSessionName;



	menuStatus = playingMap;
	return 0;
}

int mainMenu::topMenuJoin(MasterBoard* boardToPlay, inputLayer* InputLayer, compie* ComputerPlayer, WINDOW* mywindow) 
{
	//Set flags to remote and waiting for first turn.
	gameType = remote;
	joiningFirstTurn = true;

	//Get session name:
	char inputSessionName[100];
	addstr("Input session name:\n");
	getstr(&inputSessionName[0]);
	sessionName = inputSessionName;

	waitForRemotePlayer(boardToPlay, InputLayer, ComputerPlayer);

	return 0;
}

int mainMenu::multiplayerPushSaveGame(MasterBoard* boardToSave)
{
	//Should save the current game into the multiplayer folder, and then call the bat file to push to remote.
	//Ensure game is saved into the right directory.
	//This is different from local saves. Thus must handle save names before passing to saveGame.
	std::string saveGameName = ".\\multiplayer\\lineOfCommandMultiplayer\\";
	saveGameName += sessionName;
	saveGameName += "_save.txt";
	gameSave(saveGameName, boardToSave);

	//Call .bat to push.
	std::string batchPushCommand = "pushSaveToServer.bat";
	system(&batchPushCommand[0]);

	return 0;

}

int mainMenu::multiplayerPullSaveGame() 
{
	//Should attempt to pull and then compare the pulled filename against some previous quantity, for now, gameturn and playerturn.

	//Call pullSaveFromServer.bat to pull.
	std::string batchPullCommand = "pullSaveFromServer.bat";
	system(&batchPullCommand[0]);
	return 0;
}

int mainMenu::waitForRemotePlayer(MasterBoard* boardToSave, inputLayer* InputLayer, compie* ComputerPlayer)
{

	int oldFilePlayerTurn = 0;
	int oldFileGameTurn  = 0;

	//We are waiting for updated save game.
	bool receivedUpdate = false;

	if (joiningFirstTurn == false)
	{
		//First save the "old" values, to compare against whatever we load.
		int oldFilePlayerTurn = boardToSave->playerFlag;
		int oldFileGameTurn = gameTurn;

	}


	while (receivedUpdate == false)
	{
		//The batch file will wait 5 real world seconds.
		multiplayerPullSaveGame();

		//Then load game and see if it's changed at all.
		std::ifstream loadGameSave;
		loadGameSave.open(".\\multiplayer\\lineOfCommandMultiplayer\\" + sessionName + "_save.txt");
		
		//Have to successfully open the game file before we can examine it!
		if (loadGameSave.is_open() == true) 
		{
			gameLoad(boardToSave, InputLayer, ComputerPlayer, &loadGameSave);
			//If either have changed we have successfully gotten a different file.
			if (joiningFirstTurn == true || oldFilePlayerTurn != boardToSave->playerFlag || oldFileGameTurn == gameTurn)
			{
				joiningFirstTurn = false;
				receivedUpdate = true;
			}
		}



	}

	menuStatus = playingMap;
	return 0;
}