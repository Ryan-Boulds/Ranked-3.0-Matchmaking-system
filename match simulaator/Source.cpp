//Created by Ryan Boulds
//last edited 3-27-24


#include <iostream>
#include <fstream>
#include <conio.h>
#include <utility>
#include <vector>
#include<windows.h>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/count.hpp>
#include <array>
#include <random>


using namespace std;

int randomNumberGenerator(int lowRange, int highRange) {
	random_device rd; // obtain a random number from hardware
	mt19937 gen(rd()); // seed the generator
	uniform_int_distribution<> distr(lowRange, highRange); // define the range

	return distr(gen);
}

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
	if (!settingsFileExists) {

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
		}
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
	if (playerMMR >= 4800) return "Diamond 1";
	if (playerMMR >= 4600) return "Diamond 2";
	if (playerMMR >= 4400) return "Diamond 3";
	if (playerMMR >= 4000) return "Platinum 1";
	if (playerMMR >= 3600) return "Platinum 2";
	if (playerMMR >= 3200) return "Platinum 3";
	if (playerMMR >= 3000) return "Gold 1";
	if (playerMMR >= 2800) return "Gold 2";
	if (playerMMR >= 2600) return "Gold 3";
	if (playerMMR >= 2500) return "Silver 1";
	if (playerMMR >= 2400) return "Silver 2";
	if (playerMMR >= 2300) return "Silver 3";
	if (playerMMR >= 2200) return "Silver 4";
	if (playerMMR >= 2100) return "Silver 5";
	if (playerMMR >= 2000) return "Bronze 1";
	if (playerMMR >= 1900) return "Bronze 2";
	if (playerMMR >= 1800) return "Bronze 3";
	if (playerMMR >= 1700) return "Bronze 4";
	if (playerMMR >= 1600) return "Bronze 5";
	if (playerMMR >= 1500) return "Copper 1";
	if (playerMMR >= 1400) return "Copper 2";
	if (playerMMR >= 1300) return "Copper 3";
	if (playerMMR >= 1200) return "Copper 4";
	return "Copper 5";
}

