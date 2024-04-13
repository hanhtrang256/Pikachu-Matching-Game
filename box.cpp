#include "box.h"

int h_back = 0, w_back = 0;
string background[1000];
string letter[26][20][20];
int len[26] = { 9, 7, 7, 7, 7, 7, 7, 7, 5, 7, 6, 7, 8, 7, 7, 7, 7, 7, 7, 7, 7, 9, 12, 6, 7, 7 };

void fillBoxColor(int x, int y) {
	for (int i = 1; i < 6; ++i) {
		movetoPos(offset + (y - 1) * 14 + 1, 5 + (x - 1) * 7 + i);
		for (int j = 1; j < 13; ++j) cout << " ";
	}
}

void fillBoxCharacter(int x, int y, char** board) {
	int index = board[x][y] - 'A';
	for (int i = 0; i < 5; ++i) {
		movetoPos(offset + (y - 1) * 14 + ((14 - len[index]) / 2), 5 + (x - 1) * 7 + (i + 1));
		for (int j = 0; j <= len[index] + 1; j++) cout << letter[index][i][j];
	}
}

void drawBox(int x, int y, char** board) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	int color[6] = { 1, 3, 4, 9, 11, 14 };
	SetConsoleTextAttribute(console, color[rand() % 6]);
	movetoPos(offset + (y - 1) * 14, 5 + (x - 1) * 7);
	cout << ' ';
	for (int j = 1; j < 13; j++)
		cout << '_';
	for (int i = 1; i < 6; i++) {
		movetoPos(offset + (y - 1) * 14, 5 + (x - 1) * 7 + i);
		cout << '|';
		movetoPos(offset + (y - 1) * 14 + 13, 5 + (x - 1) * 7 + i);
		cout << '|';
	}
	movetoPos(offset + (y - 1) * 14, 5 + (x - 1) * 7 + 6);
	cout << ' ';
	for (int j = 1; j < 13; j++)
		cout << '-';
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	fillBoxColor(x, y);
	if (board[x][y] != ' ') fillBoxCharacter(x, y, board);
	else drawBoxBackground(x, y);
}

void drawPath(bool del, vector<pair<int, pair<int, int>>> path) {
	for (int i = 0; i < (int)path.size(); ++i) {
		int x = path[i].second.first, y = path[i].second.second;
		switch (path[i].first) {
		case 0:
			for (int j = 1; j <= 7; ++j) {
				movetoPos(offset + (y - 1) * 14 + 7, 5 + (x - 1) * 7 + 3 + j);
				if (!del) cout << '|';
				else cout << ' ';
			}
			break;
		case 1:
			for (int j = 0; j < 7; ++j) {
				movetoPos(offset + (y - 1) * 14 + 7, 5 + (x - 1) * 7 + 3 - j);
				if (!del) cout << '|';
				else cout << ' ';
			}
			break;
		case 2:
			for (int j = 1; j <= 13; ++j) {
				movetoPos(offset + (y - 1) * 14 + 7 + j, 5 + (x - 1) * 7 + 3);
				if (!del) cout << '_';
				else cout << ' ';
			}
			if (i < (int)path.size() - 1 && (path[i + 1].first == 2 || path[i + 1].first == 3)) {
				movetoPos(offset + (y - 1) * 14 + 7 + 14, 5 + (x - 1) * 7 + 3);
				if (!del) cout << '_';
				else cout << ' ';
			}
			break;
		case 3:
			for (int j = 1; j <= 13; ++j) {
				movetoPos(offset + (y - 1) * 14 + 7 - j, 5 + (x - 1) * 7 + 3);
				if (!del) cout << '_';
				else cout << ' ';
			}
			if (i < (int)path.size() - 1 && (path[i + 1].first == 2 || path[i + 1].first == 3)) {
				movetoPos(offset + (y - 1) * 14 + 7 - 14, 5 + (x - 1) * 7 + 3);
				if (!del) cout << '_';
				else cout << ' ';
			}
			break;
		default:
			break;
		}
	}
}

