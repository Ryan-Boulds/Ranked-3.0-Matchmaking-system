//Created by Ryan Boulds
//last edited 3-16-24
#include <iostream>
#include <fstream> 
//#include <array> //unused rn
#include <vector>
#include<windows.h>
#include <iomanip>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/count.hpp>

using namespace std;

// Default settings can be set here:  //This is where the settings file is created.
int createDefaultSettingsFile() {
	ofstream createSettingsFile("settings.txt");
	//information
	createSettingsFile << "Index: bool = b, int = i, double = d, char = c, string = s\n";
	//Default settings can be put here:
	createSettingsFile << "b antiboosting = 1 " << endl;
	createSettingsFile << "b ranklossprevention = 1 " << endl;
	createSettingsFile << "b streakbonus = 1 " << endl;
	createSettingsFile << "i mmrlimit = 700 " << endl; //work on this

	createSettingsFile.close();
	return 0;
}

// This runs anytime that the program wants to confirm if the settings file exists. 
int checkIfSettingsFileExists() {
	//The settings file contents are copied to the string: settingsFileContents
	ifstream settingsFileExists;
	settingsFileExists.open("settings.txt");
	if (!settingsFileExists)
	{
		//create a settings file
		createDefaultSettingsFile();
	}
	settingsFileExists.close();
	return 0;
}

// Find squad average MMR score with anti-boosting calculation.  //This is more for demo purposes now
int findAverageSquadMMRDemo(bool antiBoosting) {
	int numOfPlayersinSquad = 1;
	int playerMMRinput;
	int averageMMR = 0;
	int adjustedAverageMMR = 0;
	int largestMMRScore = -10000000; //Just a placeholder that wont ever be met.
	int MMRLimit = 700;
	vector<int> teamMMR{};

	cout << "input number of players in squad: ";
	cin >> numOfPlayersinSquad;

	//Get input from user of what the MMR of each player is.
	for (int i = 0; i < numOfPlayersinSquad; i++) {
		cout << "Enter player MRR: ";
		cin >> playerMMRinput;
		teamMMR.push_back(playerMMRinput);



		if (largestMMRScore < playerMMRinput && antiBoosting) {
			largestMMRScore = playerMMRinput;
		};


	}
	if (numOfPlayersinSquad != 0) {
		//Calculate the average MMR of the squad.
		for (int i = 0; i < numOfPlayersinSquad; i++) {
			//Without boosting countermeasures
			averageMMR = averageMMR + teamMMR[i];

			//With boosting countermeasure
			if (teamMMR[i] > largestMMRScore - MMRLimit) {
				adjustedAverageMMR = adjustedAverageMMR + teamMMR[i];
			}
			else {
				adjustedAverageMMR = adjustedAverageMMR + (largestMMRScore - MMRLimit);
			}
		}
		averageMMR = averageMMR / numOfPlayersinSquad;
		adjustedAverageMMR = adjustedAverageMMR / numOfPlayersinSquad;
	}
	else {
		cout << "Error!";
	}

	//cout << "With boosting: " << averageMMR << "\nRank after anti-boosting: " << adjustedAverageMMR;
	return adjustedAverageMMR;
} // End of main

// This is a reference for what rank a player is
string rankChecker(int playerMMR, int matchesPlayed) {
	if (playerMMR >= 5000 && matchesPlayed >= 100) return "Champion";
	else if (playerMMR >= 4800) return "Diamond 1";
	else if (playerMMR >= 4600) return "Diamond 2";
	else if (playerMMR >= 4400) return "Diamond 3";
	else if (playerMMR >= 4000) return "Platinum 1";
	else if (playerMMR >= 3600) return "Platinum 2";
	else if (playerMMR >= 3200) return "Platinum 3";
	else if (playerMMR >= 3000) return "Gold 1";
	else if (playerMMR >= 2800) return "Gold 2";
	else if (playerMMR >= 2600) return "Gold 3";
	else if (playerMMR >= 2500) return "Silver 1";
	else if (playerMMR >= 2400) return "Silver 2";
	else if (playerMMR >= 2300) return "Silver 3";
	else if (playerMMR >= 2200) return "Silver 4";
	else if (playerMMR >= 2100) return "Silver 5";
	else if (playerMMR >= 2000) return "Bronze 1";
	else if (playerMMR >= 1900) return "Bronze 2";
	else if (playerMMR >= 1800) return "Bronze 3";
	else if (playerMMR >= 1700) return "Bronze 4";
	else if (playerMMR >= 1600) return "Bronze 5";
	else if (playerMMR >= 1500) return "Copper 1";
	else if (playerMMR >= 1400) return "Copper 2";
	else if (playerMMR >= 1300) return "Copper 3";
	else if (playerMMR >= 1200) return "Copper 4";
	else return "Copper 5";
}

