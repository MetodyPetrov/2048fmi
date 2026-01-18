#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_SIZE = 10;
const int MAX_NAME = 100;
const int LEADERBOARD_SIZE = 5;

void myStrcpy(char destination[], const char source[]) {
	int i = 0;

	while (source[i] != '\0') {
		destination[i] = source[i];
		i++;
	}

	destination[i] = '\0';
}
int myStrlen(const char str[]) {
	int length = 0;

	while (str[length] != '\0') {
		length++;
	}

	return length;
}
void myStrcat(char destination[], const char source[]) {
	int destLength = myStrlen(destination);
	int i = 0;

	while (source[i] != '\0') {
		destination[destLength + i] = source[i];
		i++;
	}

	destination[destLength + i] = '\0';
}
char digitToChar(const int number) {
	return char(number + '0');
}
void buildLeaderboardFilename(char filename[], int const size) {
	myStrcpy(filename, "leaderboard_");

	char digit[2];
	digit[0] = digitToChar(size);
	digit[1] = '\0';

	myStrcat(filename, digit);
	myStrcat(filename, "x");
	myStrcat(filename, digit);
	myStrcat(filename, ".txt");
}


void clearBoard(int board[MAX_SIZE][MAX_SIZE], const int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j] = 0;
		}
	}
}

void printBoard(int board[MAX_SIZE][MAX_SIZE], const int size, const int score) {
	cout << "\nScore: " << score << "\n\n";
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << board[i][j] << "\t";
		}
		cout << "\n";
	}
}

void addRandomTile(int board[MAX_SIZE][MAX_SIZE], const int size) {
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

bool hasMoves(int board[MAX_SIZE][MAX_SIZE], const int size) {
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

bool moveLeft(int board[MAX_SIZE][MAX_SIZE], const int size, int& score) {
	bool moved = false;

	for (int i = 0; i < size; i++) {
		int temp[MAX_SIZE] = { 0 };
		int index = 0;

		for (int j = 0; j < size; j++) {
			if (board[i][j] != 0) {
				temp[index++] = board[i][j];
			}
		}

		for (int j = 0; j < index - 1; j++) {
			if (temp[j] == temp[j + 1]) {
				temp[j] *= 2;
				score += temp[j];
				temp[j + 1] = 0;
				moved = true;
			}
		}

		int finalRow[MAX_SIZE] = { 0 };
		index = 0;
		for (int j = 0; j < size; j++) {
			if (temp[j] != 0) {
				finalRow[index++] = temp[j];
			}
		}

		for (int j = 0; j < size; j++) {
			if (board[i][j] != finalRow[j]) {
				moved = true;
			}
			board[i][j] = finalRow[j];
		}
	}

	return moved;
}

bool moveRight(int board[MAX_SIZE][MAX_SIZE], const int size, int& score) {
	bool moved = false;

	for (int i = 0; i < size; i++) {
		int temp[MAX_SIZE] = { 0 };
		int index = size - 1;

		for (int j = size - 1; j >= 0; j--) {
			if (board[i][j] != 0) {
				temp[index--] = board[i][j];
			}
		}

		for (int j = size - 1; j > 0; j--) {
			if (temp[j] == temp[j - 1] && temp[j] != 0) {
				temp[j] *= 2;
				score += temp[j];
				temp[j - 1] = 0;
				moved = true;
			}
		}

		int finalRow[MAX_SIZE] = { 0 };
		index = size - 1;
		for (int j = size - 1; j >= 0; j--) {
			if (temp[j] != 0) {
				finalRow[index--] = temp[j];
			}
		}

		for (int j = 0; j < size; j++) {
			if (board[i][j] != finalRow[j]) {
				moved = true;
			}
			board[i][j] = finalRow[j];
		}
	}

	return moved;
}

bool moveUp(int board[MAX_SIZE][MAX_SIZE], const int size, int& score) {
	bool moved = false;

	for (int col = 0; col < size; col++) {
		int temp[MAX_SIZE] = { 0 };
		int index = 0;

		for (int row = 0; row < size; row++) {
			if (board[row][col] != 0) {
				temp[index++] = board[row][col];
			}
		}

		for (int i = 0; i < index - 1; i++) {
			if (temp[i] == temp[i + 1]) {
				temp[i] *= 2;
				score += temp[i];
				temp[i + 1] = 0;
				moved = true;
			}
		}

		int finalCol[MAX_SIZE] = { 0 };
		index = 0;
		for (int i = 0; i < size; i++) {
			if (temp[i] != 0) {
				finalCol[index++] = temp[i];
			}
		}

		for (int row = 0; row < size; row++) {
			if (board[row][col] != finalCol[row]) {
				moved = true;
			}
			board[row][col] = finalCol[row];
		}
	}

	return moved;
}

bool moveDown(int board[MAX_SIZE][MAX_SIZE], const int size, int& score) {
	bool moved = false;

	for (int col = 0; col < size; col++) {
		int temp[MAX_SIZE] = { 0 };
		int index = size - 1;

		for (int row = size - 1; row >= 0; row--) {
			if (board[row][col] != 0) {
				temp[index--] = board[row][col];
			}
		}

		for (int i = size - 1; i > 0; i--) {
			if (temp[i] == temp[i - 1] && temp[i] != 0) {
				temp[i] *= 2;
				score += temp[i];
				temp[i - 1] = 0;
				moved = true;
			}
		}

		int finalCol[MAX_SIZE] = { 0 };
		index = size - 1;
		for (int i = size - 1; i >= 0; i--) {
			if (temp[i] != 0) {
				finalCol[index--] = temp[i];
			}
		}

		for (int row = 0; row < size; row++) {
			if (board[row][col] != finalCol[row]) {
				moved = true;
			}
			board[row][col] = finalCol[row];
		}
	}

	return moved;
}

void updateLeaderboard(const char name[], const int score, const int size) {
	char filename[50];
	buildLeaderboardFilename(filename, size);

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

	if (pos >= LEADERBOARD_SIZE && count == LEADERBOARD_SIZE) {
		return;
	}

	for (int i = count; i > pos; i--) {
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
	outFile.close();
}

void showLeaderboard() {
	int size;
	cout << "Board size: ";
	cin >> size;

	char filename[50];
	buildLeaderboardFilename(filename, size);

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
	cout << "\n";
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

		if (moved) {
			if (score >= 30) break;
			addRandomTile(board, size);
		}
	}

	printBoard(board, size, score);
	cout << "\nGame Over!\n\n";
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
