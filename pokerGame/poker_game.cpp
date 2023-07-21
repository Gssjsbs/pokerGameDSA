#include <iostream>
#include <stack>
#include <algorithm>
#include <random>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

void lastPage();

struct Card {
    string suit;
    string rank;

};

struct User {
    string username;
    string password;
    double money;
};


class PokerGame {
private:
    vector<Card> deck;
    stack<Card> cardStack;
    vector<vector<Card>> players;
    vector<vector<Card>> dealer;
    vector<Card> dealerCards;
    vector<Card> playerCards;
    vector<Card> comp1Cards;
    vector<Card> comp2Cards;
    vector<Card> comp3Cards;

    int minimumBet;
    int potValue;
    int maxChips;
    int playerBet;

    int comp1Chips;
    int comp2Chips;
    int comp3Chips;
    int playerChips;
	
    bool playerActive;
    bool comp1Active;
    bool comp2Active;
    bool comp3Active;

	string playerName;
    string computer1;
    string computer2;
    string computer3;
    string playerCard1;
    string playerCard2;
    string computer1Card1;
    string computer1Card2;
    string computer2Card1;
    string computer2Card2;
    string computer3Card1;
    string computer3Card2;
    string dealerCard1;
    string dealerCard2;
    string dealerCard3;
    string dealerCard4;
    string dealerCard5;
    string gameStatus;

public:
    PokerGame() {
        minimumBet = 0;
        potValue = 0;
        maxChips = 100;
        playerChips = maxChips;
        comp1Chips = maxChips;
        comp2Chips = maxChips;
        comp3Chips = maxChips;

        playerActive = true;
        comp1Active = true;
        comp2Active = true;
        comp3Active = true;

        computer1 = "Computer 1";
        computer2 = "Computer 2";
        computer3 = "Computer 3";
    }