// This is the part that calculates a player's MMR score after a game
vector<int> endOfMatchMMRTally(int playerMMR, int MMRChangeLevel, int winStreak, int loseStreak, bool gameWasWon, int doLongTest, int gamesPlayed, bool rankLossPrevention, bool streakBonusEnabled) {
	int amountMMRChanged;
	int streakBonus = 0;
	int tempVariable = playerMMR;
	//simplifies for equation
	int x = MMRChangeLevel;

	//MMR curve that is not adjustable. limit x=110    // ( (x - 40.5) * (x - 180.5) / 123 ) + 64.9
	//This makes sure that the minimum gained/lost is 25 MMR
	//if (MMRChangeLevel > 110) { MMRChangeLevel = 110; }
	//amountMMRChanged = ((x - 40.5) * (x - 180.5) / 123) + 64.9;

	//This makes sure that the minimum gained/lost is 25 MMR
	if (MMRChangeLevel >= 75) { MMRChangeLevel = 75; }
	amountMMRChanged = -x + 100;

	if (gamesPlayed > 10 && streakBonusEnabled) {
		//Winstreaks factored
		if (winStreak > 2) {
			//work on this part.
			streakBonus = (amountMMRChanged / 25) * sqrt(3 * winStreak);
		}
		else if (loseStreak > 2) {
			streakBonus = (amountMMRChanged / 25) * sqrt(3 * loseStreak);
		}
	}


	//This is where it changes the MMR level
	if (gameWasWon == true) {
		//End losing streak bonus
		if (loseStreak != 0) {
			streakBonus = 0;
		}
		loseStreak = 0;		//resets losestreak to 0 after win.

		//Add MMR to player
		playerMMR = playerMMR + amountMMRChanged + streakBonus;

		//comment for testing
		if (doLongTest == 0) {
			cout << "Added: " << amountMMRChanged << " bonus " << streakBonus << " MMR\n";
		}

		winStreak++;
	}
	else { //if game was lost

		//End winning streak bonus
		if (winStreak != 0) {
			streakBonus = 0;
		}
		winStreak = 0; //resets winstreak to 0 after loss.

		//subtract MMR to player:
		// 
		//rank loss protection accounted for.

			//If player does not derank, MMR will be subtracted normally.
		if (rankChecker(playerMMR, MMRChangeLevel) == rankChecker(playerMMR - amountMMRChanged - streakBonus, MMRChangeLevel) || !rankLossPrevention)
		{
			playerMMR = playerMMR - amountMMRChanged - streakBonus;

			//comment for testing
			if (doLongTest == 0) {
				cout << "subtracted: " << -amountMMRChanged << " bonus " << streakBonus << " MMR\n";
			}
		}
		else { //If derank were to occure without rank loss prevention:
			//derank
			if (playerMMR % 100 == 0) {

				int i = 0;
				do {
					playerMMR--;
					i++;
				} while (playerMMR % 100 != 0 && i < amountMMRChanged + streakBonus);

				//comment for testing
				if (doLongTest == 0) {
					cout << "subtracted: " << playerMMR - tempVariable << " MMR   Rank protected!\n";

					cout << "subtracted: " << -amountMMRChanged << " bonus " << -streakBonus << " MMR\n";
				}
			}
			else //rank loss prevented
			{
				while (playerMMR % 100 != 0) {
					playerMMR--;
				}
				//comment for testing
				if (doLongTest == 0) {
					cout << "subtracted: " << playerMMR - tempVariable << " MMR   Rank protected!\n";
				}
			}
		}
		loseStreak++;
	}

	//Work on this
	gamesPlayed++;
	//streak factored
	if (winStreak < 3 && loseStreak < 3 || gamesPlayed <= 10) {
		MMRChangeLevel++;
	}
	else if (winStreak % 3 == 0 && winStreak >= 4 || loseStreak % 3 == 0 && loseStreak >= 4) {
		MMRChangeLevel++;
	}

	//Return results
	return { playerMMR, MMRChangeLevel, winStreak, loseStreak, gamesPlayed };
}

