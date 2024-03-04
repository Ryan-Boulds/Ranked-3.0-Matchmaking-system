//Created by Ryan Boulds
//last edited 3-4-24
#include <iostream>
#include <fstream>  
#include <vector>
#include<windows.h>
#include <iomanip>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/count.hpp>

using namespace std;

//Making a way to use vectors ()
template <typename S>
void using_index(const vector<S>& vector,
	string sep = " ")
{
	// Iterating vector by using index
	for (int i = 0; i < vector.size(); i++) {
		// Printing the element at
		// index 'i' of vector
		cout << vector[i] << sep;
	}
	cout << endl;
}

// Find squad average MMR score with anti-boosting calculation.
int findAverageSquadMMR(bool antiBoosting) {
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

//This is a reference for what rank a player is
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

//This is the part that calculates a player's MMR score after a game
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

//This fuction is for testing
int test(int whichTest, bool rankLossPrevention, bool streakBonusEnabled) {

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

			if (whichTest == 0) {
				cout << "Are you winning son?" << endl;
				cin >> gameWasWon;
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
	}


	return 0;
}//End of Test

//subtasks for setSettings
string changeSetting(string settingName, string settingsFileContents, char trueOrFalse) {
	int indexPosition;
	indexPosition = settingsFileContents.find(settingName);
	int cnt = settingName.length();
	settingsFileContents[indexPosition + cnt + 3] = trueOrFalse;

	return settingsFileContents;
}

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

vector<string> trueFlaseOrError(string parameter1, string parameter2, string settingsFileContents, string errorOccured) {

	// True, False, or invalid
	if (parameter2 == "true" || parameter2 == "1" || parameter2 == "enable") {
		//streakbonus true
		settingsFileContents = changeSetting(parameter1, settingsFileContents, '1');
	}
	else if (parameter2 == "false" || parameter2 == "0" || parameter2 == "disable") {
		//streakbonus false
		settingsFileContents = changeSetting(parameter1, settingsFileContents, '0');
	}
	else {
		//streakbonus ????
		errorOccured = "Error";
	}

	vector<string> result = { settingsFileContents, errorOccured };

	return result;

}

//This code can be cleaned up
//This is how you change the settings
int setSettings(string userInput) {
	//Settings file location: C:\Users\ryant\source\repos\match simulaator\match simulaator
	string parameter1 = "", parameter2 = "";

	//Create settings file if it does not already exist.

	//default settings are in this region:
#pragma region
//The settings file contents are copied to the string: settingsFileContents
	ifstream settingsFileExists;
	settingsFileExists.open("settings.txt");
	if (!settingsFileExists)
	{
		//create a settings file
		ofstream createSettingsFile("settings.txt");

		//Default settings can be put here:
		createSettingsFile << "antiboosting = 1" << endl;
		createSettingsFile << "ranklossprevention = 1" << endl;
		createSettingsFile << "streakbonus = 1" << endl;

	}
	settingsFileExists.close();

	//Copy settings file to string.
	ifstream copySettingsToString;
	copySettingsToString.open("settings.txt"); //open the input file
	//Copy the contents of the settings file to a string for changing the settings.
	stringstream strStream;
	strStream << copySettingsToString.rdbuf(); //read the file
	string settingsFileContents = strStream.str(); //str holds the content of the file
	copySettingsToString.close();

#pragma endregion

	//Here is where you can change the settings.

		//This is a string instead of bool for later on in the code
	string errorOccured = "No Error";

	//seperate userInput into parameter1 and parameter2
	vector<string> seperatedStrings = seperateFirstWordFromString(userInput);
	parameter1 = seperatedStrings[0];
	parameter2 = seperatedStrings[1];


	//If the setting exists, change setting or error for invalid input
	if (boost::algorithm::contains(settingsFileContents, parameter1)) {

		vector<string> output = {};
		output = trueFlaseOrError(parameter1, parameter2, settingsFileContents, errorOccured);

		settingsFileContents = output[0];
		errorOccured = output[1];
	}
	else if(parameter1 == "help"){

	}
	else {
		errorOccured = "Error";
	}


	//If an invalid input or help is written, it will help the user
	if (errorOccured == "Error" || parameter1 == "help") {
		if (errorOccured == "Error") {
			cout << "Error!" << endl;;
		}

		cout << "settings list:\n"
			<< "streakbonus <true/false>\n"
			<< "ranklossprevention <true/false>\n"
			<< "antiboosting <true/false>\n";
		errorOccured = "No Error";
	}




	//Save by replacing old settings file with updated settings in String.
	ofstream newSettingsFile("settings.txt");
	newSettingsFile << settingsFileContents;

	cout << settingsFileContents << "\n"; //print file to console

	//End of editing settings




	ifstream settingsFile;
	settingsFile.open("settings.txt");
	//search for what characters to load into memory
	string word;
	while (settingsFile >> word) {
		if (word == "antiBoostingEnabled") {
			settingsFile.ignore(3);
			settingsFile >> word;

			// store
		}
		else if (word == "rankLossPrevention") {
			settingsFile.ignore(3);
			settingsFile >> word;
			;
		}
		else if (word == "streakBonusEnabled") {
			settingsFile.ignore(3);
			settingsFile >> word;


		}

		//remove the "2" and replace it with a 1 in the txt document
	}



	cout << endl;

	return 0;
}




//This functionn contains a list of commands to control the program
int listOfCommands() {
	cout << "\n\nList of commands:" << endl;
	cout << "averagemmr - Find the average MMR of a squad" << endl;
	cout << "help - shows the list of commands" << endl;
	cout << "set - Change the settings" << endl;
	cout << "test # - this will test the machmaking algorithm."
		<< "\n     test 0 - manually test wins and losses entering 1 for wins and 0 for losses."
		<< "\n     test 1 - Shows individual player data results from randomly generated match results. Win rates are 50%."
		<< "\n     test 2 - Same as test 1, but tallies final results overtime in with 10,000 players per cycle." << endl;
	//cout << "test #" << endl;

	return 0;
}

//This is the main function where you can control the program and what functions are ran
int main() {
	//settings: 

	bool antiBoostingEnabled = 1;
	bool rankLossPrevention = 1;
	bool streakBonusEnabled = 0;
	bool infiniteLoop = 1;
	string command;
	string temp;
	while (infiniteLoop) {

		//Get input from the user
		cout << "What command would you like to run? ";
		getline(cin, command);
		//Makes user input all lower case
		boost::algorithm::to_lower(command);

		vector<string> seperatedStrings = seperateFirstWordFromString(command);
		string firstWordOfCommand = seperatedStrings[0];
		string theRestOfCommand = seperatedStrings[1];



		//This is where functions are linked based off of where which commnand was entered by the user
		if (firstWordOfCommand == "test") {

			if (theRestOfCommand == "") {
				cout << "Which test would you like to run?\n" <<
					"0 - Individual games\n1 - Player with 50% win rate over time\n2 - Tally players with 50% win rate over time\ntest ";
				cin >> theRestOfCommand;
			}

			if (theRestOfCommand == "0") {
				test(0, rankLossPrevention, streakBonusEnabled);
			}
			else if (theRestOfCommand == "1") {
				cout << "Player with 50% win rate over time:\n";
				test(1, rankLossPrevention, streakBonusEnabled);
			}
			else if (theRestOfCommand == "2") {
				cout << "Tally players with 50% win rate over time:\n";
				test(2, rankLossPrevention, streakBonusEnabled);
			}
			else {
				cout << "Error! invalid input!";
				listOfCommands();
			}
		}
		else if (firstWordOfCommand == "averagemmr") {
			cout << "Matchmaking will place you in: " << findAverageSquadMMR(antiBoostingEnabled) << " MMR." << endl;;
			cin.ignore();
		}
		else if (firstWordOfCommand == "set") {
			setSettings(theRestOfCommand);
		}
		else if (firstWordOfCommand == "help") {
			cout << endl;
			listOfCommands();
		}
		else {
			cout << "Error! invalid input!";
			listOfCommands();
		}

		cout << endl;
	}

	return 0;
}