// This is the part that calculates a player's MMR score after a game
vector<int> endOfMatchMMRTally(int playerMMR, int MMRChangeLevel, int winStreak, int loseStreak, bool gameWasWon,
	int doLongTest, int gamesPlayed, bool rankLossPrevention, bool streakBonusEnabled) {

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

		//Win streaks factored
		if (winStreak > 2) {

			//work on this part.
			streakBonus = (amountMMRChanged / 25) * sqrt(3 * winStreak);
		}
		else if (loseStreak > 2) {

			streakBonus = (amountMMRChanged / 25) * sqrt(3 * loseStreak);
		}
	}


	//This is where it changes the MMR level
	if (gameWasWon) {

		//End losing streak bonus
		if (loseStreak != 0)
		{
			streakBonus = 0;
		}
		loseStreak = 0; //resets lose streak to 0 after win.

		//Add MMR to player
		playerMMR = playerMMR + amountMMRChanged + streakBonus;

		//comment for testing
		if (doLongTest == 0) {

			cout << "Added: " << amountMMRChanged << " bonus " << streakBonus << " MMR\n";
		}

		winStreak++;
	}
	else {

		//if game was lost

		//End winning streak bonus
		if (winStreak != 0) {

			streakBonus = 0;
		}
		winStreak = 0; //resets win streak to 0 after loss.

		//subtract MMR to player:
		//
		//rank loss protection accounted for.

		//If player does not de-rank, MMR will be subtracted normally.
		if (rankChecker(playerMMR, MMRChangeLevel) == rankChecker(playerMMR - amountMMRChanged - streakBonus,
			MMRChangeLevel) || !rankLossPrevention) {

			playerMMR = playerMMR - amountMMRChanged - streakBonus;

			//comment for testing
			if (doLongTest == 0) {

				cout << "subtracted: " << -amountMMRChanged << " bonus " << streakBonus << " MMR\n";
			}
		}
		else {

			//If de-rank were to occur without rank loss prevention:
			//de-rank
			if (playerMMR % 100 == 0) {
				int i = 0;
				do
				{
					playerMMR--;
					i++;
				} while (playerMMR % 100 != 0 && i < amountMMRChanged + streakBonus);

				//comment for testing
				if (doLongTest == 0) {
					cout << "subtracted: " << playerMMR - tempVariable << " MMR   Rank protected!\n";

					cout << "subtracted: " << -amountMMRChanged << " bonus " << -streakBonus << " MMR\n";
				}
			}
			else { //rank loss prevented
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

// This function is for testing
int simulation(int whichTest, bool rankLossPrevention, bool streakBonusEnabled) {

	int champion = 0, diamond1 = 0, diamond2 = 0, diamond3 = 0, platinum1 = 0, platinum2 = 0, platinum3 = 0,
		gold1 = 0, gold2 = 0, gold3 = 0, silver1 = 0, silver2 = 0, silver3 = 0, silver4 = 0, silver5 = 0, bronze1 = 0,
		bronze2 = 0, bronze3 = 0, bronze4 = 0, bronze5 = 0, copper1 = 0, copper2 = 0, copper3 = 0, copper4 = 0, copper5
		= 0;

	int playerMMR, MMRChangeLevel = 0, winStreak = 0, loseStreak = 0, gamesPlayed = 0;
	int keepTestingThisPlayer = 0;
	//This is how many games are played
	int totalNumberOfGamesToPlay = 100;
	bool keepTesting = true;
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
		while (keepTestingThisPlayer < totalNumberOfGamesToPlay && keepTesting) {
			keepTestingThisPlayer++;

			//Ask the user if the game was won or loss
			if (whichTest == 0) {
				cout << "Are you winning son?" << endl;
				cin >> userInputDuringTest;
				boost::algorithm::to_lower(userInputDuringTest);
				if (userInputDuringTest == "yes" || userInputDuringTest == "win" || userInputDuringTest == "1") {
					gameWasWon = true;
				}
				else if (userInputDuringTest == "no" || userInputDuringTest == "loss" || userInputDuringTest == "0") {
					gameWasWon = false;
				}
				else {
					cout << "Ending test..." << endl;
					keepTesting = false;
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
			output = endOfMatchMMRTally(playerMMR, MMRChangeLevel, winStreak, loseStreak, gameWasWon, whichTest,
				gamesPlayed, rankLossPrevention, streakBonusEnabled);
			//output: playerMMR, MMRChangeLevel, winStreak, loseStreak, gamesPlayed

			playerMMR = output[0];
			MMRChangeLevel = output[1];
			winStreak = output[2];
			loseStreak = output[3];
			gamesPlayed = output[4];
			//output for testing


			if (whichTest == 0) {

				cout << rankChecker(playerMMR, gamesPlayed) << "\nMMR:" << playerMMR << "\nMMR change level: " <<
					MMRChangeLevel << "\nwinstreak: " << winStreak
					<< "\nLosingstreak: " << loseStreak << "\nGames Played: " << gamesPlayed << endl << endl;
			}
		}
		if (whichTest == 1) {
			cout << rankChecker(playerMMR, gamesPlayed) << " " << playerMMR << " MMR. W/L " << setprecision(2) <<
				matchesWon / matchesLost << " Matches: " << totalNumberOfGamesToPlay << endl;
		}

		//Counts ranks
		if (whichTest == 2) {

			if (rankChecker(playerMMR, MMRChangeLevel) == "Champion") { champion++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Diamond 1") { diamond1++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Diamond 2") { diamond2++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Diamond 3") { diamond3++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Platinum 1") { platinum1++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Platinum 2") { platinum2++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Platinum 3") { platinum3++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Gold 1") { gold1++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Gold 2") { gold2++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Gold 3") { gold3++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 1") { silver1++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 2") { silver2++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 3") { silver3++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 4") { silver4++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Silver 5") { silver5++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 1") { bronze1++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 2") { bronze2++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 3") { bronze3++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 4") { bronze4++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Bronze 5") { bronze5++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 1") { copper1++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 2") { copper2++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 3") { copper3++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 4") { copper4++; }
			if (rankChecker(playerMMR, MMRChangeLevel) == "Copper 5") { copper5++; }


			timerDuration++;
			if (timerDuration == 100000) {

				cout << "Champion: " << champion << "\nDiamond 1: " << diamond1 << "\nDiamond 2: " << diamond2 <<
					"\nDiamond 3: " << diamond3 << "\nPlatinum 1: " << platinum1 << "\nPlatinum 2: " << platinum2 <<
					"\nPlatinum 3: " << platinum3 << "\nGold 1: " << gold1 << "\nGold 2: " << gold2 << "\nGold 3: " <<
					gold3 << "\nSilver 1: " << silver1 << "\nSilver 2: " << silver2 << "\nSilver 3: " << silver3 <<
					"\nSilver 4: " << silver4 << "\nSilver 5: " << silver5 << "\nBronze 1: " << bronze1 <<
					"\nBronze 2: " << bronze2 << "\nBronze 3: " << bronze3 << "\nBronze 4: " << bronze4 <<
					"\nBronze 5: " << bronze5 << "\nCopper 1: " << copper1 << "\nCopper 2: " << copper2 <<
					"\nCopper 3: " << copper3 << "\nCopper 4: " << copper4 << "\nCopper 5: " << copper5 << endl << endl;
				Sleep(2000);
				timerDuration = 0;
			}
		} //End of doLongTest == 2


		MMRChangeLevel = 0, winStreak = 0, loseStreak = 0, gamesPlayed = 0, matchesWon = 0, matchesLost = 0,
			keepTestingThisPlayer = 0;

		if (whichTest == 0 && totalNumberOfGamesToPlay == 100) {
			keepTesting = false;
		}


		//
		if (GetAsyncKeyState(VK_ESCAPE)) {
			keepTesting = false;
		}
	}


	return 0;
} //End of Test

// This is a useful function that takes a string, searches for a whitespace, then separates the first word from the rest of the string.
vector<string> separateFirstWordFromString(string originalString) {

	//Usage:
	//vector<string> seperatedStrings = separateFirstWordFromString(userInput);
	//FIRSTWORD = seperatedStrings[0];
	//THERESTOFTHESTRING = seperatedStrings[1];

	string firstWord, restOfString;
	int i = 0;
	while (i < originalString.length() && originalString[i] != ' ' && originalString.length() > 0) {
		firstWord = firstWord + originalString[i];
		i++;
	}

	if (originalString.length() > i) {
		restOfString = originalString.substr(i + 1);
	}

	return { firstWord, restOfString };
}

// This is to prevent errors in case a string is supposed to contain all numbers with the only ASCII values being from the numbers 0-9
bool stringContainsAllNumbers(const string& str) {

	bool allNumbers = true;
	//check to see if input is numbers
	for (char ch : str) {

		if (ch >= 48 && ch <= 57) {

			allNumbers = true;
		}
		else {
			allNumbers = false;
			return allNumbers;
		}
	}
	return allNumbers;
}

// This tells the program what kind of setting it is going to change.  Example: It is true/false, or a int variable
char settingVariableType(const string& settingName, string settingsFileContents) {

	//Index: bool = b, int = i, double = d, char = c, string = s
	char variableType;
	int indexPosition;
	indexPosition = settingsFileContents.find(settingName);
	int cnt = settingName.length();
	variableType = settingsFileContents[indexPosition - 2];

	return variableType;
}

// This writes the new value for a variable into the settings
string changeSetting(const string& settingName, string settingsFileContents, string newValue) {
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

	if (settingVariableType(settingName, settingsFileContents) != 'b') {

		settingsFileContents.erase(indexPosition + cnt + 3 + i, std::string::npos); //This could cause a problem
	}
	return settingsFileContents;
}

// If the input is valid, it will change the variable.  This is only programmed for Boolean and Integers at the time being 3-16-24.
vector<string> validInputOrError(const string& parameter1, const string& parameter2, string settingsFileContents, string errorOccurred) {

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
			errorOccurred = "Error";
		}
	}
	else if (variableType == 'i') {
		bool allNumbers = stringContainsAllNumbers(parameter2);

		if (allNumbers) {
			//parameter2 ###
			settingsFileContents = changeSetting(parameter1, settingsFileContents, parameter2);
		}
		else {
			//parameter2 ????
			errorOccurred = "Error!";
		}
	}


	vector<string> result = { settingsFileContents, errorOccurred };

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
int pullVariableFromSettings(const string& settingName) {

	//This assumes that the setting exists:
	string settings = settingsFileContentsToString();
	string outputString;
	int indexPosition;
	indexPosition = settings.find(settingName);
	int cnt = settingName.length();
	int i = 0;

	bool keepLooping = true;
	while (settings[indexPosition + cnt + 3 + i] != '\n' && keepLooping) {

		outputString = outputString + settings[indexPosition + cnt + 3 + i];
		if (!stringContainsAllNumbers(outputString)) {

			keepLooping = false;
		}
		i++;
	}


	int intResult = stoi(outputString);
	return intResult;
}

// This is the function that is used for when settings are changed.
int setSettings(string userInput) {

	//Settings file location: C:\Users\ryant\source\repos\match simulaator\match simulaator
	string parameter1, parameter2;
	string errorOccurred = "No Error";
	//Create settings file if it does not already exist.

	//The settings file contents are copied to the string: settingsFileContents
	checkIfSettingsFileExists();


	//Get settings file contents and save it to the string settingsFileContents
	string settingsFileContents = settingsFileContentsToString();


	//Here is where you can change the settings.

	//separate userInput into parameter1 and parameter2
	vector<string> seperatedStrings = separateFirstWordFromString(std::move(userInput));
	parameter1 = seperatedStrings[0];
	parameter2 = seperatedStrings[1];


	//If the setting exists, change setting or error for invalid input
	if (boost::algorithm::contains(settingsFileContents, parameter1)) {

		vector<string> output = {};
		output = validInputOrError(parameter1, parameter2, settingsFileContents, errorOccurred);

		settingsFileContents = output[0];
		errorOccurred = output[1];
	}

	else if (parameter1 == "reset") {

		string resetConfirmation;
		cout << "Are you sure that you want to revert all settings back to defaults? y/n" << endl;
		cin >> resetConfirmation;
		boost::algorithm::to_lower(resetConfirmation);

		if (resetConfirmation == "y" || resetConfirmation == "yes") {

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

		//do nothing here////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	else {
		errorOccurred = "Error";
	}


	int indexPosition;
	indexPosition = settingsFileContents.find(parameter1);
	int cnt = parameter1.length();

	if (settingsFileContents[indexPosition + cnt + 1] != '=') {

		errorOccurred = "Error";
	}


	if (parameter1 != "reset" && errorOccurred != "Error") {

		//Save by replacing old settings file with updated settings in String.
		ofstream updateSettingsFile("settings.txt");
		updateSettingsFile << settingsFileContents;
		updateSettingsFile.close();
	}

	//If an invalid input or help is written, it will help the user
	if (errorOccurred == "Error" || parameter1 == "help") {

		if (errorOccurred == "Error") {

			cout << "Error!\n" << endl;
		}

		cout << "settings list:\n"
			<< "streakbonus <true/false>\n"
			<< "ranklossprevention <true/false>\n"
			<< "antiboosting <true/false>\n";
		errorOccurred = "No Error";
	}


	//Output settings file contents to show what the settings.txt file says
	cout << settingsFileContentsToString();


	return 0;
}

// Find squad average MMR score with anti-boosting calculation./////////////////////////////////////////////////////////////
int findAverageSquadMMR(vector<vector<int>> squad, bool removeFirstEntry) {

	if (removeFirstEntry) {

		squad.erase(squad.begin() + 0);
	}
	vector<int> squadMMR;
	squadMMR.resize(squad.size());
	for (int i = 0; i < squad.size(); i++) { squadMMR[i] = squad[i][1]; }
	bool antiBoosting = pullVariableFromSettings("antiboosting");
	int numOfPlayersInSquad = squadMMR.size();
	int averageMMR = 0;
	int adjustedAverageMMR = 0;
	int largestMMRScore = -10000000; //Just a placeholder that wont ever be met.
	int MMRLimit = pullVariableFromSettings("mmrlimit");
	

	//Get input from user of what the MMR of each player is.
	for (int i = 0; i < numOfPlayersInSquad; i++) {

		if (largestMMRScore < squadMMR[i] && antiBoosting) {

			largestMMRScore = squadMMR[i];
		}
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

	if (numOfPlayersInSquad != 0) {

		averageMMR = averageMMR / numOfPlayersInSquad;
		adjustedAverageMMR = adjustedAverageMMR / numOfPlayersInSquad;
	}
	else {

		adjustedAverageMMR = 0; //error prevention while testing
	}
	//IDK whats going on here.  Ill have to look into it
	if (removeFirstEntry || antiBoosting) {

		return adjustedAverageMMR;
	}
	return averageMMR;


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
	int numOfSquads = 5000;
	vector<vector<int>> squads;
	squads = playersTestData(numOfSquads);

	//Three dimensional array.  The first dimension is each squad queuing up.
	//The second dimension contains each player in the squad.
	//The third dimension is each player's playerID and MMR.
	vector<vector<vector<int>>> groupsInCue = {};

	//randomly put playersIDs in squads
	int i = 0;
	int j;
	int k = 0;
	int dot = 0;
	while (i < numOfSquads) {

		//This draws dots on screen to show that it is working.
		dot = dot + 1;
		if (dot % 1000 == 0) {

			cout << '.';
		}

		groupsInCue.resize(i + 1);
		j = 0;
		//randomly figure out how many people are squading up together

		int squadAverageMMR = 0;
		int numOfPlayersInSquad = trunc(rand() % 5);
		groupsInCue[i].push_back({ -1, 0 });
		while (j <= numOfPlayersInSquad) {

			//calculate the average before
			squadAverageMMR = squadAverageMMR + squads[k][1];

			groupsInCue[i].resize(j + 1);
			groupsInCue[i].push_back(squads[k]);
			j++;
			k++; // Goes  through each player
		}


		//Saves the average MMR of the squad for where the squad should be placed in matchmaking.  This can either have antiboosting on or off.
		groupsInCue[i][0][1] = findAverageSquadMMR(groupsInCue[i], true);

		i++;
	}
	//At this point, groupsInCue has a list of players cueing together for games. From here, it's a matter of matching them together


	// See what is in groupsInCue

	//for (int i = 0; i < groupsInCue.size(); i++) {
	//	cout << "Squad " << i + 1 << ":" << endl;
	//	cout << "IDs: ";
	//	for (int j = 1; j < groupsInCue[i].size(); j++) {
	//		cout << groupsInCue[i][j][0] + 1;
	//
	//		// This is for formatting:
	//		int num = groupsInCue[i][j][0];
	//		if (num == 0) {
	//			num = 1;
	//		}
	//		int spaces = 4 - trunc(log10(num));
	//
	//		for (int a = 0; a < spaces; a++) {
	//			cout << " ";
	//		}
	//	}
	//
	//	cout << "\nMMR: ";
	//	for (int j = 1; j < groupsInCue[i].size(); j++) {
	//		cout << groupsInCue[i][j][1] << " ";
	//	}
	//	if (pullVariableFromSettings("antiboosting") == 1) {
	//		cout << "Adjusted Average: " << groupsInCue[i][0][1] << " ";
	//	}
	//	else {
	//		cout << "Average: " << groupsInCue[i][0][1] << " ";
	//	}
	//	cout << endl << endl;
	//}


	cout << "\n";
	return groupsInCue;
}


//Create teams using data from createSquads()
int squadTeamSetUp() {

	// Initial setup
	vector<vector<vector<int>>> squadsInQue;

	bool continueMakingSquads = true;
	while (continueMakingSquads) {

		if (squadsInQue.empty()) {

			squadsInQue = createSquads();
		}
		cout << "squadTeamSetUp";
		// Set initial parameters
		vector<vector<int>> team1;
		vector<vector<int>> team2;

		vector<vector<int>> lobby; //For calculating average mmr of team1 + team2
		int initialRange = 25;
		int rangeOfMMR = initialRange;


		// Populate team1
		for (int i = 1; i < squadsInQue[0].size(); i++) {

			team1.push_back({ squadsInQue[0][i][0], squadsInQue[0][i][1] });
			lobby.push_back({ squadsInQue[0][i][0], squadsInQue[0][i][1] });
		}
		squadsInQue.erase(squadsInQue.begin()); // Remove first squad from queue

		//cout << "Before:  T1: " << team1.size() << " T2: " << team2.size() << endl;

		// Loop to fill both teams
		int dot = 0;
		while (team1.size() < 5 || team2.size() < 5) {

			//cout << "inside T1: " << team1.size() << " T2: " << team2.size() << endl;

			rangeOfMMR = rangeOfMMR + 25; //problem here
			dot = dot + 1;
			if (dot % 10 == 0) {

				cout << '.';
			}


			bool squadAdded = false; // Flag to track if any squad was added in this iteration

			int squadQueIndex = 0;
			while (squadQueIndex < squadsInQue.size() && !squadAdded) {

				//right here is where we make sure that the squads added are within a certain range
				// Squad that might be added has an average mmr less than the range allowed for the lobby
				if (abs(squadsInQue[squadQueIndex][0][1] - findAverageSquadMMR(lobby, false)) < rangeOfMMR) {


					// Check if the squad can fit into either team
					// If it can fit into team1
					if (team1.size() + squadsInQue[squadQueIndex].size() - 1 <= 5) {


						// Add squad to team1
						for (int i = 1; i < squadsInQue[squadQueIndex].size(); i++) {

							team1.push_back({ squadsInQue[squadQueIndex][i][0], squadsInQue[squadQueIndex][i][1] });
							lobby.push_back({ squadsInQue[squadQueIndex][i][0], squadsInQue[squadQueIndex][i][1] });
						}
						squadsInQue.erase(squadsInQue.begin() + squadQueIndex);
						squadAdded = true;
						rangeOfMMR = initialRange;
					}
					// If it can fit into team2
					else if (team2.size() + squadsInQue[squadQueIndex].size() - 1 <= 5 && !squadAdded) {

						// Add squad to team2
						for (int i = 1; i < squadsInQue[squadQueIndex].size(); i++) {

							team2.push_back({ squadsInQue[squadQueIndex][i][0], squadsInQue[squadQueIndex][i][1] });
							lobby.push_back({ squadsInQue[squadQueIndex][i][0], squadsInQue[squadQueIndex][i][1] });
						}
						squadsInQue.erase(squadsInQue.begin() + squadQueIndex);
						squadAdded = true;
						rangeOfMMR = initialRange;
					}
				}

				squadQueIndex++;
			}
		}


		int ave1 = 0;
		int ave2 = 0;
		int adjustedAve1 = 0;
		int adjustedAve2 = 0;

		cout << endl;
		for (auto& i : team1) {

			cout << "1 PlayerID: " << i[0] << "  PlayerMMR: " << i[1] << endl;
			ave1 = ave1 + i[1];
		}
		ave1 = ave1 / 5;

		cout << endl;

		for (auto& i : team2) {

			cout << "2 PlayerID: " << i[0] << "  PlayerMMR: " << i[1] << endl;
			ave2 = ave2 + i[1];
		}
		ave2 = ave2 / 5;
		cout << endl << "Actual average T1: " << ave1 << endl;
		cout << "Actual average T2: " << ave2 << endl << endl;
		cout << "Adjusted average T1: " << findAverageSquadMMR(team1, false) << endl;
		cout << "Adjusted average T2: " << findAverageSquadMMR(team2, false) << endl << endl;
		cout << "Adjusted difference: " << abs(findAverageSquadMMR(team1, false) - findAverageSquadMMR(team2, false)) << endl;
		cout << "Actual difference: " << abs(ave1 - ave2) << endl;
		cout << "range: " << rangeOfMMR << endl;

		cout << "\n\n";

		string makeANewSquad;
		cout << "Do you want to find another squad?\n";
		cin >> makeANewSquad;
		if (makeANewSquad == "1", makeANewSquad == "yes", makeANewSquad == "y") {
			continueMakingSquads = true;
		}
		else {
			return 0;
		}
	}
}


//Create teams using data from createSquads()
vector<vector<int>> modifiedSquadTeamSetUp(vector<vector<vector<int>>> squadsInQue) {

	// Set initial parameters
	vector<vector<int>> team1;
	vector<vector<int>> team2;

	vector<vector<int>> lobby; //For calculating average mmr of team1 + team2
	int initialRange = 25;
	int rangeOfMMR = initialRange;


	// Populate team1
	for (int i = 1; i < squadsInQue[0].size(); i++) {

		team1.push_back({ squadsInQue[0][i][0], squadsInQue[0][i][1] });
		lobby.push_back({ squadsInQue[0][i][0], squadsInQue[0][i][1] });
	}
	squadsInQue.erase(squadsInQue.begin()); // Remove first squad from queue

	//cout << "Before:  T1: " << team1.size() << " T2: " << team2.size() << endl;

	// Loop to fill both teams
	int dot = 0;
	while (team1.size() < 5 || team2.size() < 5) {

		//cout << "inside T1: " << team1.size() << " T2: " << team2.size() << endl;

		rangeOfMMR = rangeOfMMR + 25; //problem here
		dot = dot + 1;
		if (dot % 10 == 0) {

			cout << '.';
		}


		bool squadAdded = false; // Flag to track if any squad was added in this iteration

		int squadQueIndex = 0;
		while (squadQueIndex < squadsInQue.size() && !squadAdded) {

			//right here is where we make sure that the squads added are within a certain range
			// Squad that might be added has an average mmr less than the range allowed for the lobby
			if (abs(squadsInQue[squadQueIndex][0][1] - findAverageSquadMMR(lobby, false)) < rangeOfMMR) {


				// Check if the squad can fit into either team
				// If it can fit into team1
				if (team1.size() + squadsInQue[squadQueIndex].size() - 1 <= 5) {


					// Add squad to team1
					for (int i = 1; i < squadsInQue[squadQueIndex].size(); i++) {

						team1.push_back({ squadsInQue[squadQueIndex][i][0], squadsInQue[squadQueIndex][i][1] });
						lobby.push_back({ squadsInQue[squadQueIndex][i][0], squadsInQue[squadQueIndex][i][1] });
					}
					squadsInQue.erase(squadsInQue.begin() + squadQueIndex);
					squadAdded = true;
					rangeOfMMR = initialRange;
				}
				// If it can fit into team2
				else if (team2.size() + squadsInQue[squadQueIndex].size() - 1 <= 5 && !squadAdded) {

					// Add squad to team2
					for (int i = 1; i < squadsInQue[squadQueIndex].size(); i++) {

						team2.push_back({ squadsInQue[squadQueIndex][i][0], squadsInQue[squadQueIndex][i][1] });
						lobby.push_back({ squadsInQue[squadQueIndex][i][0], squadsInQue[squadQueIndex][i][1] });
					}
					squadsInQue.erase(squadsInQue.begin() + squadQueIndex);
					squadAdded = true;
					rangeOfMMR = initialRange;
				}
			}

			squadQueIndex++;
		}
	} 
		//take the two teams, and make 0-4 the first team and 5-9 the second team
		vector<vector<int>> outPutOfBothTeams;
		for (int i = 0; i < team1.size(); i++ ) {
			outPutOfBothTeams.push_back({ team1[i] });
		}
		for (int i = 0; i < team2.size(); i++) {
			outPutOfBothTeams.push_back({ team2[i] });
		}
		
	//Note: lobby = team1 + team2;




//print out lobby for testing purposes  //This is commented out
#pragma region
		/*
	int ave1 = 0;
	int ave2 = 0;
	int adjustedAve1 = 0;
	int adjustedAve2 = 0;

	cout << endl;
	for (auto& i : team1) {

		cout << "1 PlayerID: " << i[0] << "  PlayerMMR: " << i[1] << endl;
		ave1 = ave1 + i[1];
	}
	ave1 = ave1 / 5;

	cout << endl;

	for (auto& i : team2) {

		cout << "2 PlayerID: " << i[0] << "  PlayerMMR: " << i[1] << endl;
		ave2 = ave2 + i[1];
	}
	ave2 = ave2 / 5;
	cout << endl << "Actual average T1: " << ave1 << endl;
	cout << "Actual average T2: " << ave2 << endl << endl;
	cout << "Adjusted average T1: " << findAverageSquadMMR(team1, false) << endl;
	cout << "Adjusted average T2: " << findAverageSquadMMR(team2, false) << endl << endl;
	cout << "Adjusted difference: " << abs(findAverageSquadMMR(team1, false) - findAverageSquadMMR(team2, false)) << endl;
	cout << "Actual difference: " << abs(ave1 - ave2) << endl;
	cout << "range: " << rangeOfMMR << endl;

	cout << "\n\n";

	*/
#pragma endregion

	return outPutOfBothTeams;

}


int whoWins(vector<vector<int>> teams) {
	vector<vector<int>> team1, team2;
	int winner;
	int team1Average = 0, team2Average = 0;
	//separte team1 and team2
	for (int i = 0; i < 5; i++) {
		team1.push_back(teams[i]);
		team1Average = team1Average + teams[i][1];
	}
	for (int i = 5; i < 10; i++) {
		team2.push_back(teams[i]);
		team2Average = team2Average + teams[i][1];
	}
	team1Average = team1Average / 5;
	team2Average = team2Average / 5;
	//team1Average = findAverageSquadMMR(team1, false);
	//team2Average = findAverageSquadMMR(team2, false);

	//Generate "random" winner
	vector<int> teamThatWon;
	int test;
	if (team1Average + 700 < team2Average) {
		teamThatWon = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		cout << team1Average << " - " << team2Average << " = " << abs(team1Average - team2Average) << " Terrible 2 ";
		test = 0;
	}
	else if (team1Average + 400 < team2Average) {
		teamThatWon = { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 };
		cout << team1Average << " - " << team2Average << " = " << abs(team1Average - team2Average) << " Bad 2 ";
		test = 1;
	}
	else if (team1Average + 200 < team2Average) {
		teamThatWon = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };
		cout << team1Average << " - " << team2Average << " = " << abs(team1Average - team2Average) << " Good 2 ";
		test = 2;
	}
	else if (team1Average < team2Average) {
		teamThatWon = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 };
		cout << team1Average << " - " << team2Average << " = " << abs(team1Average - team2Average) << " Perfect 2 ";
		test = 3;
	}
	else if (team2Average + 700 < team1Average) {
		teamThatWon = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
		cout << team1Average << " - " << team2Average << " = " << abs(team1Average - team2Average) << " Terrible 1 ";
		test = 7;
	}
	else if (team2Average + 400 < team1Average) {
		teamThatWon = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 };
		cout << team1Average << " - " << team2Average << " = " << abs(team1Average - team2Average) << " Bad 1 ";
		test = 6;
	}
	else if (team2Average + 200 < team1Average) {
		teamThatWon = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 };
		cout << team1Average << " - " << team2Average << " = " << abs(team1Average - team2Average) << " Good 1 ";
		test = 5;
	}
	else if (team2Average < team1Average) {
		teamThatWon = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 };
		cout << team1Average << " - " << team2Average << " = " << abs(team1Average - team2Average) << " Perfect 1 ";
		test = 4;
	}
	else {
		teamThatWon = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 };
		cout << team1Average << " - " << team2Average << " = " << abs(team1Average - team2Average) << " EQUAL ";
		test = 8;
	}







	int randomNum = randomNumberGenerator(0, 9);

	winner = teamThatWon[randomNum];


	cout <<"Team winner: "<< winner << endl;

	return test;
}