// This fuction is for testing
int simulation(int whichTest, bool rankLossPrevention, bool streakBonusEnabled) {

	int champion = 0, diamond1 = 0, diamond2 = 0, diamond3 = 0, platinum1 = 0, platinum2 = 0, platinum3 = 0,
		gold1 = 0, gold2 = 0, gold3 = 0, silver1 = 0, silver2 = 0, silver3 = 0, silver4 = 0, silver5 = 0, bronze1 = 0,
		bronze2 = 0, bronze3 = 0, bronze4 = 0, bronze5 = 0, copper1 = 0, copper2 = 0, copper3 = 0, copper4 = 0, copper5 = 0;

	int playerMMR, MMRChangeLevel = 0, winStreak = 0, loseStreak = 0, gamesPlayed = 0;
	int keepTestingThisPlayer = 0;
	//This is how many games are played
	int totalNumberOfGamesToPlay = 100;
	bool keepTesting = 1;
	int timerDuration = 0;
	double matchesWon = 0, matchesLost = 0;
	bool gameWasWon;
	string userInputDuringTest;

	vector<int> output = {};

	while (keepTesting) {

		//reset variables
		if (rankLossPrevention) {
			//If Rank Loss prevention is turned on, the ranks will start lower to account for this making higher ranks harder to achieve.
			playerMMR = 2000;
		}
		else {
			//This is normal matchmaking.
			playerMMR = 2500;
		}

		if (whichTest != 0) {
			totalNumberOfGamesToPlay = rand() % (200 - 15 + 1) + 15;
		}
		if (whichTest == 1) {
			Sleep(1000);
		}

		//This needs work
		while (keepTestingThisPlayer < totalNumberOfGamesToPlay) {
			keepTestingThisPlayer++;

			//Ask the user if the game was won or loss
			if (whichTest == 0) {
				cout << "Are you winning son?" << endl;
				cin >> userInputDuringTest;
				boost::algorithm::to_lower(userInputDuringTest);
				if (userInputDuringTest == "yes" || userInputDuringTest == "win" || userInputDuringTest == "1") {
					gameWasWon = 1;
				}
				else if (userInputDuringTest == "no" || userInputDuringTest == "loss" || userInputDuringTest == "0") {
					gameWasWon = 0;
				}
				else {
					cout << "Ending test..." << endl;
					break;
				}

			}
			if (whichTest != 0) {
				gameWasWon = rand() & 1;
			}
			if (gameWasWon == 1) {
				matchesWon++;
			}
			else {
				matchesLost++;
			}


			//input: playerMMR, MMRChangeLevel, winStreak, loseStreak, bool gameWasWon, doLongTest gamesPlayed
			output = endOfMatchMMRTally(playerMMR, MMRChangeLevel, winStreak, loseStreak, gameWasWon, whichTest, gamesPlayed, rankLossPrevention, streakBonusEnabled);
			//output: playerMMR, MMRChangeLevel, winStreak, loseStreak, gamesPlayed

			playerMMR = output[0];
			MMRChangeLevel = output[1];
			winStreak = output[2];
			loseStreak = output[3];
			gamesPlayed = output[4];
			//output for testing



			if (whichTest == 0) {
				cout << rankChecker(playerMMR, gamesPlayed) << "\nMMR:" << playerMMR << "\nMMR change level: " << MMRChangeLevel << "\nwinstreak: " << winStreak
					<< "\nLosingstreak: " << loseStreak << "\nGames Played: " << gamesPlayed << endl << endl;
			}
		}
		if (whichTest == 1) {
			cout << rankChecker(playerMMR, gamesPlayed) << " " << playerMMR << " MMR. W/L " << setprecision(2) << matchesWon / matchesLost << " Matches: " << totalNumberOfGamesToPlay << endl;
		}

		//Counts ranks
		if (whichTest == 2) {
			if (rankChecker(playerMMR, MMRChangeLevel) == "Champion") {
				champion++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Diamond 1") {
				diamond1++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Diamond 2") {
				diamond2++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Diamond 3") {
				diamond3++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Platinum 1") {
				platinum1++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Platinum 2") {
				platinum2++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Platinum 3") {
				platinum3++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Gold 1") {
				gold1++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Gold 2") {
				gold2++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Gold 3") {
				gold3++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 1") {
				silver1++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 2") {
				silver2++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 3") {
				silver3++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 4") {
				silver4++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 5") {
				silver5++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 1") {
				bronze1++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 2") {
				bronze2++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 3") {
				bronze3++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 4") {
				bronze4++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 5") {
				bronze5++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 1") {
				copper1++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 2") {
				copper2++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 3") {
				copper3++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 4") {
				copper4++;
			}
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 5") {
				copper5++;
			}


			timerDuration++;
			if (timerDuration == 100000) {

				cout << "Champion: " << champion << "\nDiamond 1: " << diamond1 << "\nDiamond 2: " << diamond2 << "\nDiamond 3: " << diamond3 << "\nPlatinum 1: " << platinum1 << "\nPlatinum 2: " << platinum2 <<
					"\nPlatinum 3: " << platinum3 << "\nGold 1: " << gold1 << "\nGold 2: " << gold2 << "\nGold 3: " << gold3 << "\nSilver 1: " << silver1 << "\nSilver 2: " << silver2 << "\nSilver 3: " << silver3 <<
					"\nSilver 4: " << silver4 << "\nSilver 5: " << silver5 << "\nBronze 1: " << bronze1 << "\nBronze 2: " << bronze2 << "\nBronze 3: " << bronze3 << "\nBronze 4: " << bronze4 <<
					"\nBronze 5: " << bronze5 << "\nCopper 1: " << copper1 << "\nCopper 2: " << copper2 << "\nCopper 3: " << copper3 << "\nCopper 4: " << copper4 << "\nCopper 5: " << copper5 << endl << endl;
				Sleep(2000);
				timerDuration = 0;
			}


		} //End of doLongTest == 2


		MMRChangeLevel = 0, winStreak = 0, loseStreak = 0, gamesPlayed = 0, matchesWon = 0, matchesLost = 0, keepTestingThisPlayer = 0;

		if (whichTest == 0 && totalNumberOfGamesToPlay == 100) {
			keepTesting = 0;
		}




		//
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			keepTesting = 0;
		}

	}


	return 0;
}//End of Test

