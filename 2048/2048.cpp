#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_SIZE = 10;
const int MAX_NAME = 100;
const int LEADERBOARD_SIZE = 5;

void clearBoard(int board[MAX_SIZE][MAX_SIZE], int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j] = 0;
		}
	}
}

void printBoard(int board[MAX_SIZE][MAX_SIZE], int size, int score) {
	cout << "\nScore: " << score << "\n\n";
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << board[i][j] << "\t";
		}
		cout << "\n";
	}
}

void addRandomTile(int board[MAX_SIZE][MAX_SIZE], int size) {
	int emptyRow[MAX_SIZE * MAX_SIZE];
	int emptyCol[MAX_SIZE * MAX_SIZE];
	int count = 0;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == 0) {
				emptyRow[count] = i;
				emptyCol[count] = j;
				count++;
			}
		}
	}

	if (count == 0) {
		return;
	}

	int index = rand() % count;
	int value = (rand() % 10 < 8) ? 2 : 4;

	board[emptyRow[index]][emptyCol[index]] = value;
}

bool hasMoves(int board[MAX_SIZE][MAX_SIZE], int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == 0) {
				return true;
			}
			if (i + 1 < size && board[i][j] == board[i + 1][j]) {
				return true;
			}
			if (j + 1 < size && board[i][j] == board[i][j + 1]) {
				return true;
			}
		}
	}
	return false;
}


bool moveLeft(int board[MAX_SIZE][MAX_SIZE], int size, int& score) {
	
}
bool moveRight(int board[MAX_SIZE][MAX_SIZE], int size, int& score) {

}
bool moveUp(int board[MAX_SIZE][MAX_SIZE], int size, int& score) {

}
bool moveDown(int board[MAX_SIZE][MAX_SIZE], int size, int& score) {

}

void updateLeaderboard(const char name[], int score, int size) {
	char filename[50];
	// buildLeaderboardFilename(filename, size); TODO...

	char names[LEADERBOARD_SIZE][MAX_NAME];
	int scores[LEADERBOARD_SIZE];
	int count = 0;

	ifstream inFile(filename);
	while (inFile >> names[count] >> scores[count]) {
		count++;
	}
	inFile.close();

	int pos = count;
	for (int i = 0; i < count; i++) {
		if (score > scores[i]) {
			pos = i;
			break;
		}
	}

	if (pos >= LEADERBOARD_SIZE) {
		return;
	}

	for (int i = LEADERBOARD_SIZE - 1; i > pos; i--) {
		myStrcpy(names[i], names[i - 1]);
		scores[i] = scores[i - 1];
	}

	myStrcpy(names[pos], name);
	scores[pos] = score;

	if (count < LEADERBOARD_SIZE) {
		count++;
	}

	ofstream outFile(filename);
	for (int i = 0; i < count; i++) {
		outFile << names[i] << " " << scores[i] << "\n";
	}
}

void showLeaderboard() {
	int size;
	cout << "Board size: ";
	cin >> size;

	char filename[50];
	// buildLeaderboardFilename(filename, size); TODO...

	ifstream file(filename);
	if (!file.is_open()) {
		cout << "No such board exists.\n";
		return;
	}

	char name[MAX_NAME];
	int score;

	cout << "\nLeaderboard " << size << "x" << size << ":\n";
	while (file >> name >> score) {
		cout << name << " - " << score << "\n";
	}
	file.close();
}

void startGame() {
	int board[MAX_SIZE][MAX_SIZE];
	int size = 0;
	int score = 0;
	char name[MAX_NAME];

	cout << "Enter nickname: ";
	cin >> name;

	do {
		cout << "Board size (4-10): ";
		cin >> size;
	} while (size < 4 || size > 10);

	clearBoard(board, size);
	addRandomTile(board, size);
	addRandomTile(board, size);

	while (hasMoves(board, size)) {
		printBoard(board, size, score);
		cout << "\nMove (w/a/s/d): ";

		char command;
		cin >> command;

		bool moved = false;

		if (command == 'a') moved = moveLeft(board, size, score);
		else if (command == 'd') moved = moveRight(board, size, score);
		else if (command == 'w') moved = moveUp(board, size, score);
		else if (command == 's') moved = moveDown(board, size, score);

		// TODO: 2048 hit logic (optional?)

		if (moved) {
			addRandomTile(board, size);
		}
	}

	printBoard(board, size, score);
	cout << "\nGame Over!\n";
	updateLeaderboard(name, score, size);
}

int main() {
	srand((unsigned int)time(NULL));

	int choice;

	do {
		cout << "--- 2048 ---\n";
		cout << "1. Start Game\n";
		cout << "2. Leaderboard\n";
		cout << "3. Exit\n";
		cout << "Choice: ";
		cin >> choice;

		if (choice == 1) {
			startGame();
		}
		else if (choice == 2) {
			showLeaderboard();
		}

	} while (choice != 3);

	return 0;
}