int matchmakeAndTally() {
	int zero = 0;
	int one = 0;
	int two = 0;
	int three = 0;
	int four = 0;
	int five = 0;
	int six = 0;
	int seven = 0;
	int eight = 0;
	int nine = 0;

	//for createSquads:
	//The first dimension is each squad queuing up.
	//The second dimension contains each player in the squad.
	//The third dimension is each player's playerID and MMR.
	vector<vector<vector<int>>> squadsInQue;
	vector<vector<int>> teams, team1, team2;

	squadsInQue = createSquads();



	// This loop is for a team setup
	for (int i = 0; i < 100; i++) {

		//convert teams into team1 and team2
		teams = modifiedSquadTeamSetUp(squadsInQue);

		for (int a = 0; a < 5; a++) {
			team1.push_back(teams[a]);
		}
		for (int a = 5; a < 10; a++) {
			team2.push_back(teams[a]);
		}



		//print out the teams for testing purposes
		#pragma region
		 /*
		cout << "\nThe two teams\n";
		for (auto& i : team1) {

			cout << "#1 PlayerID: " << i[0] << "  PlayerMMR: " << i[1] << endl;

		}
		for (auto& i : team2) {

			cout << "#2 PlayerID: " << i[0] << "  PlayerMMR: " << i[1] << endl;

		}
		*/
		 
		#pragma endregion


		//remove players that were matched together from squadsInQue
		#pragma region

		//[b][j][0] seach for the playerID
		int b = 0;
		//This goes through every squad
		while (b < squadsInQue.size()) {
			int j = 1;
			bool squadIsRemoved = false;
			//This goes through every player entry
			while (j < squadsInQue[b].size() && !squadIsRemoved) {
				//This searches for any of the players on the team.   // k might not be neccesary, but I am leaving it for now due to not having time at this moment in particular.
				int k = 0;
				while (k < teams.size() && !squadIsRemoved) {
					//If one of the players in the squad is found on the team, they are dropped from the que
					if (teams[k][0] == squadsInQue[b][j][0])
					{
						squadsInQue.erase(squadsInQue.begin() + b);
						squadIsRemoved = true;
					}
					k++;
				}
				j++;
			}
			//Do not increment if the squad is removed.
			if (!squadIsRemoved) {
				b++;
			}
		}
	#pragma endregion

		//Find out which team wins
		int teamThatWins = whoWins(teams);

		teams.clear();
		team1.clear();
		team2.clear();
		//Sleep(2000);

		if (teamThatWins == 0) {
			zero++;
		}
		else if (teamThatWins == 1) {
			one++;
		}
		else if (teamThatWins == 2) {
			two++;
		}
		else if (teamThatWins == 3) {
			three++;
		}
		else if (teamThatWins == 4) {
			four++;
		}
		else if (teamThatWins == 5) {
			five++;
		}
		else if (teamThatWins == 6) {
			six++;
		}
		else if (teamThatWins == 7) {
			seven++;
		}
		else if (teamThatWins == 8) {
			three++;
			four++;
		}
		

		








	}//End of for loop that is for each team set up

	

	//Now that the players in the last team are removed from squadsInQue, we can use squadsInQue again to search for a team.

	//We need to make sure that the MMR is adjusted for the players on team1 and team2.


	//This is the homestretch! Then a database can be added!


	cout << zero << " " << one << " " << two << " " << three << " " << four << " " << five << " " << six << " " << seven << " " << eight << endl;


}