// This is a useful function that takes a string, searches for a whitespace, then seperates the first word from the rest of the string.
vector<string> seperateFirstWordFromString(string orignalString) {
	//Usage:
	//vector<string> seperatedStrings = seperateFirstWordFromString(userInput);
	//FIRSTWORD = seperatedStrings[0];
	//THERESTOFTHESTRING = seperatedStrings[1];

	string firstWord, restOfString;
	int i = 0;
	while (i < orignalString.length() && orignalString[i] != ' ' && orignalString.length() > 0) {

		firstWord = firstWord + orignalString[i];
		i++;
	}

	if (orignalString.length() > i) {
		restOfString = orignalString.substr(i + 1);
	}

	return { firstWord, restOfString };
}

// This is to prevent errors in case a string is suposed to contain all numbers with the only ASCII values being from the numbers 0-9
bool stringContainsAllNumbers(string str) {
	bool allNumbers = true;
	//check to see if input is numbers
	for (char ch : str) {
		if (ch >= 48 && ch <= 57) {
			allNumbers = true;
		}
		else {
			allNumbers = false;
			break;
		}

	}
	return allNumbers;
}


// This writes the new value for a variable into the settings
string changeSetting(string settingName, string settingsFileContents, string newValue) {
	int indexPosition;
	indexPosition = settingsFileContents.find(settingName);
	int cnt = settingName.length();
	int i = 0;

	//If the string settingsFileContents is not long enough, this will fix it
	if (settingsFileContents.size() < indexPosition + cnt + 3 + newValue.size()) {
		settingsFileContents.resize(indexPosition + cnt + 3 + newValue.size(), ' ');
	}
	
	while (i < newValue.size()) {
		settingsFileContents[indexPosition + cnt + 3 + i] = newValue[i];
		
		i++;
	}

	settingsFileContents.erase(indexPosition + cnt + 3 + i, std::string::npos);

	return settingsFileContents;
}

// This tells the program what kind of setting it is going to change.  Example: It is true/false, or a int variable
char settingVariableType(string settingName, string settingsFileContents) {
	//Index: bool = b, int = i, double = d, char = c, string = s 
	char variableType;
	int indexPosition;
	indexPosition = settingsFileContents.find(settingName);
	int cnt = settingName.length();
	variableType = settingsFileContents[indexPosition - 2 ];

	return variableType;
}