    void displayMenu() {
        int choice;
        system("cls");
        cout << "$$$$$$$\   $$$$$$\  $$\   $$\ $$$$$$$$\ $$$$$$$\\" << endl; 
		cout << "$$  __$$\ $$  __$$\ $$ | $$|$$ _____|$$  __$$\\" << endl;		 
		cout << "$$ |  $$|$$ /  $$|$$ |$$/ $$ |     $$ |  $$ |" << endl;
		cout << "$$$$$$$ |$$ |  $$|$$$$$/  $$$$$\    $$$$$$$  |" << endl;
		cout << "$$  ___/ $$ |  $$|$$  $$\  $$  __|  $$  __$$<" << endl; 
		cout << "$$ |     $$ |  $$|$$ |\$$\  $$ |     $$ |  $$ |" << endl;
		cout << "$$ |      $$$$$$ |$$ | \$$\ $$$$$$$$\ $$ |  $$ |" << endl;
		cout << "\__|      \______/ \__|  \__|\________|\__|  \__|" << endl;
        cout << endl;                                        

        cout << "1. Log In" << endl;
        cout << "2. Create a new account" << endl;
        cout << "3. Exit" << endl << endl;
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                createAccount();
                break;
            case 3:
                cout << endl << "Thank you for playing!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl << endl;
                displayMenu();
        }
    }

    void login() {
        string username, password;
        cout << "\nLog In" << endl << endl;
        cin.ignore();
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        ifstream userFile("users.csv");
        bool found = false;
        string line;
        while (getline(userFile, line)) {
            size_t pos = line.find(',');
            string existingUsername = line.substr(0, pos);

            size_t pos2 = line.find(',', pos + 1);
            string existingPassword = line.substr(pos + 1, pos2 - pos - 1);

            if (existingUsername == username && existingPassword == encryptPassword(password)) {
                found = true;
                cout << "Login successful!" << endl << endl;
                playerName = username;
                break;
            }
        }

        if (!found) {
            cout << "Invalid username or password!" << endl;
            sleep(2);
            displayMenu();
        }

        userFile.close();
    }

    void createAccount() {
        string username, password, verifyPass;
        cout << "Create an account" << endl << endl;
        cout << "Enter your username: ";
        cin.ignore();
        getline(cin, username);

        ifstream userFile("users.csv");
        string line;
        while (getline(userFile, line)) {
            size_t pos = line.find(',');
            string existingUsername = line.substr(0, pos);

            if (existingUsername == username) {
                cout << "User already exists." << endl << endl;
                userFile.close();
                sleep(2);
                displayMenu();
                return;
            }
        }

        userFile.close();

        cout << "Enter your password: ";
        getline(cin, password);
        cout << "Verify your password: ";
        getline(cin, verifyPass);

        if (password == verifyPass) {
            password = encryptPassword(password);
            ofstream file("users.csv", ios::app);
            if (file.is_open()) {
                file << username << "," << password << "," << "100"
                     << endl;
                file.close();
                cout << "Registered successfully!" << endl;
            } else {
                cout << "Unable to open the file." << endl;
            }
        } else {
            cout << "Passwords do not match." << endl;
        }
    }

    string transformPassword(const string &password, int shift) {
        string transformedPassword = password;

        for (char &ch: transformedPassword) {
            if (isalpha(ch)) {
                char base = isupper(ch) ? 'A' : 'a';
                ch = static_cast<char>((ch - base + shift) % 26 + base);
            }
        }

        return transformedPassword;
    }

    string encryptPassword(const string &password) {
        int shift = password.length();
        return transformPassword(password, shift);
    }

    vector<User> readCSV(const string& filename) {
        vector<User> users;
        ifstream file(filename);

        if (!file) {
            cerr << "Failed to open the file: " << filename << endl;
            return users;
        }

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string username, password, moneyStr;
            if (getline(iss, username, ',') &&
                getline(iss, password, ',') &&
                getline(iss, moneyStr, ',')) {
                try {
                    double money = stod(moneyStr);
                    users.push_back({username, password, money});
                } catch (const exception& e) {
                    cerr << "Error parsing money value: " << moneyStr << endl;
                }
            } else {
                cerr << "Invalid line in the file: " << line << endl;
            }
        }

        file.close();
        return users;
    }

    void countChips() {
        vector<User> users = readCSV("users.csv");

        // Access and use the extracted data
        for (const auto &user: users) {
            playerChips = user.money;
            playerName = user.username;
        }
    }
	
	void updateChips() {
        ifstream file("users.csv"); // Open the CSV file

        vector<string> lines; // Store the lines of the file

        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }

        file.close(); // Close the file

        const char delimiter = ',';

        // Iterate over the lines and update the "money" field
        for (string& line : lines) {
            istringstream iss(line);
            string field;
            vector<string> fields;

            while (getline(iss, field, delimiter)) {
                fields.push_back(field);
            }

            // Identify the column index of "money" (assuming it's the third column)
            const int moneyColumnIndex = 2;

            // Replace the "money" field with the desired value
            fields[moneyColumnIndex] = to_string(playerChips);

            // Reconstruct the modified line
            line.clear();
            for (const string& field : fields) {
                line += field + delimiter;
            }

            line.pop_back(); // Remove the trailing delimiter
        }

        // Open the file in write mode to overwrite the contents
        ofstream outputFile("users.csv");

        // Write the modified lines back to the file
        for (const string& line : lines) {
            outputFile << line << endl;
        }

        outputFile.close(); // Close the file
    }
    
    void run() {
        cardDistribution(1);
        pokerTable();
        int randomPlayer = firstTurn();
        int firstTurn = randomPlayer;
        int secondTurn = randomPlayer;
        int thirdTurn = randomPlayer;
        firstRound(firstTurn);
        cardDistribution(2);
        secondRound(secondTurn);
        cardDistribution(3);
        thirdRound(thirdTurn);
        cardDistribution(4);
        pokerTable();
        updateChips();
    }

    unordered_map<string, int> cardValues = {
            {"Ace",   14},
            {"King",  13},
            {"Queen", 12},
            {"Jack",  11}
    };

    bool isFlush(const vector<string> &dealerSuits, int dealerSize, const vector<string> &playerSuits, int playerSize) {
        unordered_map<string, int> suitCounts;

        // Count suits in dealerSuits array
        for (int i = 0; i < dealerSize; i++) {
            suitCounts[dealerSuits[i]]++;
        }

        // Count suits in playerSuits array
        for (int i = 0; i < playerSize; i++) {
            suitCounts[playerSuits[i]]++;
        }

        int maxOccurrence = 0;
        for (const auto &pair: suitCounts) {
            if (pair.second > maxOccurrence) {
                maxOccurrence = pair.second;
            }
        }

        if (maxOccurrence >= 5) {
            return true;
        } else {
            return false;
        }
    }

    bool isStraight(const vector<string> &dealerRanks, const vector<string> &playerRanks) {
    // Define the valid ranks for a standard 52-card deck.
    const unordered_set<string> validRanks = {
        "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"
    };

    // Combine the ranks of the dealer and the player.
    vector<string> allRanks = dealerRanks;
    allRanks.insert(allRanks.end(), playerRanks.begin(), playerRanks.end());

    // Convert the card ranks to numerical values.
    vector<int> numericalRanks;
    for (const auto &rank : allRanks) {
        if (validRanks.find(rank) != validRanks.end()) {
            if (rank == "Ace") {
                numericalRanks.push_back(14); // Ace can be high or low, assign it the highest value here.
            } else if (rank == "King") {
                numericalRanks.push_back(13);
            } else if (rank == "Queen") {
                numericalRanks.push_back(12);
            } else if (rank == "Jack") {
                numericalRanks.push_back(11);
            } else {
                numericalRanks.push_back(stoi(rank));
            }
        } else {
            return false; // Invalid rank, cannot form a straight
        }
    }

    // Sort the ranks in ascending order.
    sort(numericalRanks.begin(), numericalRanks.end());

    // Check if there is a consecutive straight of 5 cards.
    int missingCards = 0;
    for (int i = 0; i < numericalRanks.size() - 1; i++) {
        if (numericalRanks[i] + 1 != numericalRanks[i + 1]) {
            missingCards++;
            if (missingCards > 1) {
                return false; // Cannot form a straight
            }
        }
    }

    return true;
}



    string countPairs(const vector<string> &dealerRanks, int dealerSize, const vector<string> &playerRanks, int playerSize) {
        unordered_map<string, int> countMap;

        // Count strings in dealerhand
        for (int i = 0; i < dealerSize; i++) {
            countMap[dealerRanks[i]]++;
        }

        // Count strings in playerhand
        for (int i = 0; i < playerSize; i++) {
            countMap[playerRanks[i]]++;
        }

        int fourOfAKind = 0;
        int threeOfAKind = 0;
        int twoOfAKind = 0;

        // Count occurrences
        for (const auto &pair: countMap) {
            int count = pair.second;
            if (count == 4) {
                fourOfAKind++;
            } else if (count == 3) {
                threeOfAKind++;
            } else if (count == 2) {
                twoOfAKind++;
            }
        }

        if (fourOfAKind >= 1) {
            return "Four of a Kind";
        } else if (threeOfAKind >= 1 && twoOfAKind >= 1) {
            return "Full House";
        } else if (threeOfAKind >= 1) {
            return "Three of a Kind";
        } else if (twoOfAKind >= 2) {
            return "Two Pair";
        } else if (twoOfAKind == 1) {
            return "Pair";
        } else {
            return "No valid hand";
        }
    }

    int highCard(const vector<string> &ranks) {
        unordered_map<string, int> cardValues;

        int highestValue = 0;
        for (const string &rank: ranks) {
            int value = 0;
            auto it = cardValues.find(rank);
            if (it != cardValues.end()) {
                value = it->second; // Use the numerical value from cardValues map for face cards
            } else {
                try {
                    value = stoi(rank); // Convert numerical rank strings to integers
                } catch (const invalid_argument &e) {
                    value = 0;
                }
            }
            highestValue = max(highestValue, value);
        }
        return highestValue;
    }
    	    
    int lastRoundChecker(const vector<string> &playerRanks, const vector<string> &playerSuits,
                         const vector<string> &comp1Ranks, const vector<string> &comp1Suits,
                         const vector<string> &comp2Ranks, const vector<string> &comp2Suits,
                         const vector<string> &comp3Ranks, const vector<string> &comp3Suits,
                         const vector<string> &dealerRanks, const vector<string> &dealerSuits) {
        int playerScore = 50;
        int comp1Score = 50;
        int comp2Score = 50;
        int comp3Score = 50;
        string playerHand;
        string comp1Hand;
        string comp2Hand;
        string comp3Hand;
        vector<string> playerRanksCopy = playerRanks;
        vector<string> playerSuitsCopy = playerSuits;
        int playerSize = playerSuits.size();
        vector<string> comp1RanksCopy = comp1Ranks;
        vector<string> comp1SuitsCopy = comp1Suits;
        int comp1Size = comp1Suits.size();
        vector<string> comp2RanksCopy = comp2Ranks;
        vector<string> comp2SuitsCopy = comp2Suits;
        int comp2Size = comp2Suits.size();
        vector<string> comp3RanksCopy = comp3Ranks;
        vector<string> comp3SuitsCopy = comp3Suits;
        int comp3Size = comp3Suits.size();
        vector<string> dealerRanksCopy = dealerRanks;
        vector<string> dealerSuitsCopy = dealerSuits;
        int dealerSize = dealerSuits.size();


        vector<string> dealerRanksVec = dealerRanksCopy;
        vector<string> playerRanksVec = playerRanksCopy;
        vector<string> comp1RanksVec = comp1RanksCopy;
        vector<string> comp2RanksVec = comp2RanksCopy;
        vector<string> comp3RanksVec = comp3RanksCopy;

        if (playerActive) {
            if (isStraight(dealerRanksVec, playerRanksVec) &&
                isFlush(dealerSuits, dealerSize, playerRanks, playerSize)) {
                playerScore = 2;
                playerHand = "Straight Flush";
            } else if (countPairs(dealerRanks, dealerSize, playerRanks, playerSize) == "Four of a Kind") {
                playerScore = 3;
                playerHand = "Four of a Kind";
            } else if (countPairs(dealerRanks, dealerSize, playerRanks, playerSize) == "Full House") {
                playerScore = 4;
                playerHand = "Full House";
            } else if (isFlush(dealerSuits, dealerSize, playerSuits, playerSize)) {
                playerScore = 5;
                playerHand = "Flush";
            } else if (isStraight(dealerRanksVec, playerRanksVec)) {
                playerScore = 6;
                playerHand = "Straight";
            } else if (countPairs(dealerRanks, dealerSize, playerRanks, playerSize) == "Three of a Kind") {
                playerScore = 7;
                playerHand = "Three of a Kind";
            } else if (countPairs(dealerRanks, dealerSize, playerRanks, playerSize) == "Two Pair") {
                playerScore = 8;
                playerHand = "Two Pair";
            } else if (countPairs(dealerRanks, dealerSize, playerRanks, playerSize) == "Pair") {
                playerScore = 9;
                playerHand = "Pair";
            } else {
                playerScore = highCard(playerRanks) + 10;
                playerHand = "High Card";
            }
            cout << playerScore;
        }
        if (comp1Active) {
            if (isStraight(dealerRanksVec, comp1RanksVec) && isFlush(dealerSuits, dealerSize, comp1Ranks, comp1Size)) {
                comp1Score = 2;
                comp1Hand = "Straight Flush";
            } else if (countPairs(dealerRanks, dealerSize, comp1Ranks, comp1Size) == "Four of a Kind") {
                comp1Score = 3;
                comp1Hand = "Four of a Kind";
            } else if (countPairs(dealerRanks, dealerSize, comp1Ranks, comp1Size) == "Full House") {
                comp1Score = 4;
                comp1Hand = "Full House";
            } else if (isFlush(dealerSuits, dealerSize, comp1Suits, comp1Size)) {
                comp1Score = 5;
                comp1Hand = "Flush";
            } else if (isStraight(dealerRanksVec, comp1RanksVec)) {
                comp1Score = 6;
                comp1Hand = "Straight";
            } else if (countPairs(dealerRanks, dealerSize, comp1Ranks, comp1Size) == "Three of a Kind") {
                comp1Score = 7;
                comp1Hand = "Three of a Kind";
            } else if (countPairs(dealerRanks, dealerSize, comp1Ranks, comp1Size) == "Two Pair") {
                comp1Score = 8;
                comp1Hand = "Two Pair";
            } else if (countPairs(dealerRanks, dealerSize, comp1Ranks, comp1Size) == "Pair") {
                comp1Score = 9;
                comp1Hand = "Pair";
            } else {
                comp1Score = highCard(comp1Ranks)+10;
                comp1Hand = "High Card";
            }
            cout << comp1Score;
        }
        if (comp2Active) {
            if (isStraight(dealerRanksVec, comp2RanksVec) && isFlush(dealerSuits, dealerSize, comp2Ranks, comp2Size)) {
                comp2Score = 2;
                comp2Hand = "Straight Flush";
            } else if (countPairs(dealerRanks, dealerSize, comp2Ranks, comp2Size) == "Four of a Kind") {
                comp2Score = 3;
                comp2Hand = "Four of a Kind";
            } else if (countPairs(dealerRanks, dealerSize, comp2Ranks, comp2Size) == "Full House") {
                comp2Score = 4;
                comp2Hand = "Full House";
            } else if (isFlush(dealerSuits, dealerSize, comp2Suits, comp2Size)) {
                comp2Score = 5;
                comp2Hand = "Flush";
            } else if (isStraight(dealerRanksVec, comp2RanksVec)) {
                comp2Score = 6;
                comp2Hand = "Straight";
            } else if (countPairs(dealerRanks, dealerSize, comp2Ranks, comp2Size) == "Three of a Kind") {
                comp2Score = 7;
                comp2Hand = "Three of a Kind";
            } else if (countPairs(dealerRanks, dealerSize, comp2Ranks, comp2Size) == "Two Pair") {
                comp2Score = 8;
                comp2Hand = "Two Pair";
            } else if (countPairs(dealerRanks, dealerSize, comp2Ranks, comp2Size) == "Pair") {
                comp2Score = 9;
                comp2Hand = "Pair";
            } else {
                comp2Score = highCard(comp2Ranks) + 10;
                comp2Hand = "High Card";
            }
            cout << comp2Score;
        }
        if (comp3Active) {
            if (isStraight(dealerRanksVec, comp3RanksVec) && isFlush(dealerSuits, dealerSize, comp3Ranks, comp3Size)) {
                comp3Score = 2;
                comp3Hand = "Straight Flush";
            } else if (countPairs(dealerRanks, dealerSize, comp3Ranks, comp3Size) == "Four of a Kind") {
                comp3Score = 3;
                comp3Hand = "Four of a Kind";
            } else if (countPairs(dealerRanks, dealerSize, comp3Ranks, comp3Size) == "Full House") {
                comp3Score = 4;
                comp3Hand = "Full House";
            } else if (isFlush(dealerSuits, dealerSize, comp3Suits, comp3Size)) {
                comp3Score = 5;
                comp3Hand = "Flush";
            } else if (isStraight(dealerRanksVec, comp3RanksVec)) {
                comp3Score = 6;
                comp3Hand = "Straight";
            } else if (countPairs(dealerRanks, dealerSize, comp3Ranks, comp3Size) == "Three of a Kind") {
                comp3Score = 7;
                comp3Hand = "Three of a Kind";
            } else if (countPairs(dealerRanks, dealerSize, comp3Ranks, comp3Size) == "Two Pair") {
                comp3Score = 8;
                comp3Hand = "Two Pair";
            } else if (countPairs(dealerRanks, dealerSize, comp3Ranks, comp3Size) == "Pair") {
                comp3Score = 9;
                comp3Hand = "Pair";
            } else {
                comp3Score = highCard(comp3Ranks) + 10;
                comp3Hand = "High Card";
            }
            cout << comp3Score;
        }
        
		if (playerScore == comp1Score && playerScore == comp2Score && playerScore == comp3Score && playerScore != 50){
			gameStatus = "All participant wins by " + playerHand;
			playerChips += potValue /4;
			comp1Chips += potValue /4;
			comp2Chips += potValue /4;
			comp3Chips += potValue /4;
		}else if (playerScore == comp1Score && playerScore == comp2Score && playerScore != 50){
			if (playerScore < comp3Score){
				gameStatus = "Player, Comp1, and Comp2 wins by " + playerHand;
				playerChips += potValue /3;
				comp1Chips += potValue / 3;
				comp2Chips += potValue /3;
			}else{
				gameStatus = "Comp3 wins by " + comp3Hand;
				comp3Chips += potValue;
			}
		}else if (playerScore == comp2Score && playerScore == comp3Score && playerScore != 50){
			if (playerScore < comp1Score){
				gameStatus = "Player, Comp2, and Comp3 wins by " + playerHand;
				playerChips += potValue /3;
				comp2Chips += potValue / 3;
				comp3Chips += potValue /3;
			}else{
				gameStatus = "Comp1 wins by " + comp3Hand;
				comp1Chips += potValue;
			}
		}else if (comp1Score == comp2Score && comp1Score == comp3Score && comp1Score != 50){
			if (comp1Score < playerScore){
				gameStatus = "Computers wins by " + comp1Hand;
				comp1Chips += potValue /3;
				comp2Chips += potValue / 3;
				comp3Chips += potValue /3;
			}else{
				gameStatus = "Player wins by " + playerHand;
				playerChips += potValue;
			}
		}else if (playerScore == comp1Score && playerScore != 50) {
            if (playerScore < comp2Score && playerScore < comp3Score) {
                gameStatus = "Player and Comp1 wins by " + playerHand;
                playerChips += potValue / 2;
                comp1Chips += potValue / 2;
            } else {
                if (comp2Score < comp3Score) {
                    gameStatus = "Comp2 wins by " + comp2Hand;
                    comp2Chips += potValue;
                } else {
                    gameStatus = "Comp3 wins by " + comp3Hand;
                    comp3Chips += potValue;
                }
            }
        } else if (playerScore == comp2Score && playerScore != 50) {
            if (playerScore < comp1Score && playerScore < comp3Score) {
                gameStatus = "Player and Comp2 wins by " + playerHand;
                playerChips += potValue / 2;
                comp2Chips += potValue / 2;
            } else {
                if (comp1Score < comp3Score) {
                    gameStatus = "Comp1 wins by " + comp1Hand;
                    comp1Chips += potValue;
                } else {
                    gameStatus = "Comp3 wins by " + comp3Hand;
                    comp3Chips += potValue;
                }
            }
        } else if (playerScore == comp3Score && playerScore != 50) {
            if (playerScore < comp1Score && playerScore < comp2Score) {
                gameStatus = "Player and Comp3 wins by " + playerHand;
                playerChips += potValue / 2;
                comp3Chips += potValue / 2;
            } else {
                if (comp1Score < comp2Score) {
                    gameStatus = "Comp1 wins by " + comp1Hand;
                    comp1Chips += potValue;
                } else {
                    gameStatus = "Comp2 wins by " + comp2Hand;
                    comp2Chips += potValue;
                }
            }
        } else if (comp1Score == comp2Score && comp1Score != 50) {
            if (comp1Score < playerScore && comp1Score < comp3Score) {
                gameStatus = "Comp1 and Comp2 wins by " + comp1Hand;
                comp2Chips += potValue / 2;
                comp1Chips += potValue / 2;
            } else {
                if (playerScore < comp3Score) {
                    gameStatus = "Player wins by " + playerHand;
                    playerChips += potValue;
                } else {
                    gameStatus = "comp3 wins by " + comp3Hand;
                    comp3Chips += potValue;
                }
            }
        } else if (comp1Score == comp3Score && comp1Score != 50) {
            if (comp1Score < playerScore && comp1Score < comp2Score) {
                gameStatus = "Comp1 and Comp3 wins by " + comp1Hand;
                comp3Chips += potValue / 2;
                comp1Chips += potValue / 2;
            } else {
                if (playerScore < comp2Score) {
                    gameStatus = "Player wins by " + playerHand;
                    playerChips += potValue;
                } else {
                    gameStatus = "Comp2 wins by " + comp2Hand;
                    comp2Chips += potValue;
                }
            }
        } else if (comp2Score == comp3Score && comp2Score != 50) {
            if (comp2Score < playerScore && comp2Score < comp1Score) {
                gameStatus = "Comp2 and Comp3 wins by " + comp2Hand;
                comp2Chips += potValue / 2;
                comp3Chips += potValue / 2;
            } else {
                if (playerScore < comp1Score) {
                    gameStatus = "Player wins by " + playerHand;
                    playerChips += potValue;
                } else {
                    gameStatus = "Comp1 wins by " + comp1Hand;
                    comp1Chips += potValue;
                }
            }
        } else {
            if (playerScore < comp1Score && playerScore < comp2Score && playerScore < comp3Score) {
                gameStatus = "Player wins by " + playerHand;
                playerChips += potValue;
            } else if (comp1Score < playerScore && comp1Score < comp2Score && comp1Score < comp3Score) {
                gameStatus = "Comp1 wins by " + comp1Hand;
                comp1Chips += potValue;
            } else if (comp2Score < playerScore && comp2Score < comp1Score && comp2Score < comp3Score) {
                gameStatus = "Comp2 wins by " + comp2Hand;
                comp2Chips += potValue;
            } else {
                gameStatus = "Comp3 wins by " + comp3Hand;
                comp3Chips += potValue;
            }
        }
        potValue = 0;
    }


    void firstRound(int firstPlayer) {
        if (firstPlayer != 0) {//means computer napili
            computerBetting(firstPlayer);//magbebet si first computer
            pokerTable(); //update poker table

            for (int i = 0; i < 3; i++) {//para ma-ask yung natitirang dalwang player
                if (firstPlayer == 1) {//mean si computer 1 ang unang napili
                    //move ni computer 2
                    if (comp2Active) {
//						computerBetting(2);
                        computerMove(2);
                        firstPlayer = 2;//next turn
                    } else {
                        firstPlayer = 2;//next turn
                    }
                } else if (firstPlayer == 2) {//means nakaira na si computer 2
                    //move ni computer 3
                    if (comp3Active) {
//						computerBetting(3);
                        computerMove(3);
                        firstPlayer = 3;//next turn
                    } else {
                        firstPlayer = 3;//next turn
                    }
                } else if (firstPlayer == 3) {//means nakatira na si computer 3
                    //move ni player
                    if (playerActive) {
                        playerMove(1);
                        firstPlayer = 0; //next turn
                    } else {
                        firstPlayer = 0; //next turn
                    }
                } else if (firstPlayer == 0) {//means nakatira na si player
                    //move ni computer 1
                    if (comp1Active) {
//						computerBetting(1);
                        computerMove(1);
                        firstPlayer = 1;
                    } else {
                        firstPlayer = 1;
                    }
                }
                pokerTable();//update table per loop
            }
        } else {
            playerBetting();//magbebet si player
            pokerTable(); //update poker table

            for (int i = 0; i < 3; i++) {//para ma-ask yung natitirang dalwang player
                if (firstPlayer == 0) {//mean si player ang unang napili
                    //move ni computer 1
                    if (comp1Active) {
//						computerBetting(1);
                        computerMove(1);
                    }
                    firstPlayer = 1;//next turn
                } else if (firstPlayer == 1) {//means nakaira na si computer 1
                    //move ni computer 2
                    if (comp2Active) {
//						computerBetting(2);
                        computerMove(2);
                    }
                    firstPlayer = 2;//next turn
                } else if (firstPlayer == 2) {//means nakatira na si computer 2
                    //move ni computer 3
                    if (comp3Active) {
//						computerBetting(3);
                        computerMove(3);
                    }
                    firstPlayer = 3; //next turn
                } else if (firstPlayer == 3) {//means nakatira na si computer 3
                    //move ni player
                    if (playerActive) {
                        playerMove(1);
                    }
                    firstPlayer = 0;
                }
                winnerChecker();
                pokerTable();//update table
            }
        }
    }

    void secondRound(int secondPlayer) {
        //3 cards distribution for dealer
        for (int i = 0; i < 4; i++) {
            if (secondPlayer == 0) {//meeans si player yung nauna kanina
                //move ni player
                if (playerActive) {
                    playerMove(2);
                    secondPlayer = 1;//next turn
                } else {
                    secondPlayer = 1;//next turn
                }
            } else if (secondPlayer == 1) {//turn ni computer 1
                //move  ni computer 1
                if (comp1Active) {
                    computerMove(1);
                    secondPlayer = 2; //next turn
                } else {
                    secondPlayer = 2; //next turn
                }
            } else if (secondPlayer == 2) {//turn ni computer 2
                //move ni computer 2
                if (comp2Active) {
                    computerMove(2);
                    secondPlayer = 3;//next turn
                } else {
                    secondPlayer = 3;//next turn
                }
            } else if (secondPlayer == 3) {//turn ni computer 3
                //move ni computer 3
                if (comp3Active) {
                    computerMove(3);
                    secondPlayer = 0;//next turn
                } else {
                    secondPlayer = 0;//next turn
                }
            }
            pokerTable();//update table
            winnerChecker();
        }
    }

    void thirdRound(int thirdPlayer) {
        //2 cards distribution for dealer
        for (int i = 0; i < 4; i++) {
            if (thirdPlayer == 0) {//meeans si player yung nauna kanina
                //move ni player
                if (playerActive) {
                    playerMove(3);
                    thirdPlayer = 1;//next turn
                } else {
                    thirdPlayer = 1;//next turn
                }
            } else if (thirdPlayer == 1) {//turn ni computer 1
                //move  ni computer 1
                if (comp1Active) {
                    computerMove(1);
                    thirdPlayer = 2; //next turn
                } else {
                    thirdPlayer = 2; //next turn
                }
            } else if (thirdPlayer == 2) {//turn ni computer 2
                //move ni computer 2
                if (comp2Active) {
                    computerMove(2);
                    thirdPlayer = 3;//next turn
                } else {
                    thirdPlayer = 3;//next turn
                }
            } else if (thirdPlayer == 3) {//turn ni computer 3
                //move ni computer 3
                if (comp3Active) {
                    computerMove(3);
                    thirdPlayer = 0;//next turn
                } else {
                    thirdPlayer = 0;//next turn
                }
            }
            pokerTable();//update table
            winnerChecker();
        }
    }

    void winnerChecker() {
        if (!comp1Active && !comp2Active && !comp3Active) {
            gameStatus = "Player won! By being the only player remaining";
            playerChips += potValue;
            pokerTable();
            exit(0);
        } else if (!playerActive && !comp2Active && !comp3Active) {
            gameStatus = "Computer 1 won! By being the only player remaining";
            comp1Chips += potValue;
            pokerTable();
            exit(0);
        } else if (!comp1Active && !playerActive && !comp3Active) {
            gameStatus = "Computer 2 won! By being the only player remaining";
            comp2Chips += potValue;
            pokerTable();
            exit(0);
        } else if (!comp1Active && !comp2Active && !playerActive) {
            gameStatus = "Computer 3 won! By being the only player remaining";
            comp3Chips += potValue;
            pokerTable();
            exit(0);
        }
    }

    void playerMove(int currentRound) {
        int choice;
        if (currentRound == 1) {
            cout << "									MOVES:" << endl;
            cout << "							[1]Check	[2]Call		[3]Fold" << endl;
        } else {
            cout << "									MOVES:" << endl;
            cout << "						[1]Check	[2]Call		[3]Fold		[4]Raise" << endl;
        }
        while (true) {
            cout << "What is your move: ";
            cin >> choice;

            if (choice == 1) {
                checkMove(0);
                break;
            } else if (choice == 2) {
                if (playerChips < minimumBet) {
                    gameStatus = "Insufficient Chips. Suggest to Check or Fold";
                    pokerTable();
                    playerMove(currentRound);
                    break;
                } else {
                    callMove(0);
                    break;
                }
            } else if (choice == 3) {
                foldMove(0);
                break;
            } else if (choice == 4) {
                if (playerChips < minimumBet) {
                    gameStatus = "Insufficient Chips. Suggest to Check or Fold";
                    pokerTable();
                    playerMove(currentRound);
                    break;
                } else {
                    raiseMove(0);
                    break;
                }
            }
        }
    }

    void computerMove(int computerNumber) {
        while (true) {

            random_device rd;
            mt19937 rng(rd());
            rng.seed(time(nullptr));
            uniform_int_distribution<int> uni(1, 3);
            int compMoves = uni(rng);

            if (computerNumber == 1) {//means si computer 1 ang momove
                if (compMoves == 1) {//check
                    checkMove(1);
                } else if (compMoves == 2 && comp1Chips >= minimumBet) {//call
                    callMove(1);
                } else if (compMoves == 3 && comp1Chips > minimumBet) {//raise
                    raiseMove(1);
                } else {//fold
                    checkMove(1);
                }
                break;
            } else if (computerNumber == 2) {
                if (compMoves == 1) {//check
                    checkMove(2);
                } else if (compMoves == 2 && comp2Chips >= minimumBet) {//call
                    callMove(2);
                } else if (compMoves == 3 && comp2Chips > minimumBet) {//raise
                    raiseMove(2);
                } else {//fold
                    checkMove(2);
                }
                break;
            } else if (computerNumber == 3) {
                if (compMoves == 1) {//check
                    checkMove(3);
                } else if (compMoves == 2 && comp3Chips >= minimumBet) {//call
                    callMove(3);
                } else if (compMoves == 3 && comp3Chips > minimumBet) {//raise
                    raiseMove(3);
                } else {//fold
                    checkMove(3);
                }
                break;
            }
        }
        sleep(2);
    }

    void foldMove(int foldUser) {
        if (foldUser == 0) {
            playerActive = false;
            gameStatus = "Player used Fold!";
        } else if (foldUser == 1) {
            comp1Active = false;
            gameStatus = "Computer 1 used Fold!";
        } else if (foldUser == 2) {
            comp2Active = false;
            gameStatus = "Computer 2 used Fold!";
        } else if (foldUser == 3) {
            comp3Active = false;
            gameStatus = "Computer 3 used Fold!";
        }
        sleep(2);
    }

    void checkMove(int checkUser) {
        if (checkUser == 0) {
            gameStatus = "Player used Check!";
        } else if (checkUser == 1) {
            gameStatus = "Computer 1 used Check!";
        } else if (checkUser == 2) {
            gameStatus = "Computer 2 used Check!";
        } else if (checkUser == 3) {
            gameStatus = "Computer 3 used Check!";
        }
        sleep(2);
    }

    void callMove(int callUser) {
        if (callUser == 0) {
            playerChips -= minimumBet;
            potValue += minimumBet;
            gameStatus = "Player used Call!";
        } else if (callUser == 1) {
            comp1Chips -= minimumBet;
            potValue += minimumBet;
            gameStatus = "Computer 1 used Call!";
        } else if (callUser == 2) {
            comp2Chips -= minimumBet;
            potValue += minimumBet;
            gameStatus = "Computer 2 used Call!";
        } else if (callUser == 3) {
            comp3Chips -= minimumBet;
            potValue += minimumBet;
            gameStatus = "Computer 3 used Call!";
        }
        sleep(2);
    }

    void raiseMove(int raiseUser) {//betting for raise move pls fix
        if (raiseUser == 0) {
            if (playerChips > minimumBet) {
                bettingForRaise(0);
            } else {
                gameStatus = "Player does not have enough chips!";
            }
        } else if (raiseUser == 1) {
            if (comp1Chips > minimumBet) {
                bettingForRaise(1);
            } else {
                gameStatus = "Computer 1 does not have enough chips!";
            }
        } else if (raiseUser == 2) {
            if (comp2Chips > minimumBet) {
                bettingForRaise(2);
            } else {
                gameStatus = "Computer 2 does not have enough chips!";
            }
        } else if (raiseUser == 3) {
            if (comp3Chips > minimumBet) {
                bettingForRaise(3);
            } else {
                gameStatus = "Computer 3 does not have enough chips!";
            }
        }
        sleep(2);
    }

    void bettingForRaise(int raiseBetter) {
        int raiseAmount;
        random_device rd;
        mt19937 rng(rd());
        rng.seed(time(nullptr));

        while (true) {
            if (raiseBetter == 0) {//player raise
                cout << "How much would you want to raise? ";
                cin >> raiseAmount;
                if (raiseAmount > minimumBet) {
                    playerChips -= raiseAmount;
                    minimumBet = raiseAmount;
                    potValue += raiseAmount;
                    gameStatus = "Player raised the minimum bet to $" + to_string(raiseAmount);
                    break;
                } else if (raiseAmount < minimumBet) {
                    gameStatus = "Bet higher amount!";
                    pokerTable();
                } else if (raiseAmount > playerChips) {
                    gameStatus = "Insufficient Chips!";
                    pokerTable();
                }
            } else if (raiseBetter == 1) {
                uniform_int_distribution<int> uni(minimumBet, comp1Chips);
                int compRaiseBet = uni(rng);
                comp1Chips -= compRaiseBet;
                minimumBet = compRaiseBet;
                potValue += compRaiseBet;
                gameStatus = "Computer 1 raised the minimum bet to $" + to_string(compRaiseBet);
                break;
            } else if (raiseBetter == 2) {
                uniform_int_distribution<int> uni(minimumBet, comp2Chips);
                int compRaiseBet = uni(rng);
                comp2Chips -= compRaiseBet;
                minimumBet = compRaiseBet;
                potValue += compRaiseBet;
                gameStatus = "Computer 2 raised the minimum bet to $" + to_string(compRaiseBet);
                break;
            } else if (raiseBetter == 3) {
                uniform_int_distribution<int> uni(minimumBet, comp3Chips);
                int compRaiseBet = uni(rng);
                comp3Chips -= compRaiseBet;
                minimumBet = compRaiseBet;
                potValue += compRaiseBet;
                gameStatus = "Computer 3 raised the minimum bet to $" + to_string(compRaiseBet);
                break;
            }
            sleep(2);
        }
    }

    int firstTurn() {
        random_device rd;
        mt19937 rng(rd());
        rng.seed(time(nullptr));
        uniform_int_distribution<int> uni(0, 3);
//			int randomPlayer = 2;
        int randomPlayer = uni(rng);

        if (randomPlayer == 3) {
            gameStatus = "Computer 3 wil start the bet";
            sleep(2);
        } else if (randomPlayer == 2) {
            gameStatus = "Computer 2 wil start the bet";
            sleep(2);
        } else if (randomPlayer == 1) {
            gameStatus = "Computer 1 wil start the bet";
            sleep(2);
        } else if (randomPlayer == 0) {
            gameStatus = "Player wil start the bet";
        }
        return randomPlayer;
    }