//This is set up for solo queuing right now.  //WIP
int soloQueuingTeamSetup() {

	vector<int> playersMMR = { 2000, 2100, 2200, 2300, 2400, 2600, 2700, 2800, 2900, 3000 };
	vector<int> redTeam = { 0, 0, 0, 0, 0 };
	vector<int> blueTeam = { 0, 0, 0, 0, 0 };
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
	cout << "clear - Clear the console." << endl;
	cout << "close - This will close the window and end the program." << endl;
	cout << "createteams - this will show how squads are placed together onto teams" << endl;
	cout << "help - shows the list of commands" << endl;
	cout << "settings -  This will let you see what the value of each setting holds." << endl;
	cout << "seesetting <setting name> - This allows you to see the value at a specific setting" << endl;
	cout << "set <setting parameter> <T/F or ###> - Change the settings" << endl;
	cout << "simulate <#> - this will test the matchmaking algorithm."
		<< "\n     simulate 0 - manually test wins and losses entering 1 for wins and 0 for losses."
		<< "\n     simulate 1 - Shows individual player data results from randomly generated match results. Win rates are 50%."
		<< "\n     simulate 2 - Same as test 1, but tallies final results overtime in with 10,000 players per cycle." <<
		endl;

	//cout << "test #" << endl;

	return 0;
}