// If the input is valid, it will change the variable.  This is only programmed for Boolean and Integers at the time being 3-16-24.
vector<string> validInputOrError(string parameter1, string parameter2, string settingsFileContents, string errorOccured) {
	char variableType = settingVariableType(parameter1, settingsFileContents);
	
	//Index: bool = b, int = i, double = d, char = c, string = s 
	if (variableType == 'b') {
		// True, False, or invalid
		if (parameter2 == "true" || parameter2 == "1" || parameter2 == "enable") {
			//parameter2 true
			settingsFileContents = changeSetting(parameter1, settingsFileContents, "1");
		}
		else if (parameter2 == "false" || parameter2 == "0" || parameter2 == "disable") {
			//parameter2 false
			settingsFileContents = changeSetting(parameter1, settingsFileContents, "0");
		}
		else {
			//parameter2 ????
			errorOccured = "Error";
		}
	}
	else if (variableType == 'i') {

		bool allNumbers = stringContainsAllNumbers(parameter2);

		//check to see if input is numbers
		for (char ch : parameter2) {
			int v = ch; // ASCII Val converted
			if (ch >= 48 && ch <= 57) {
				allNumbers = true;
			}
			else {
				allNumbers = false;
				break;
			}
		}
		if (allNumbers) {
			//parameter2 ###
			settingsFileContents = changeSetting(parameter1, settingsFileContents, parameter2);
		}
		else {
			//parameter2 ????
			errorOccured = "Error!";
		}


		

	}


	vector<string> result = { settingsFileContents, errorOccured };

	return result;

}

// This function calls the settings.txt file and copies it to a string.
string settingsFileContentsToString() {
	//Copy settings file to string.
	ifstream copySettingsToString;
	copySettingsToString.open("settings.txt"); //open the input file
	//Copy the contents of the settings file to a string for changing the settings.
	stringstream strStream;
	strStream << copySettingsToString.rdbuf(); //read the file
	string settingsFileContents = strStream.str(); //str holds the content of the file
	copySettingsToString.close();

	return settingsFileContents;
}

// This function searches the settings.text file for a setting, then it will retrieve the value stored in that setting's variable.  This has problems when mmrlimit is 2 digits
int pullVariableFromSettings(string settingName) {
	
	//This assumes that the setting exists:
	string settings = settingsFileContentsToString();
	string outputString;
	int indexPosition;
	indexPosition = settings.find(settingName);
	int cnt = settingName.length();
	int i = 0;
		
	while (settings[indexPosition + cnt + 3 + i] != '\n') {
		outputString = outputString + settings[indexPosition + cnt + 3 + i];
		if (!stringContainsAllNumbers(outputString)) {
			break;
		}
		i++;
	}
	

	int intResult = stoi(outputString);
	return intResult;

}


// This is the function that is used for when settings are changed.
int setSettings(string userInput) {
	//Settings file location: C:\Users\ryant\source\repos\match simulaator\match simulaator
	string parameter1 = "", parameter2 = "";
	string errorOccured = "No Error";
	//Create settings file if it does not already exist.
#pragma region
//The settings file contents are copied to the string: settingsFileContents
	checkIfSettingsFileExists();
#pragma endregion

	//Get settings file contents and save it to the string settingsFileContents
	string settingsFileContents = settingsFileContentsToString();


	//Here is where you can change the settings.

	//seperate userInput into parameter1 and parameter2
	vector<string> seperatedStrings = seperateFirstWordFromString(userInput);
	parameter1 = seperatedStrings[0];
	parameter2 = seperatedStrings[1];


	//If the setting exists, change setting or error for invalid input
	if (boost::algorithm::contains(settingsFileContents, parameter1)) {

		vector<string> output = {};
		output = validInputOrError(parameter1, parameter2, settingsFileContents, errorOccured);

		settingsFileContents = output[0];
		errorOccured = output[1];
	}

	else if (parameter1 == "reset") {
		string resetComfirmation = "";
		cout << "Are you sure that you want to revert all settigns back to defaults? y/n" << endl;
		cin >> resetComfirmation;
		boost::algorithm::to_lower(resetComfirmation);

		if (resetComfirmation == "y" || resetComfirmation == "yes") {
			createDefaultSettingsFile();
			cout << "\nThe settings were reverted back to default." << endl;
		}
		else {
			cout << "\nThe settings were not changed back to default." << endl;
		}
		//This is for solving a problem
		cin.ignore();
	}
	else if (parameter1 == "help") {
		//do nothing here
	}
	else {
		errorOccured = "Error";
	}


	//If an invalid input or help is written, it will help the user
	if (errorOccured == "Error" || parameter1 == "help") {
		if (errorOccured == "Error") {
			cout << "Error!\n" << endl;;
		}

		cout << "settings list:\n"
			<< "streakbonus <true/false>\n"
			<< "ranklossprevention <true/false>\n"
			<< "antiboosting <true/false>\n";
		errorOccured = "No Error";
	}

	if (parameter1 != "reset") {
		//Save by replacing old settings file with updated settings in String.
		ofstream updatSettingsFile("settings.txt");
		updatSettingsFile << settingsFileContents;
		updatSettingsFile.close();
	}

	//Output settings file contents to show what the settings.txt file says
	cout << settingsFileContentsToString();


	return 0;
}