private:
    vector<Card> createDeck() {
        vector<Card> deck;
        string suits[] = {"Clubs", "Diamonds", "Hearts", "Spades"};
        string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};

        for (const auto &suit: suits) {
            for (const auto &rank: ranks) {
                deck.push_back({suit, rank});
            }
        }

        return deck;
    }

    void shuffleDeck() {
        random_device rd;
        mt19937 rng(rd());
        rng.seed(time(nullptr));

        int n = deck.size();

        for (int i = n - 1; i > 0; --i) {
            uniform_int_distribution<int> uni(0, i);
            int j = uni(rng);
            swap(deck[i], deck[j]);
        }
    }

    void resetCardStack() {
        while (!cardStack.empty()) {
            cardStack.pop();
        }
    }

    void cardDistribution(int round) {
        if (round == 1) {
            deck = createDeck();
            players.resize(4, vector<Card>(2));
            dealer.resize(1, vector<Card>(5));

            resetCardStack();
            shuffleDeck();

            for (const auto &card: deck) {
                cardStack.push(card);
            }

            for (int cardCounter = 0; cardCounter < 2; ++cardCounter) {
                for (int playerCounter = 0; playerCounter < 4; ++playerCounter) {
                    players[playerCounter][cardCounter] = cardStack.top();
                    cardStack.pop();
                }
            }

            for (int cardCounter = 0; cardCounter < 5; ++cardCounter) {
                for (int dealerCounter = 0; dealerCounter < 1; ++dealerCounter) {
                    dealer[dealerCounter][cardCounter] = cardStack.top();
                    cardStack.pop();
                }
            }

            playerCard1 = players[0][0].rank + " of " + players[0][0].suit;
            playerCard2 = players[0][1].rank + " of " + players[0][1].suit;
            computer1Card1 = players[1][0].rank + " of " + players[1][0].suit;
            computer1Card2 = players[1][1].rank + " of " + players[1][1].suit;
            computer2Card1 = players[2][0].rank + " of " + players[2][0].suit;
            computer2Card2 = players[2][1].rank + " of " + players[2][1].suit;
            computer3Card1 = players[3][0].rank + " of " + players[3][0].suit;
            computer3Card2 = players[3][1].rank + " of " + players[3][1].suit;
        } else if (round == 2) {
            dealerCard1 = dealer[0][0].rank + " of " + dealer[0][0].suit + ", ";
            dealerCard2 = dealer[0][1].rank + " of " + dealer[0][1].suit + ", ";
            dealerCard3 = dealer[0][2].rank + " of " + dealer[0][2].suit + ", ";
        } else if (round == 3) {
            dealerCard4 = dealer[0][3].rank + " of " + dealer[0][3].suit + ", ";
            dealerCard5 = dealer[0][4].rank + " of " + dealer[0][4].suit;
        } else if (round == 4) {
            vector<string> playerRanks = {players[0][0].rank, players[0][1].rank};
            vector<string> playerSuits = {players[0][0].suit, players[0][1].suit};

            vector<string> comp1Ranks = {players[1][0].rank, players[1][1].rank};
            vector<string> comp1Suits = {players[1][0].suit, players[1][1].suit};

            vector<string> comp2Ranks = {players[2][0].rank, players[2][1].rank};
            vector<string> comp2Suits = {players[2][0].suit, players[2][1].suit};

            vector<string> comp3Ranks = {players[3][0].rank, players[3][1].rank};
            vector<string> comp3Suits = {players[3][0].suit, players[3][1].suit};

            vector<string> dealerRanks = {dealer[0][0].rank, dealer[0][1].rank, dealer[0][2].rank,
                                          dealer[0][3].rank, dealer[0][4].rank};
            vector<string> dealerSuits = {dealer[0][0].suit, dealer[0][1].suit, dealer[0][2].suit,
                                          dealer[0][3].suit, dealer[0][4].suit};

            lastRoundChecker(playerRanks, playerSuits, comp1Ranks, comp1Suits, comp2Ranks, comp2Suits,
                             comp3Ranks, comp3Suits, dealerRanks, dealerSuits);
        }
    }

    void computerBetting(int computerIdentity) {
        random_device rd;
        mt19937 rng(rd());
        rng.seed(time(nullptr));
        uniform_int_distribution<int> uni(5, 30);
        int compBet = uni(rng);

        if (computerIdentity == 1) {
            compBet = min(compBet, comp1Chips - minimumBet);
            comp1Chips -= compBet;
            if (compBet > minimumBet) {
                minimumBet = compBet;
            };
            potValue += compBet;
            gameStatus = "Computer 1 sets the minimum bet to $" + to_string(compBet);
        } else if (computerIdentity == 2) {
            compBet = min(compBet, comp2Chips - minimumBet);
            comp2Chips -= compBet;
            if (compBet > minimumBet) {
                minimumBet = compBet;
            };
            potValue += compBet;
            gameStatus = "Computer 2 sets the minimum bet to $" + to_string(compBet);
        } else if (computerIdentity == 3) {
            compBet = min(compBet, comp3Chips - minimumBet);
            comp3Chips -= compBet;
            if (compBet > minimumBet) {
                minimumBet = compBet;
            };
            potValue += compBet;
            gameStatus = "Computer 3 sets the minimum bet to $" + to_string(compBet);
        }
        sleep(2);
    }

    void playerBetting() {
        while (true) {
            cout << "How much would you want to bet: ";
            cin >> playerBet;

            if (playerBet > playerChips) {
                cout << "Insufficient chips. Please enter a valid bet" << endl;
            } else {
                gameStatus = "Player sets the minimum bet to $" + to_string(playerBet);
                if (playerBet > minimumBet) {
                    minimumBet = playerBet;
                };
                playerChips -= playerBet;
                potValue += playerBet;
                break;
            }
        }
    }

    void pokerTable(){
        system("cls");
        cout << "																Minimum Bet: $" << minimumBet
             << endl << endl;
        cout << "							Status: " << gameStatus << endl << endl << endl;
        cout << "									Dealer" << endl;
        cout << "									Cards:" << endl;
        cout << "					" << dealerCard1 << dealerCard2 << dealerCard3 << dealerCard4 << dealerCard5
             << endl;
        cout << "									Pot: $" << potValue << endl << endl << endl;
        cout << "	" << computer2 << "														" << computer3 << " "
             << endl;
        cout << "	Money: $" << comp2Chips << "														Money: $"
             << comp3Chips << endl;//cards:
        cout << "	Card1:" << computer2Card1 << "													Card1:" << computer3Card1
             << " " << endl;
        cout << "	Card2:" << computer2Card2 << "													Card2:" << computer3Card2
             << " " << endl << endl << endl << endl;
        cout << "				"<< playerName << "									" << computer1 << " "
             << endl;
        cout << "				Money: $" << playerChips << "								Money: $" << comp1Chips
             << endl;
        cout << "				" << playerCard1 << "								Card1:" << computer1Card1 << " "
             << endl;
        cout << "				" << playerCard2 << "								Card2:" << computer1Card2 << " "
             << endl;
        cout << "" << endl;

    }
};

int main() {
    PokerGame game;
    game.displayMenu();
    game.countChips();
    game.run();
    lastPage();
    return 0;
}

void lastPage(){
	PokerGame game;
	char choice;
	cout << "Press [C] to Continue and [E] to Exit: ";
	cin >> choice;
	
	switch (choice){
		case 'C':
			game.countChips();
			game.run();
			break;
		case  'E':
			main();
			break;
	}
}