//This is the main function where you can control the program and what functions are ran
int main() {

	//settings:
	checkIfSettingsFileExists();
	bool infiniteLoop = true;
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

		vector<string> seperatedStrings = separateFirstWordFromString(command);
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

			cout << "Matchmaking will place you in: " << findAverageSquadMMRDemo(antiBoostingEnabled) << " MMR." <<
				endl;
			cin.ignore();
		}
		else if (firstWordOfCommand == "set") {

			setSettings(theRestOfCommand);
		}
		else if (firstWordOfCommand == "help") {

			listOfMainCommands();
		}
		else if (firstWordOfCommand == "settings") {

			cout << settingsFileContentsToString() << endl;
		}
		else if (firstWordOfCommand == "createteams") {

			//This is for whatever I am working on at the moment.
			squadTeamSetUp();
			cin.ignore();
		}
		else if (firstWordOfCommand == "seesetting") {

			//unused rn
			cout << pullVariableFromSettings(theRestOfCommand);
		}
		else if (firstWordOfCommand == "clear") {
			system("cls");
		}
		else if (firstWordOfCommand == "close") {
			infiniteLoop = false;
		}
		else if (firstWordOfCommand == "test") {
			vector<vector<vector<int>>> squadsInQue;
			squadsInQue = createSquads();
			modifiedSquadTeamSetUp(squadsInQue);
		}
		else if (firstWordOfCommand == "test2") {

			matchmakeAndTally();
		}
		else {

			cout << "Error! invalid input!";
			listOfMainCommands();
		}
		cout << endl;
	}

	return 0;
}