// Find squad average MMR score with anti-boosting calculation./////////////////////////////////////////////////////////////
int findAverageSquadMMR(vector<vector<int>> squad) {
	squad.erase(squad.begin() + 0);

	vector<int> squadMMR;
	squadMMR.resize(squad.size());
	for (int i = 0; i < squad.size(); i++) { squadMMR[i] = squad[i][1]; };
	bool antiBoosting = pullVariableFromSettings("antiboosting");
	int numOfPlayersinSquad = squadMMR.size();
	int averageMMR = 0;
	int adjustedAverageMMR = 0;
	int largestMMRScore = -10000000; //Just a placeholder that wont ever be met.
	int MMRLimit = pullVariableFromSettings("mmrlimit"); //This will be in settings later. /////////////////////////////////
	

	//Get input from user of what the MMR of each player is.
	for (int i = 0; i < numOfPlayersinSquad; i++) {

		if (largestMMRScore < squadMMR[i] && antiBoosting) {
			largestMMRScore = squadMMR[i];
		};


	}

	//Calculate the average MMR of the squad.
	for (int i = 0; i < squad.size(); i++) {
		//Without boosting countermeasures
		averageMMR = averageMMR + squadMMR[i];

		//With boosting countermeasure
		if (squadMMR[i] > largestMMRScore - MMRLimit) {
			adjustedAverageMMR = adjustedAverageMMR + squadMMR[i];
		}
		else {
			adjustedAverageMMR = adjustedAverageMMR + (largestMMRScore - MMRLimit);
		}
	}
		averageMMR = averageMMR / numOfPlayersinSquad;
		adjustedAverageMMR = adjustedAverageMMR / numOfPlayersinSquad;
	


		return adjustedAverageMMR;
	//cout << "With boosting: " << averageMMR << "\nRank after anti-boosting: " << adjustedAverageMMR;
	//return adjustedAverageMMR;
} // End of main

//Create players for test data until database is created.
vector<vector<int>> playersTestData(int numOfSquads) {
	vector<vector<int>> players;

	//This generates the player's ID and MMR levels
	int i = 0;
	//In the case that all squads have 5 players, this makes sure that there will be enough players to fill each squad.
	while (i < numOfSquads * 5) {
		players.resize(i + 1);
		players[i].resize(2);
		players[i][0] = i;
		players[i][1] = rand() % (5400 - 1000 + 1) + 1000;
		i++;
	} //Format {PlayerID, MMR}

	return players;
}


//working on this right now:
vector<vector<vector<int>>> createSquads() {

	//test data
	//{PlayerID, MMR} (for testing. it should be from a database)
	int numOfSquads = 500;
	vector<vector<int>> squads;
	squads = playersTestData(numOfSquads);

	//Three dementional array.  The first dimension is each squad queing up.  
	//The second dimension contains each other in the squad.
	//The third dimension is each player's playerID and MMR.
	vector<vector<vector<int>>> groupsInCue = {};

	//randonly put playersIDs in squads
	int i = 0;
	int j;
	int k = 0;

	while (i < numOfSquads) {
		groupsInCue.resize(i + 1);
		j = 0;
		//randomly figure out how many people are squading up together

		int squadAverageMMR = 0;
		int numOfPlayersInSquad = trunc(rand() % 5);
		groupsInCue[i].push_back({ -1,0 });
		while (j <= numOfPlayersInSquad) {
			//calcuate the avearage before
			squadAverageMMR = squadAverageMMR + squads[k][1];

			groupsInCue[i].resize(j + 1);
			groupsInCue[i].push_back(squads[k]);
			j++;
			k++; // Goes  through each player
		}


		//Saves the average MMR of the squad for where the squad should be placed in matchmaking.  This can either have antiboosting on or off.
		groupsInCue[i][0][1] = findAverageSquadMMR(groupsInCue[i]);

		i++;
	}
	//At this point, groupsInCue has a list of players cueing together for games. From here, its a matter of matching them together


	// See what is in groupsInCue
	
	for (int i = 0; i < groupsInCue.size(); i++) {
		cout << "Squad " << i + 1 << ":" << endl;
		cout << "IDs: ";
		for (int j = 1; j < groupsInCue[i].size(); j++) {
			cout << groupsInCue[i][j][0] + 1;

			// This is for formatting:		
			int num = groupsInCue[i][j][0];
			if (num == 0) {
				num = 1;
			}
			int spaces = 4 - trunc(log10(num));

			for (int a = 0; a < spaces; a++) {
				cout << " ";
			}
		}

		cout << "\nMMR: ";
		for (int j = 1; j < groupsInCue[i].size(); j++) {
			cout << groupsInCue[i][j][1] << " ";
		}
		if (pullVariableFromSettings("antiboosting") == 1) {
			cout << "Adjusted Average: " << groupsInCue[i][0][1] << " ";
		}
		else {
			cout << "Average: " << groupsInCue[i][0][1] << " ";
		}
		cout << endl << endl;
	}
	


	return groupsInCue;
}