void drawBoxBackground(int block_x, int block_y) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	for (int i = (h_back / 5) * (block_x - 1); i < (h_back / 5) * block_x; ++i) {
		movetoPos(offset + (block_y - 1) * 14, 5 + i);
		for (int j = (w_back / 8) * (block_y - 1); j < (w_back / 8) * (block_y - 1) + 14; ++j) {
			cout << background[i][j];
		}
	}
}

void deleteBox(int x, int y) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	for (int i = 0; i < 7; i++) {
		movetoPos(offset + (y - 1) * 14, 5 + (x - 1) * 7 + i);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
	}
}

void chosenBox(int x, int y, char** board) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_BLUE);
	fillBoxColor(x, y);
	if (board[x][y] != ' ') {
		fillBoxCharacter(x, y, board);
	}
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void normalBox(int x, int y, pair<int, int> fc, char** board) {
	if (x == fc.first && y == fc.second) { // if is chosen box
		chosenBox(x, y, board);
		return;
	}
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	fillBoxColor(x, y);
	if (board[x][y] != ' ') { // if not empty
		fillBoxCharacter(x, y, board);
	}
	else {
		drawBoxBackground(x, y);
	}
}

void highlightBox(int x, int y, char** board) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	fillBoxColor(x, y);
	if (board[x][y] != ' ') {
		fillBoxCharacter(x, y, board);
	}
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void invalidBox(int x, int y, char** board) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_RED);
	fillBoxColor(x, y);
	if (board[x][y] != ' ') {
		fillBoxCharacter(x, y, board);
	}
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void validBox(int x, int y, char** board) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, BACKGROUND_GREEN);
	fillBoxColor(x, y);
	if (board[x][y] != ' ') {
		fillBoxCharacter(x, y, board);
	}
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void hintBox(int x, int y, char** board) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, BACKGROUND_RED | BACKGROUND_BLUE);
	fillBoxColor(x, y);
	if (board[x][y] != ' ') {
		fillBoxCharacter(x, y, board);
	}
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

// print board to screen
void printBoard(char** board) {
	for (int i = 1; i <= BOARD_HEIGHT; ++i) {
		for (int j = 1; j <= BOARD_WIDTH; ++j) {
			deleteBox(i, j);
			drawBox(i, j, board);
			Sleep(10);
		}
	}
}

// clear board displayed on screen
void clearBoard(char** board, int px, int py, pair<int, int> fc) {
	PlaySound(TEXT("clear.wav"), NULL, SND_FILENAME | SND_ASYNC);
	if (board[px][py] != ' ') {
		int tx = px;
		int ty = py;
		px = py = 0;
		normalBox(tx, ty, fc, board);
	}
	else drawBoxBackground(px, py);
	for (int i = 1; i <= BOARD_HEIGHT; i++) {
		for (int j = 1; j <= BOARD_WIDTH; j++) {
			if (board[i][j] != ' ') {
				drawBoxBackground(i, j);
				Sleep(10);
			}
		}
	}
	Sleep(50);
}

// read file background
void getBoardBackground() {
	ifstream fin("background.txt");
	string line;
	int ind = 0;

	while (getline(fin, line)) {
		background[ind++] = line;
		w_back = max(w_back, (int)line.size());
	}

	h_back = 35;
	// fill with blankspaces
	for (int i = 0; i < ind; ++i) {
		while ((int)background[i].size() < 69) background[i] += ' ';
		int dem = 0;
		// move background to center
		while (dem < 21) {
			background[i] = " " + background[i];
			++dem;
		}
		dem = 0;
		while (dem < 22) {
			background[i] = background[i] + " ";
			++dem;
		}
		w_back = max(w_back, background[i].size());
	}
	fin.close();
}

// get box character art from file
void getAlphabet() {
	ifstream fin("alphabets.txt");
	string s;
	int index = 0, row = 0;
	while (getline(fin, s)) {
		if (s == "*") {
			++index;
			row = 0;
		}
		else {
			for (int i = 0; i < (int)s.size(); ++i)
				letter[index][row][i] = s[i];
			++row;
		}
	}
	fin.close();
}