int squadTeamSetUp() {

	vector<vector<vector<int>>> squadsInQue = createSquads();

	// This is the squad that is being made
	

	// Fine multiple squads for testing
	for (int i = 0; i < 20; i++) {


		vector<vector<int>> team;
		int availableSeatsOnTeam = 5;
		//Adding a squad to the team
		for (int squadInQueIndex = 0; squadInQueIndex < squadsInQue.size() && availableSeatsOnTeam > 0; squadInQueIndex++) {
			//While squad is not full and there are squads in the queue

			//Checks if squad will fit on team.  If they do, then it will add them to the team
			if (squadsInQue[squadInQueIndex].size() - 1 <= availableSeatsOnTeam) { // subtract 1 here
				//Adds squad to team
				for (int i = 1; i < squadsInQue[squadInQueIndex].size(); i++) { // start from 1 here
					team.push_back({ squadsInQue[squadInQueIndex][i][0], squadsInQue[squadInQueIndex][i][1] });
				}
				squadsInQue.erase(squadsInQue.begin() + squadInQueIndex);
				availableSeatsOnTeam = 5 - team.size();
				squadInQueIndex = 0; // reset index to start from the beginning of the queue
				
			}
		}

		cout << endl;
		for (int i = 0; i < team.size(); i++) {
			cout << "PlayerID: " << team[i][0] << "  PlayerMMR: " << team[i][1] << endl;
		}

		cout << "\n\n\n";
	}











	//cout << squads.size();


	/*int mmrSearchRestriction = 10;
	while (squad.size() > 5) {


		for (int i = 0; i < squads.size(); i++) {

			if (squads[i][1] > averageMMR - mmrSearchRestriction && squads[i][1] < averageMMR + mmrSearchRestriction) {
				squad.push_back(squads[i]);
				break;
			}


		}


		mmrSearchRestriction = mmrSearchRestriction + 10;

		averageMMR = 0;
		int i = 0;
		while (i < squad.size()) {
			averageMMR = averageMMR + squad[i][2];

			i++;
		}
		averageMMR = averageMMR / (i + 1);
		cout << averageMMR << " " << i;;

	}
	*/

	return 0;
}




//This is set up for solo queuing right now.  //WIP
int soloQueuingTeamSetup() {
	vector<int> playersMMR = { 2000, 2100, 2200, 2300, 2400, 2600, 2700, 2800, 2900, 3000 };
	vector<int> redTeam = { 0,0,0,0,0 };
	vector<int> blueTeam = { 0,0,0,0,0 };
	int i = 0;
	int j = 0;
	int redTally = 0, blueTally = 0;

	//To evenly distribute players between teams (Built for solo queuing players at the moment.)
	while (playersMMR.size() > i) {

		//The lowest 4 players are split evenly and the top 4 players are split evenly. 
		if (i <= 2) {
			redTeam[j] = playersMMR[i];
			blueTeam[j] = playersMMR[i + 1];
		}
		else if (i >= 6) {
			blueTeam[j] = playersMMR[i];
			redTeam[j] = playersMMR[i + 1];
		}

		redTally = redTally + redTeam[j];
		blueTally = blueTally + blueTeam[j];

		i += 2;
		j++;
	}
	//Depending on which team is higher, the higher skilled middle player will go to the team with less average MMR to attempt to make the game more even.
	if (redTally / 4 < blueTally / 4) {
		redTeam[2] = playersMMR[4];
		blueTeam[2] = playersMMR[4 + 1];
	}
	else {
		redTeam[2] = playersMMR[4];
		blueTeam[2] = playersMMR[4 + 1];
	}
	redTally = redTally + redTeam[2];
	blueTally = blueTally + blueTeam[2];


	//displays the results in the console
#pragma region

	i = 0;
	while (blueTeam.size() > i) {
		cout << redTeam[i] << " " << blueTeam[i] << endl;
		i++;
	}
	cout << redTally / 5 << " " << blueTally / 5;
#pragma endregion



}






//This function shows information to the user by showing a list of commands to control the program////////////////////////////
int listOfMainCommands() {
	cout << "\nList of commands:" << endl;
	cout << "averagemmr - Find the average MMR of a squad" << endl;
	cout << "close - This will close the window and end the program." << endl;
	cout << "help - shows the list of commands" << endl;
	cout << "seevariables -  This will check to see what the value is for the variable entered." << endl;
	cout << "set <setting parameter> <true/false> - Change the settings" << endl;
	cout << "simulate <#> - this will test the machmaking algorithm."
		<< "\n     simulate 0 - manually test wins and losses entering 1 for wins and 0 for losses."
		<< "\n     simulate 1 - Shows individual player data results from randomly generated match results. Win rates are 50%."
		<< "\n     simulate 2 - Same as test 1, but tallies final results overtime in with 10,000 players per cycle." << endl;
	
	//cout << "test #" << endl;

	return 0;
}

//This is the main function where you can control the program and what functions are ran
int main() {
	//settings: 
	checkIfSettingsFileExists();
	bool infiniteLoop = 1;
	while (infiniteLoop) {
		bool antiBoostingEnabled = pullVariableFromSettings("antiboosting");
		bool rankLossPrevention = pullVariableFromSettings("ranklossprevention");
		bool streakBonusEnabled = pullVariableFromSettings("streakbonus");

		string command;
		string temp;


		//Get input from the user
		cout << "What command would you like to run? ";
		getline(cin, command);
		//Makes user input all lower case
		boost::algorithm::to_lower(command);

		vector<string> seperatedStrings = seperateFirstWordFromString(command);
		string firstWordOfCommand = seperatedStrings[0];
		string theRestOfCommand = seperatedStrings[1];



		//This is where functions are linked based off of where which commnand was entered by the user
		if (firstWordOfCommand == "simulate") {

			if (theRestOfCommand == "") {
				cout << "Which test would you like to run?\n" <<
					"0 - Individual games\n1 - Player with 50% win rate over time\n2 - Tally players with 50% win rate over time\ntest ";
				cin >> theRestOfCommand;
			}

			if (theRestOfCommand == "0") {
				simulation(0, rankLossPrevention, streakBonusEnabled);
				cin.ignore();
			}
			else if (theRestOfCommand == "1") {
				cout << "Player with 50% win rate over time:\n";
				simulation(1, rankLossPrevention, streakBonusEnabled);
			}
			else if (theRestOfCommand == "2") {
				cout << "Tally players with 50% win rate over time:\n";
				simulation(2, rankLossPrevention, streakBonusEnabled);

			}
			else {
				cout << "Error! invalid input!";
				listOfMainCommands();
				cin.ignore();
			}

		}
		else if (firstWordOfCommand == "averagemmr") {
			cout << "Matchmaking will place you in: " << findAverageSquadMMRDemo(antiBoostingEnabled) << " MMR." << endl;;
			cin.ignore();
		}
		else if (firstWordOfCommand == "set") {
			setSettings(theRestOfCommand);

		}
		else if (firstWordOfCommand == "help") {
			listOfMainCommands();
		}
		else if (firstWordOfCommand == "seevariables") {
			cout << settingsFileContentsToString() << endl;

		}
		else if (firstWordOfCommand == "test") {
			//This is for whatever I am working on at the moment.
			squadTeamSetUp();
		}
		else if (firstWordOfCommand == "test2") {
			//unused rn
			cout << pullVariableFromSettings("mmrlimit");
		}
		else if (firstWordOfCommand == "close") {
			break;
		}
		else {
			cout << "Error! invalid input!";
			listOfMainCommands();
		}
		cout << endl;
	}

	return 0;
}
